/*
 * debug.c
 *
 *  Created on: Oct 6, 2020
 *      Author: muaddib
 */



#include "stm32f1xx.h"
#include "system_stm32f1xx.h"

#include "debug.h"


#if DEBUG_SVO

int _write ( int file, char *ptr, int len )
{
  int i = 0;
  for ( i = 0; i < len; i++ )
    ITM_SendChar ( *ptr++ );
  return len;
}

#endif

