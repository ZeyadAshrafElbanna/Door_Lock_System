/*
 * Timer_1.c
 *
 *  Created on: 2 Nov 2023
 *      Author: Zeyad El-banna
 */

#include "Timer_1.h"
#include "common_macros.h"
#include <avr/io.h>
#include<avr/interrupt.h>

static volatile void (*g_callBackPtr)(void) = NULL_PTR;


ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
   TCCR1A = (1<<FOC1A) | (1<<FOC1B) | (((Config_Ptr->mode)&0x03) | (TCCR1A & 0xFC));
   TCCR1B = (((Config_Ptr->mode)>>2)<<WGM12) | ((Config_Ptr->prescaler) | (TCCR1B & 0xF8));
   TCNT1 = Config_Ptr->initial_value;
   OCR1A = Config_Ptr->compare_value;
   if((Config_Ptr->mode) == compar )
   {
	   TIMSK = (1<<OCIE1A);
   }
   else if ((Config_Ptr->mode) == normal)
   {
	   TIMSK = (1<<TOIE1);
   }


}
void Timer1_deInit(void)
{
	TCCR1B = 0 ;
	TCNT1 = 0;
	OCR1A = 0 ;
}
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

