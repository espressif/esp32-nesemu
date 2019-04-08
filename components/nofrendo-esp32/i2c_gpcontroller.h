#ifndef _I2C_KEYBOARD_H_
#define _I2C_KEYBOARD_H_

#include <stdio.h>
#include "driver/i2c.h"

void i2c_gpcontrollerInit();
int i2c_gpReadInput();

#endif
