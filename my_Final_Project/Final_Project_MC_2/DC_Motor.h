/*
 * DC_Motor.h
 *
 *  Created on: 13 Oct 2023
 *      Author: Zeyad El-banna
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include"std_types.h"
#define MOTOR_PORT_ID PORTB_ID
#define MOTOR_PIN_1   PIN0_ID
#define MOTOR_PIN_2   PIN1_ID
#define MOTOR_ENABLE_PORT   PORTB_ID
#define MOTOR_ENABLE_PIN   PIN3_ID
typedef enum
{
	stop_motor , clock_wise , anti_clock_wise
}DcMotor_State;
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
#endif /* DC_MOTOR_H_ */
