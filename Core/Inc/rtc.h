/*
 * rtc.h
 *
 *  Created on: Oct 6, 2020
 *      Author: muaddib
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include <stdint.h>



#define RCT_MAGIC_WORD 0xdead

uint32_t RTC_Init(void);

#endif /* INC_RTC_H_ */
