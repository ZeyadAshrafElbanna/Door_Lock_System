/*
 * Timer_1.h
 *
 *  Created on: 2 Nov 2023
 *      Author: Zeyad El-banna
 */

#ifndef TIMER_1_H_
#define TIMER_1_H_

#include "std_types.h"
/****************************************************************************************
 *                             Types Declaration
 ****************************************************************************************/
typedef enum
{
	 No_Clock , F_CPU_0 , F_CPU_8 ,F_CPU_64 , F_CPU_256 ,F_CPU_1024 , EXTERNAL_FALING , EXTERNAL_RAISING
}Timer1_Prescaler;

typedef enum
{
	normal , compar = 4
}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*********************************************************************************************
 *                                      Functions
 *********************************************************************************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));




#endif /* TIMER_1_H_ */
