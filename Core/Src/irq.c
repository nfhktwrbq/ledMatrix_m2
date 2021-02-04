/*
 * irq.c
 *
 *  Created on: Oct 21, 2020
 *      Author: muaddib
 */

#include "irq.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case IRQ_Pin:
		  if(buttonSemHandle != NULL)
      {
        osSemaphoreRelease(buttonSemHandle);
      }
			break;
		case BUTTON_DOWN_Pin:

			break;
		default:
			break;
	}
}
