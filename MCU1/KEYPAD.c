/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the KEYPAD
 Date        : 29/3/2021
 ============================================================================
 */
#include "KEYPAD.h"

 unsigned char key;
/******************************Static Functions Prototypes******************************************/
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_Num);
/****************************** Functions Definitions ********************************/
uint8 KeyPad_getPressedKey(void)
{
	uint8 col,row;
	while(1)
	{
		for(col=0;col<N_col;col++) /* 1st loop on columns */
		{
			/*
			 1 output at a time and the rest of cols are inputs
			 */
			KEYPAD_PORT_DIR = (0b00010000<<col);
			/*
			 PortA pullup resistors
			 */
			KEYPAD_OUT_PORT = (~(0b00010000<<col));
			for(row=0;row<N_row;row++) /* nested 2nd loop on rows */
			{
				/*Check if the bit is cleared to konw if the switch is press in this row*/
				if(BIT_IS_CLEAR(KEYPAD_IN_PORT,row))
				{
						return KeyPad_4x4_adjustKeyNumber((row*N_col)+col+1);
				}
			}
		}
	}
}

/******************************Static Functions declaration******************************************/
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_Num)
{
	switch(button_Num)
	{
		case 1: return 7;
				break;
		case 2: return 8;
				break;
		case 3: return 9;
				break;
		case 4: return '%'; /* ASCII Code of % */
				break;
		case 5: return 4;
				break;
		case 6: return 5;
				break;
		case 7: return 6;
				break;
		case 8: return '*'; /* ASCII Code of '*' */
				break;
		case 9: return 1;
				break;
		case 10: return 2;
				break;
		case 11: return 3;
				break;
		case 12: return '-'; /* ASCII Code of '-' */
				break;
		case 13: return 13;  /* ASCII of Enter */
				break;
		case 14: return 0;
				break;
		case 15: return '='; /* ASCII Code of '=' */
				break;
		case 16: return '+'; /* ASCII Code of '+' */
				break;
		default: return button_Num;
	}
}

