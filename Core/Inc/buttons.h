/*
 * buttons.h
 *
 *  Created on: Oct 31, 2020
 *      Author: muaddib
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#define MAX_BUTTONS_QTY 5

typedef enum
{
	BUTTON_UP = 1,
	BUTTON_MIN = BUTTON_UP,
	BUTTON_DOWN = 2,
	BUTTON_LEFT = 4,
	BUTTON_RIGHT = 8,
	BUTTON_ENTER = 16,
	BUTTON_MAX = BUTTON_ENTER,
} BUTTON_ID;

typedef enum
{
	PRESSED,
	RELEASED,
	CLICKED,
} BUTTON_STATE;

typedef struct
{
	BUTTON_ID id;
	BUTTON_STATE state;
} s_button;

void buttons_task(void *argument);

#endif /* INC_BUTTONS_H_ */
