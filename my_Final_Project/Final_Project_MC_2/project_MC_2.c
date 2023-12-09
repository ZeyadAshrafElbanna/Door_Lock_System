/*
 * project_MC_2.c
 *
 *  Created on: 2 Nov 2023
 *      Author: Zeyad El-banna
 */
#include"uart.h"
#include"twi.h"
#include"Timer_1.h"
#include"DC_Motor.h"
#include"Buzzer.h"
#include"external_eeprom.h"
#include<util/delay.h>
#include<avr/io.h>

/**********************************************************************************************
 *                              Preprocessor Macros
 **********************************************************************************************/

#define I_AM_READY 0xFF
#define SAME_PASS 0x01
#define NOT_SAME_PASS 0x00
#define DOOR_OPENED 0x02
#define DOOR_CLOSED 0x03
#define START 0x44
#define ENTER_PASS 0xD0
#define RE_ENTER_PASS 0xC0
#define ENTER 13
#define ENTER_PASS_TWO 0xE0
#define CHANGE_PASS 0xF0
#define ALARM 0xA0
#define WRONG 0xAA

/*************************************************************************************************
 *                             Global variables
 *************************************************************************************************/

volatile uint16 add = 0x0311;
volatile static uint8 ticks = 0;
volatile static uint8 chances= 0;

/**************************************************************************************************
 *                             Function Prototypes
 *************************************************************************************************/

void timer_isr(void);

