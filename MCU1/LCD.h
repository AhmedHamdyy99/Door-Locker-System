/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Header file for the LCD
 Date        : 29/3/2021
 ============================================================================
 */

/*****************GUARDING *****************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
/*****************STATIC CONFIGURATIONS *****************/
#define REGISTER_SELECT_RS  PD4
#define READ_OR_WRITE       PD5
#define ENABLE_LCD          PD6
#define PORT_DIRECTION_CTRL DDRD
#define PORT_DIRECTION_DATA DDRC
#define CONTROL_PORT_LCD    PORTD
#define DATA_PORT_LCD       PORTC
#define LCD_8_BIT_MODE_WITH_2_LINES 0x38
#define CLEAR_SCREEN_COMMAND        0x01
#define CURSOR_OFF                  0x0C
#define CURSOR_ON                   0x0E
#define CURSOR_BLINK                 0x0F
#define CURSOR_LOCATION             0x0E
#define FORCE_CURSOR_TO_FIRST_OF_LINE 0x80
#define ROW_1_LCD 1
#define COLUMN_0_LCD 0
/*****************FUNCTIONS PROTOTYPE *****************/
void LCD_clearScreen(void);
void LCD_init();
void LCD_sendCommand (uint8 command);
void LCD_displayCharacter(uint8 ch_ASCII);
void LCD_displayString(const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_intgerToStringDisplay(int data);
void LCD_clearScreen();
#endif
