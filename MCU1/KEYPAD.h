/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Header file for the KEYPAD
 Date        : 29/3/2021
 ============================================================================
 */

/*****************GUARDING *****************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

extern unsigned char key;
/*****************STATIC CONFIGURATIONS *****************/
/* Keypad configurations for number of rows and columns */
#define N_col 4
#define N_row 4
#define KEYPAD_OUT_PORT PORTA
#define KEYPAD_IN_PORT PINA
#define KEYPAD_PORT_DIR DDRA

/*****************FUNCTIONS PROTOTYPE *****************/
uint8 KeyPad_getPressedKey(void);


#endif