int main(void)
{
	/* enable global I bit */
	SREG |= (1<<7);
	/* configurations of timer to use in delay and motor timing */
	Timer1_ConfigType Timer_config = {26473 , 0 ,F_CPU_1024,normal };
	Timer1_ConfigType Timer_config_hold = {42098, 0 ,F_CPU_1024,normal };
	/* set timer ISR function */
	Timer1_setCallBack(timer_isr);
	/* initialise motor and buzzer */
	DcMotor_Init();
	Buzzer_init();
	/* I2C configuration and initialisation */
	TWI_ConfigType TWI_config = {0b00000010 , 400000};
	TWI_init(&TWI_config);
	/* UART configuration and initialisation*/
	UART_ConfigType UART_Config = {eghit , diabled , one_bit ,9600};
	UART_init(&UART_Config);
	_delay_ms(500);
	/*sending ready to MC_1 to start receive instructions of each operation*/
	UART_sendByte(I_AM_READY);
	/* data received from MC_1*/
	uint8 data ;
	/*variables to receive passwords from MC_1 */
	uint8 data_pass  , data_pass_2, data_rePass;
	uint32 pass = 0 , pass_2 = 0, re_pass = 0;
	/* counter variable */
	uint8 i ;
	/* variables to read and write in EEPROM */
	uint32 num = 0 , n;
	while(1)
	{
		/* receive instruction of operation from MC_1  */
		data = UART_recieveByte();
		/* control first enter password condition */
		if(data == ENTER_PASS)
		{
			/* receive 5 numbers of password from MC_1 */
			pass = 0;
			for( i = 0  ; i<5 ; i++)
			  {
			     data_pass = UART_recieveByte();
			     pass = (pass*10) + data_pass;
			  }
		}
		/* control re_entered password condition */
		else if (data == RE_ENTER_PASS)
		{
			/* receive 5 numbers of re_password from MC_1 */
			re_pass = 0;
			for( i = 0  ; i<5 ; i++)
				{
				  data_rePass = UART_recieveByte();
				  re_pass = (re_pass*10) + data_rePass;
				}
		}
		/* compare password and re_password condition */
		else if (data == ENTER)
		{
			num = 0;
			/* when they are equal save password in EEPROM */
			if(pass == re_pass)
			{
				UART_sendByte(SAME_PASS);
				for(i = 0 ; i<5 ; i++ )
				{
					num = pass%10;
					pass = pass/10 ;
					EEPROM_writeByte((add+4-i),num);
					_delay_ms(10);
				}
			}
			/* other that send not_same_pass to MC_1*/
			else
			{
				UART_sendByte(NOT_SAME_PASS);
			}
		}
		/* control second entered password from main options condition */
		else if (data == ENTER_PASS_TWO)
		{
			/* get password saved in EEPROM */
			num = 0;
			pass_2 = 0;
			for(i=0 ;i<5;i++ )
				{
					 EEPROM_readByte((add+i),&n);
					 num = (num*10) + n ;
					 _delay_ms(10);
				}
			/* receive 5 numbers of password from MC_1 */
			UART_sendByte(START);
			for( i = 0  ; i<5 ; i++)
				 {
				      data_pass_2 = UART_recieveByte();
					  pass_2 = (pass_2*10) + data_pass_2;
				 }
			/* start comparing of saved password and entered password */
			if(UART_recieveByte() == ENTER)
			{
			   UART_sendByte(START);
			}
			/* if entered password is true */
			if(num == pass_2)
			{
				/* send open door to MC_1 */
				UART_sendByte(START);
				/* rotate motor clockwise with full speed */
				UART_sendByte(DOOR_OPENED);
				DcMotor_Rotate(clock_wise,100);
				/* start timer to calculate and hold for 15 seconds */
				Timer1_init(&Timer_config);
				while(ticks != 3){}
				/* stop motor and close timer */
				DcMotor_Rotate(stop_motor,0);
				Timer1_deInit();
				ticks = 0 ;
				/* start timer to hold for 3 seconds */
				Timer1_init(&Timer_config_hold);
				while(ticks != 1){}
				/* stop timer and send door_locking to MC_1 */
				Timer1_deInit();
				ticks = 0;
				while(UART_recieveByte() != DOOR_CLOSED){}
				UART_sendByte(START);
				UART_sendByte(DOOR_CLOSED);
				/* rotate motor anti clockwise with full speed */
				DcMotor_Rotate(anti_clock_wise,100);
				/*start timer to calculate 15 seconds */
				Timer1_init(&Timer_config);
				while(ticks != 3){}
				/* stop motor and close timer */
			    DcMotor_Rotate(stop_motor,0);
				Timer1_deInit();
			    ticks = 0;
			}
			/* if entered password was wrong */
			else if (num != pass_2)
			{
				/* give him 3 chances */
                chances ++;
                /* if chances equal 3 send Alarm instruction to MC_1 */
                if(chances == 3)
                {
                	chances = 0;
                	UART_sendByte(START);
                	UART_sendByte(ALARM);
                }
                else
                {
                	UART_sendByte(START);
                 	UART_sendByte(WRONG);
                }
			}
		}
		/* change password condition */
		else if (data == CHANGE_PASS)
		{
			/* first get old password from EEPROM */
			num = 0;
			pass_2 = 0;
			for(i=0 ;i<5;i++ )
				{
					 EEPROM_readByte((add+i),&n);
					 num = (num*10) + n ;
					 _delay_ms(10);
				}
			/* get password from user in MC_1 */
			UART_sendByte(START);
			for( i = 0  ; i<5 ; i++)
				{
				    data_pass_2 = UART_recieveByte();
                    pass_2 = (pass_2*10) + data_pass_2;
			    }
			if(UART_recieveByte() == ENTER)
				{
				  UART_sendByte(START);
				}
			/* if two passwords are equal send one to MC_1 */
			if(num == pass_2)
				{
				  UART_sendByte(1);
				}
			/* if not send 0 to MC_1 */
			else if (num != pass_2)
				{
				  UART_sendByte(0);
				}
		}
		/* Alarm condition */
		else if (data == ALARM)
		{
			/* initialise timer to hold buzzer on for 1 min */
			Timer1_ConfigType Timer_config_buzzer = {0 , 0 ,F_CPU_1024,normal };
			UART_sendByte(START);
			Buzzer_on();
			Timer1_init(&Timer_config_buzzer);
			while(ticks != 7){}
			/* turn off buzzer and close timer */
			Buzzer_off();
			Timer1_deInit();
			ticks = 0 ;
		}
	}
	return 0 ;
}
/* timer ISR definition */
void timer_isr(void)
{
	ticks ++;
}
