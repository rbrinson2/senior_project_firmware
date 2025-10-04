#include "ina260.h"
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

int error;

void sensorInit() {
    error = wiringPiSetup();
}

