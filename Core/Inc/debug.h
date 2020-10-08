/*
 * debug.h
 *
 *  Created on: Oct 6, 2020
 *      Author: muaddib
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <stdio.h>

#define DEBUG_SVO 	1

#define DEBUG_PRINT 	1
#define DEBUG_PRINT_MAIN (1 && DEBUG_PRINT)


#if DEBUG_PRINT_MAIN
#define _DEBUG_PRINT_MAIN(...) do{ printf( __VA_ARGS__ ); } while( 0 )
#else
#define _DEBUG_PRINT_MAIN(...) do{ } while ( 0 )
#endif

#endif /* INC_DEBUG_H_ */
