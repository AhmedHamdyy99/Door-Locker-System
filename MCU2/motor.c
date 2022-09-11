/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the motor
 Date        : 21/7/2021
 ============================================================================
 */

#include "motor.h"
#include "timer_0.h"
#include "USART.h"

/*****************FUNCTIONS DEFINTIONS *****************/
uint8 volatile Motor_OFF=0;
/*global variable that flags the motor for ON/OFF operations*/
uint8  sec=0;
uint8  tick=0;

/*Motor configurations function*/
void motor_clockwise(void)
{
	/*Funtion that Rotate the motor clockwise */
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_1_BIT_PIN_2);
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_2_BIT_PIN_7);

	/*input pins for the motor which are output pins for the mcu*/

	CLEAR_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_1_BIT_PIN_2);
	SET_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_2_BIT_PIN_7);
	SET_BIT(MOTOR_INPUT_PORT,PC7);
	/* Motor ROTATE CLOCKWISE  at the beginning */

}
void motor_anticlockwise(void)
{
	/*Funtion that Rotate the motor anticlockwise */
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_1_BIT_PIN_2);
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_2_BIT_PIN_7);
	/*input pins for the motor which are output pins for the mcu*/

	SET_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_1_BIT_PIN_2);
	CLEAR_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_2_BIT_PIN_7);
	SET_BIT(MOTOR_INPUT_PORT,PC7);
	/* Motor ROTATE CLOCKWISE  at the beginning */

}
void motor_off(void)
{
	/*Funtion that shutdown the motor */
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_1_BIT_PIN_2);
	SET_BIT(MOTOR_INPUT_PORT_DIRECTION,MOTOR_INPUT_2_BIT_PIN_7);
	/*input pins for the motor which are output pins for the mcu*/

	CLEAR_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_1_BIT_PIN_2);
	CLEAR_BIT(MOTOR_INPUT_PORT,MOTOR_INPUT_2_BIT_PIN_7);
	CLEAR_BIT(MOTOR_INPUT_PORT,PC7);
	/* Motor ROTATE CLOCKWISE  at the beginning */

}
void doorAction(void)
{
	/*Function that perform the action of the open door command in the projects
	 * it function on the timer0
	 * first 15 sec motor on clockwise
	 * 3 sec off
	 * 15 anticlockwise
	 * then OFF
	 * */
	tick++;
	if(tick==NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK_1024)
	{
		sec++;
		if(sec==1)
		{
			/*wait for the UI MCU to send the door command so the CTRL MCU
			 * can send the door locking string that will be on the screen simultaneously with
			 * the motor running
			 * motor clockwise*/

			while(UART_recieveByte() != DOOR_LOCKING){} // wait until MC1 is ready
			UART_sendString("Door is Locking$");
			motor_clockwise();
		}
		if(sec==15)
		{
			/*wait for the UI MCU to send the door command so the CTRL MCU
			 * can send the door locking string that will be on the screen simultaneously with
			 * the motor running
			 * motor stop*/

			while(UART_recieveByte() != DOOR_STOP){} // wait until MC1 is ready
			UART_sendString("Door Stopped$");
			motor_off();
		}
		if(sec==18)
		{
			/*wait for the UI MCU to send the door command so the CTRL MCU
			 * can send the door locking string that will be on the screen simultaneously with
			 * the motor running
			 * motor anticlockwise*/

			while(UART_recieveByte() != DOOR_UNLOCKING){} // wait until MC1 is ready
			UART_sendString("Door Unlocking$");
			motor_anticlockwise();
		}
		if(sec==33)
		{
			/*wait for the UI MCU to send the door command so the CTRL MCU
			 * can send the door locking string that will be on the screen simultaneously with
			 * the motor running
			 * MOTOR_SHUTDOWN
			 * TIMER_0_SHUTDOWN*/

			UART_sendByte(DOOR_OFF); // MC2 is ready
			while(UART_recieveByte() != DOOR_OFF){} // wait until MC1 is ready
			UART_sendString("Action Done$");
			motor_off();
			sec=0;
			tick=0;
			Motor_OFF=1;
			Timer0_deinit();
		}
		tick=0;
	}

}

