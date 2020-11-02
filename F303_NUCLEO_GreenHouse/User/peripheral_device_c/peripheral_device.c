#include "peripheral_device.h"

void PeripheralDevices_Init(void)
{
	ManualControl_KEY_Config();
	Lamp_Config();
	ULN2003_Config();
	Heater_Config();
	Cooler_Config();
	CO2_Generator_Config();
	Ventilator_Config();
	Water_Sprayer_Config();
	ALARM_Config();
}




