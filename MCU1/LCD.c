/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the LCD
 Date        : 29/3/2021
 ============================================================================
 */

#include"LCD.h"
/*****************FUNCTIONS DEFINTIONS *****************/
/*INIT function to initialize the LCD*/
void LCD_init(void)
{
	SET_BIT(PORT_DIRECTION_CTRL,REGISTER_SELECT_RS);
	SET_BIT(PORT_DIRECTION_CTRL,READ_OR_WRITE);
	SET_BIT(PORT_DIRECTION_CTRL,ENABLE_LCD);
	/*Set the first 3 bits of port D to output pins*/
	PORT_DIRECTION_DATA = 0xFF; /* Configure the data port c as output port */
	LCD_sendCommand(LCD_8_BIT_MODE_WITH_2_LINES); /*  2-line LCD  8-bit Data Mode with 5x7 dot display mode */
	LCD_sendCommand(CURSOR_BLINK); /* cursor off */
	LCD_clearScreen(); /* clear LCD */
}

/*Function to send command for the LCD */
void LCD_sendCommand( uint8 command )
{
	CLEAR_BIT(CONTROL_PORT_LCD ,REGISTER_SELECT_RS);    /*0---> COMMAND */
	CLEAR_BIT(CONTROL_PORT_LCD,READ_OR_WRITE);         /*0---> WRITE*/
	/*set RS to 0 to make Rs to select the command register and send instruction and to wrtie */
	_delay_ms(1);   /*delay for tas =  50 ns due to the timing diagram*/
	SET_BIT(CONTROL_PORT_LCD,ENABLE_LCD);
	/*Enable the LCD*/
	_delay_ms(1);   /*delay for tpw-tdsw=290-100 =190 ns due to the timing diagram*/
	DATA_PORT_LCD  = (DATA_PORT_LCD  & 0x00) |(  command   & 0xFF ) ;
	/*set the instruction in the port to command the LCD */
	_delay_ms(1);   /*delay for tdsw =  100 ns due to the timing diagram*/
	CLEAR_BIT(CONTROL_PORT_LCD,ENABLE_LCD);
	_delay_ms(1);   /*delay for  13 ns due to the timing diagram*/
}
/*Function to display a character for the LCD */
void LCD_displayCharacter(uint8 ch_ASCII)
{
	SET_BIT(CONTROL_PORT_LCD ,REGISTER_SELECT_RS);    /*1---> DATA */
	CLEAR_BIT(CONTROL_PORT_LCD,READ_OR_WRITE);         /*0---> write*/
	/*set RES to 1 to make Rs to select the data register and send instruction and to write data */
	_delay_ms(1);   /*delay for tas =  50 ns due to the timing diagram*/
	SET_BIT(CONTROL_PORT_LCD,ENABLE_LCD);
	/*Enable the LCD*/
	_delay_ms(1);   /*delay for tpw-tdsw=290-100 =190 ns due to the timing diagram*/
	DATA_PORT_LCD  = (DATA_PORT_LCD  & 0x00) |(  ch_ASCII  & 0xFF ) ;
	/*send data to the port to display the LCD */
	_delay_ms(1);   /*delay for tdsw =  100 ns due to the timing diagram*/
	CLEAR_BIT(CONTROL_PORT_LCD,ENABLE_LCD);
	_delay_ms(1);   /*delay 13 ns due to the timing diagram*/
}
/*Function to display a string for the LCD */
void LCD_displayString(const char *Str)
{
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}
	/*while the value that the pointers point to is not equal to NULL display it on the screen*/
}
/*Function to go to a certain row and column */
void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	/* first of all calculate the required address */
	switch(row)
	{
	case 0:
		Address=col;
		break;
	case 1:
		Address=col+0x40;
		break;
	case 2:
		Address=col+0x10;
		break;
	case 3:
		Address=col+0x50;
		break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | FORCE_CURSOR_TO_FIRST_OF_LINE );
}
/*Function to display a certain row and column */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}
/*Function to convert from int to string*/
void LCD_intgerToStringDisplay(int data)
{
	char stringDATA[16]; /* String to hold the ascii result */
	itoa(data,stringDATA,10); /* 10 for decimal */
	LCD_displayString(stringDATA);
}
/*Function to clear the screen*/
void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_SCREEN_COMMAND); /*clear display*/
}
