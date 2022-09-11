/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Header file for the USART
 Date        : 21/7/2021
 ============================================================================
 */


/*****************GUARDING *****************/
#ifndef USART_H_
#define USART_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
/*******************************************************************************
 *                         STATIC CONFIGS                                 *
 *******************************************************************************/
#define _5_bit 0
#define _6_bit 2
#define _7_bit 4
#define _8_bit 6
//#define _9_bit sdqwe
#define CHANGE_PASSWORD_KEY 43
#define OPEN_DOOR_KEY       45
#define CHANGE_PASSWORD   0x43
#define OPEN_DOOR         0x45
#define DOOR_LOCKING      0x99
#define DOOR_UNLOCKING    0x98
#define DOOR_STOP         0x97
#define DOOR_OFF          0x96
#define ODOOR             0x55
#define IS_DOOR_OFF       0x84
#define PASS_CONFIRM      0x70
#define PASS_NOT_CONFIRM  0x71
#define IS_PASS_EQUAL     0x72
#define SEND_PASS         0x73
#define ENTER_PASS        0x74
#define PASS_RE_CONFIRM   0x77
#define NEW_PASS          0x78
#define WRONG             0x83
#define CPASS             0x79
#define BUZZER_ON         0x75
#define BUZZER_OFF        0x76
#define BAUD_PRESCALE (((F_CPU / ( (baudrate) * 8UL))) - 1) //UBRR 8 for double speed mode U2X=1
/*******************************************************************************
 *                         TYPES DECLARATION                               *
 *******************************************************************************/
typedef enum
{
	/*defines the number of bits needed*/
   _5_bits =_5_bit , _6_bits = _6_bit ,_7_bits = _7_bit, _8_bits=_8_bit
}NO_OF_BIT_DATA;
typedef enum
{
	/*defines the parity bit needed*/
   disabled =0, EVEN = 32 ,ODD = 48
}PARITY_BIT;
typedef enum
{
	/*defines the stop bit needed*/
   _1_bit =0, _2_bit=8
}STOP_BIT;

typedef struct
{
	/*defines the config structure that is needed for the following inputs*/
	NO_OF_BIT_DATA         bitdata;
	PARITY_BIT            paritybit;
	STOP_BIT               stopbit;
	uint16             baudrate;
}USART_config_type;


/*****************FUNCTIONS PROTOTYPE *****************/
void UART_sendByte(uint8 data);
uint8 UART_recieveByte (void);
void UART_sendString(uint8 *Str);
void UART_receivestring(uint8 *Str);
void UART_init(const USART_config_type * Config_Ptr);
#endif
