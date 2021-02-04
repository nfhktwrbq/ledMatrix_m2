/*
 * radio.c
 *
 *  Created on: Oct 31, 2020
 *      Author: muaddib
 */
#include "cmsis_os.h"
#include "global_inc.h"
#include "radio.h"
#include "si4432.h"


void radio_task(void *argument)
{
	uint8_t buf[16];
	uint8_t len = 0;
	for(;;)
	{
		DEBUG_PRINT_MAIN("R\n");
		volatile uint32_t count = osSemaphoreGetCount (buttonSemHandle);
    DEBUG_PRINT_MAIN("RC = %u\n", count);

		if(osSemaphoreAcquire(buttonSemHandle, 100) == osOK)
		{
			if(si4432_receive_AN415(buf, &len))
			{
				DEBUG_PRINT_MAIN("PL = %u\n", len);
				for(int i = 0; i < sizeof(buf); i++)
				{
					DEBUG_PRINT_MAIN("0x%x ", buf[i]);
				}
				DEBUG_PRINT_MAIN("\n");
			}
		}
		osDelay(1000);
		//_DEBUG_PRINT_MAIN("V=%u\n", si4432_get_battery_voltage());
		//_DEBUG_PRINT_MAIN("radio\n");
	}
}

