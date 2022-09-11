/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the USART
 Date        : 21/7/2021
 ============================================================================
 */


#include"USART.h"
static volatile uint16 BAUD_PRESCALER=0,BAUD_RATE=0;
/*****************FUNCTIONS DEFINTIONS *****************/
void UART_sendByte(uint8 data)
{
	/*pooling till the flag of the UDRE is set when UDR is empty and ready for new transmit*/
	while(BIT_IS_CLEAR(UCSRA,UDRE)){} /*if equal one --> transmitter register is empty*/
	UDR =data;
	/*USART I/O DATA REGISTER*/
}

uint8 UART_recieveByte(void)
{
	/*pooling till the flag of the RXC is set when UDR is empty and ready for new receiving*/
	while (BIT_IS_CLEAR(UCSRA,RXC))/*if equal one --> receiving register is empty*/
	{}
	return UDR;
	/*clear RXC flag and read data*/
}
void UART_sendString(uint8 *Str)
{
	/*Takes a string input and then send it via UART without its null character*/
	uint8 i =0;
	while(Str[i] != '\0')
	{
		/*Send the components of the string without the null character*/
		UART_sendByte(Str[i]);
		i++;
	}

}

void UART_receivestring(uint8 *Str)
{
	/*Takes a string input and then via it receive the string UART has transmittedr*/
	uint8 i =0;
	Str[i] =UART_recieveByte();
	/*creating an imaginary null character of my own at receiver ( $S )*/
	while(Str[i] != '$')
	{/*send every character of the string*/
		i++;
		Str[i]=UART_recieveByte();
	}
	Str[i] ='\0';
	/*return the null character to its original */

}
void UART_init(const USART_config_type * Config_Ptr)
{
	/*FRAME*/
	/*USCRA CONFIG*/
	SET_BIT(UCSRA,U2X);;  /*higher transmission speed*/
	/*UCSRB CONFIG*/
	CLEAR_BIT(UCSRB,RXCIE);  /*DISABLE INTTRUPTS*/
	CLEAR_BIT(UCSRB,TXCIE);  /*DISABLE INTTRUPTS*/
	CLEAR_BIT(UCSRB,UDRIE); /*DISABLE USART Data Register Empty Interrupt Enable*/
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN); /*ENABLE REVIECER AND TRANSMITTER*/
	/*USZ BIT SETTINGS 8 bits*/
	CLEAR_BIT(UCSRB,UCSZ2); /*8 BIT MODE*/
	/*UCSRC CONFIG*/
	SET_BIT(UCSRC,URSEL); /*Must be one when writing to USSRC*/
	CLEAR_BIT(UCSRC,UCPOL);
	CLEAR_BIT(UCSRC,UMSEL);/*ASYNCHORNUS MODE*/
	/*MODE WITH DISAPLED PARITY UMP1 UMP0 00*/
	UCSRC = (UCSRC & 0xCF) | (Config_Ptr-> paritybit);
	/*STOP BIT*/
	UCSRC = (UCSRC & 0xF7) | (Config_Ptr-> stopbit);
	/*USZ BIT SETTINGS*/
	UCSRC = (UCSRC & 0xF9) | (Config_Ptr-> bitdata);
	/*9600 bits /sec from BAUD RATE equation*/
	/*UBRR = (Freq (osc of processor) /8*BAUD ) -1 = 12  must be positive or zero*/
	BAUD_RATE=(Config_Ptr-> baudrate);
	BAUD_PRESCALER =(((1000000 / ( (Config_Ptr-> baudrate) * 8))) - 1);
	UBRRL= BAUD_PRESCALER;
	UBRRH =(BAUD_PRESCALER)>>8;
	/*SEPARATE HARDWARE REGISTERS*/
	/*URSEL 0 FOR NOT WRITING IN THE UBRRH=*/
}
