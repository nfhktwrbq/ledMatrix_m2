/*
 * debug.h
 *
 *  Created on: Oct 6, 2020
 *      Author: muaddib
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <stdio.h>

#define DEBUG_SVO 		1

#define DEBUG_PRINT 		1
#define DEBUG_MAIN 		(1 && DEBUG_PRINT)
#define DEBUG_SI4432		(1 && DEBUG_PRINT)
#define DEBUG_BUTTONS	(1 && DEBUG_PRINT)


#if DEBUG_PRINT
#define DEBUG_PRINT_MAIN(...) do{ printf( __VA_ARGS__ ); } while( 0 )
#else
#define DEBUG_PRINT_MAIN(...) do{ } while ( 0 )
#endif

#if DEBUG_SI4432
#define DEBUG_SI4432_PRINT(...) do{ printf( __VA_ARGS__ ); } while( 0 )
#else
#define DEBUG_SI4432_PRINT(...) do{ } while ( 0 )
#endif

#if DEBUG_BUTTONS
#define DEBUG_BUTTONS_PRINT(...) do{ printf( __VA_ARGS__ ); } while( 0 )
#else
#define DEBUG_BUTTONS_PRINT(...) do{ } while ( 0 )
#endif

#endif /* INC_DEBUG_H_ */
