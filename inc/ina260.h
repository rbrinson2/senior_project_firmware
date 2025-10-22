

#ifndef __INA260_H__
#define __INA260_H__

#define I2C_ADDR 0x40

extern void gpmuInit();
extern float getCurrent();
extern float getVoltage();
extern float getPower();
extern void setPowerLimit(long pl);

#endif
