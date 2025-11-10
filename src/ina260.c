#include "ina260.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

#define INA260_CONFIG_ADDR  0x00
#define INA260_CURRENT_ADDR  0x01
#define INA260_VOLTAGE_ADDR  0x02
#define INA260_POWER_ADDR  0x03
#define NEGATIVE_SIGN 0x8000
#define MICRO_PLACES 9
#define INT_BASE 10

int fd_serial;
int fd_i2c;
unsigned long gpuPowerMax;
unsigned long gpuPowerMin;

void gpmuInit() {
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start WiringPi: %s\n", strerror(errno));
    }

    if ((fd_serial = serialOpen("/dev/ttyS0", 115200)) < 0) {
        fprintf(stderr, "Unable to open serial port: %s\n", strerror(errno));
    }

    fd_i2c = wiringPiI2CSetup(I2C_ADDR);
    wiringPiI2CWriteReg16(fd_i2c, INA260_CONFIG_ADDR, 0x276F);

    gpuPowerMax = getPowerMax();
    gpuPowerMin = getPowerMin();
}


void reverseHalfWord(uint16_t *halfword){
    uint16_t firstHalf = *halfword >> 8;
    uint16_t secondHalf = *halfword << 8;

    *halfword = secondHalf | firstHalf;
}

float getCurrent(){
    uint16_t c = wiringPiI2CReadReg16(fd_i2c, INA260_CURRENT_ADDR);
    reverseHalfWord(&c);
    return (int16_t)c * 0.00125;
}
float getVoltage(){
    uint16_t v = wiringPiI2CReadReg16(fd_i2c, INA260_VOLTAGE_ADDR);
    reverseHalfWord(&v);
    return (v << 1) * 0.00125;
}
float getPower(){
    uint16_t p = wiringPiI2CReadReg16(fd_i2c, INA260_POWER_ADDR);
    reverseHalfWord(&p);
    return ((int16_t)p << 1) * 0.01;
}

void setPowerLimit(long pl){
    serialPrintf(fd_serial, "setPowerLimit: %d\r\n", pl);
}
unsigned long getPowerLimit(){
    char microwatts[MICRO_PLACES + 1];
    int i = 0;

    // Flush data from previous calls
    while (serialDataAvail(fd_serial)) serialGetchar(fd_serial);

    // Call to get data
    serialPrintf(fd_serial, "getPowerLimit\r\n");
    delay(10);

    while (serialDataAvail(fd_serial) > 0 && i < MICRO_PLACES) {
        char c = serialGetchar(fd_serial);
        if (c == '\r' || c == '\n') break;
        microwatts[i++] = c;
    }
    microwatts[i] = '\0';

    if (i == 0) printf("No data recieved\r\n");

    return strtoul(microwatts, NULL, INT_BASE);
}
unsigned long getPowerMax(){
    char microwatts[MICRO_PLACES + 1];
    int i = 0;

    // Flush data from previous calls
    while (serialDataAvail(fd_serial)) serialGetchar(fd_serial);

    // Call to get data
    serialPrintf(fd_serial, "getPowerMax\r\n");
    delay(10);

    while (serialDataAvail(fd_serial) > 0 && i < MICRO_PLACES) {
        char c = serialGetchar(fd_serial);
        if (c == '\r' || c == '\n') break;
        microwatts[i++] = c;
    }
    microwatts[i] = '\0';

    if (i == 0) printf("No data recieved\r\n");

    return strtoul(microwatts, NULL, INT_BASE);
}
unsigned long getPowerMin(){
    char microwatts[MICRO_PLACES + 1];
    int i = 0;

    // Flush data from previous calls
    while (serialDataAvail(fd_serial)) serialGetchar(fd_serial);

    // Call to get data
    serialPrintf(fd_serial, "getPowerMin\r\n");
    delay(10);

    while (serialDataAvail(fd_serial) > 0 && i < MICRO_PLACES) {
        char c = serialGetchar(fd_serial);
        if (c == '\r' || c == '\n') break;
        microwatts[i++] = c;
    }
    microwatts[i] = '\0';

    if (i == 0) printf("No data recieved\r\n");

    long mw = strtoul(microwatts, NULL, INT_BASE);
    if (mw <= 0) mw = (long)(gpuPowerMax * 0.8);

    return mw;
}
