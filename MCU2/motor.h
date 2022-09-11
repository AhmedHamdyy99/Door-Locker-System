/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Header file for the motor
 Date        : 21/7/2021
 ============================================================================
 */


/*****************GUARDING *****************/
#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*****************STATIC CONFIGURATIONS *****************/
#define MOTOR_INPUT_PORT_DIRECTION DDRC
#define MOTOR_INPUT_1_BIT_PIN_2    PC5
#define MOTOR_INPUT_2_BIT_PIN_7    PC6
#define MOTOR_INPUT_PORT           PORTC
#define DOOR_LOCKING               0x99
#define DOOR_UNLOCKING             0x98
#define DOOR_STOP                  0x97
#define DOOR_OFF                   0x96
#define IS_DOOR_OFF                0x84

extern uint8 volatile Motor_OFF;
/*****************FUNCTION DECLRATIONS *****************/
void motor_off(void);
void motor_anticlockwise(void);
void motor_clockwise(void);
void doorAction(void);
#endif


