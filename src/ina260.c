#include "ina260.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>


int fd_serial;
int fd_i2c;

void gpmuInit() {
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start WiringPi: %s\n", strerror(errno));
    }

    if ((fd_serial = serialOpen("/dev/ttyAMA0", 115200)) < 0) {
        fprintf(stderr, "Unable to open serial port: %s\n", strerror(errno));
    }

    fd_i2c = wiringPiI2CSetup(I2C_ADDR);
}

