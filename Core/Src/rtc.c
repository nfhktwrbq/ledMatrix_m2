/*
 * rtc.c
 *
 *  Created on: Oct 5, 2020
 *      Author: muaddib
 */

#include "rtc.h"

/*
 * rtc.c
 *
 *  Created on: Oct 5, 2020
 *      Author: muaddib
 */
#include "stm32f1xx.h"
#include "rtc.h"

#define RTC_TIMEOUT 1000u
#define LSE_FREQ 	32768

uint32_t RTC_Init(void)
{
    uint32_t prescaler = 0U;

    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR |=  PWR_CR_DBP;

    /* Waiting for synchro */
    RTC->CRL &= ~RTC_CRL_RSF;


    uint32_t tickstart = 0U;


    tickstart = HAL_GetTick();

    /* Wait the registers to be synchronised */
    while ((RTC->CRL & RTC_CRL_RSF) == 0u)
    {
      if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT)
      {
        return 2;
      }
    }


    if(BKP->DR1 == RCT_MAGIC_WORD)
    {
      return 0;
    }


    tickstart = HAL_GetTick();
    /* Wait till RTC is in INIT state and if Time out is reached exit */
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0u)
    {
        if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT)
        {
            return 2;
        }
    }


    /* Disable the write protection for RTC registers */
    RTC->CRL |= RTC_CRL_CNF;



    /* Set Initialization mode */


    /* Clear Flags Bits */
    RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);

    prescaler = LSE_FREQ - 1U;

    /* Configure the RTC_PRLH / RTC_PRLL */
    RTC->PRLH |= (prescaler >> 16U);
    RTC->PRLL |= (prescaler & 0x0000ffff);

    RTC->CNTH = 0;
    RTC->CNTL = 0;

    BKP->DR1 = RCT_MAGIC_WORD;

    /* Wait for synchro */
    RTC->CRL &= ~(1 << RTC_CRL_CNF);
    tickstart = HAL_GetTick();
    /* Wait till RTC is in INIT state and if Time out is reached exit */
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0u)
    {
      if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT)
      {
         return 2;
      }
    }




    return 0;

}

