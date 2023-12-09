/*
 * DC_Motor.c
 *
 *  Created on: 13 Oct 2023
 *      Author: Zeyad El-banna
 */
#include"DC_Motor.h"
#include"gpio.h"
#include"PWM.h"
void DcMotor_Init(void)
{
   GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN_1, PIN_OUTPUT);
   GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN_2, PIN_OUTPUT);
   GPIO_setupPinDirection(MOTOR_ENABLE_PORT , MOTOR_ENABLE_PIN, PIN_OUTPUT);
   GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1, LOGIC_LOW);
   GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2, LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
   switch(state)
   {
       case 0 :  GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1, LOGIC_LOW);
                 GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2, LOGIC_LOW);
                 break ;
       case 1 :  GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1, LOGIC_HIGH);
                 GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2, LOGIC_LOW);
                 break ;
       case 2 :  GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_1, LOGIC_LOW);
                 GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN_2, LOGIC_HIGH);
                 break;
   }
   uint8 duty_cycle = 255 * ((float32)speed / 100) ;
   PWM_Timer0_Start(duty_cycle);

}

