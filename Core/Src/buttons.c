/*
 * buttons.c
 *
 *  Created on: Oct 31, 2020
 *      Author: muaddib
 */
#include "cmsis_os.h"
#include "global_inc.h"
#include "buttons.h"

static BUTTON_ID _prev_press_state = 0;
static BUTTON_ID _cur_press_state = 0;

static BUTTON_ID _get_press_state(void)
{
	BUTTON_ID state = 0;

	if(HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_RESET)
	{
		state |= BUTTON_DOWN;
	}

	if(HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_RESET)
	{
		state |= BUTTON_UP;
	}

	if(HAL_GPIO_ReadPin(BUTTON_LEFT_GPIO_Port, BUTTON_LEFT_Pin) == GPIO_PIN_RESET)
	{
		state |= BUTTON_LEFT;
	}

	if(HAL_GPIO_ReadPin(BUTTON_RIGHT_GPIO_Port, BUTTON_RIGHT_Pin) == GPIO_PIN_RESET)
	{
		state |= BUTTON_RIGHT;
	}

	if(HAL_GPIO_ReadPin(BUTTON_ENTER_GPIO_Port, BUTTON_ENTER_Pin) == GPIO_PIN_RESET)
	{
		state |= BUTTON_ENTER;
	}

	return state;
}

static void _handle_pressed_buttons(void)
{

	for(BUTTON_ID iter_button = BUTTON_MIN; iter_button <= BUTTON_MAX; iter_button <<= 1)
	{
		if( (iter_button & _cur_press_state) ^ (iter_button & _prev_press_state) ) //state change
		{
			if(iter_button & _cur_press_state)// button press
			{
				DEBUG_BUTTONS_PRINT("button %u pressed\n", iter_button);
			}
			else //button release
			{
				DEBUG_BUTTONS_PRINT("button %u released\n", iter_button);
			}
		}
	}
}

void buttons_task(void *argument)
{
	while(1)
	{
	  DEBUG_PRINT_MAIN("B\n");
		_cur_press_state = _get_press_state();

		if(_cur_press_state != _prev_press_state)
		{
			_handle_pressed_buttons();
			_prev_press_state = _cur_press_state;
		}
		osDelay(1000);

	}
}
