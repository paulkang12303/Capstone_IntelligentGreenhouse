#ifndef _PERIPHERAL_DEVICE_H
#define _PERIPHERAL_DEVICE_H

#include "manual_controller.h"
#include "lamp.h"
#include "uln2003_28BYJ_48.h"
#include "airconditioner.h"
#include "co2_generator.h"
#include "ventilator.h"
#include "water_sprayer.h"
#include "alarm.h"
#include "sensor_L432.h"
#include "i2c_gpio.h"
#include "oled.h"

void PeripheralDevices_Init(void);

#endif
