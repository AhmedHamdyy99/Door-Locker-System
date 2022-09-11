/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : THE CONTOL MCU
 Date        : 27/7/2021
 ============================================================================
 */
/********************************THE CONTOL MCU********************************/
#include "EEPROM.h"
#include"USART.h"
#include"timer_0.h"
#include"motor.h"
#include"BUZZER.h"
#define ArraySize 5

uint8 password[ArraySize] , passwordconfirmation[ArraySize];
uint8 passwordRead[ArraySize];
uint8 BUZZ=0;

void delay_in_ms_x_100(uint8 x);
uint8 passwordCompare(uint8 password[ArraySize],uint8 passwordconfirmation[ArraySize]);
void password_register(void);
void confirm_pass(void);
void open_door(void);

int main()
{
	uint8 changePass=0,confirmPass=0,mainMenu=0,action=0,buzzer=1,f=0;
	/*finite states that the shall be interacting with each other depending on the USART
	 * commands*/
	SREG  |= (1<<7);           // Enable global interrupts in MC.
	Timer0_config_type Timer0_config = {NORMALMODE,F_CPU_1024,0,NORMAL};/*Timer0 init*/

	EEPROM_init(); /*EEPROM init*/
	USART_config_type USART_CONFIG = {_8_bits,disabled,_1_bit,9600};
	UART_init(&USART_CONFIG);
	/*USART init*/

	password_register();
	changePass=1;
	confirmPass=0;
	mainMenu=1;
	/*After the User enters a new password
	 * he must confirm it to navigate throughout the system freely
	 * the password is then compared
	 * if the two passwordes are correct
	 * the user moves to the confirmPass
	 * if the two passwords are incorrect
	 */



	while(1)
	{
		if (confirmPass==0)
		{
			/*The confirmPass is the state in which the user is asked for the password
			 * that is saved in the eeprom
			 * he must confirm it to navigate throughout the system freely
			 * if he entered the wrong pass for 3 times it moves to the BUZZER State
			 * if he entered it correct he move to the next phase the main menu phase
			 * thus sending the correct actions needed
			 */
			confirm_pass();
			f=UART_recieveByte(); /*recieve the flag of the BUZZER*/
			if(f==BUZZER_ON)
			{
				/*Move to the buzzer state*/
				buzzer=0;
				confirmPass=1;
				changePass=1;
				mainMenu=1;


				BUZZ=0;
			}
			else{
				/*Move to main menu*/
				confirmPass=1;
				changePass=1;
				mainMenu=0;
				buzzer=1;

			}
			f=0;

		}

		else if(changePass==0){
			/*if the user wanted to
			 * change his old password in the storage
			 * he must enter the two correct passwords for such an action to occur */
			password_register();

			changePass=1;
			confirmPass=0;
			mainMenu=1;
			/*move to confirmpass state*/

		}

		else if(mainMenu==0)
		{
			/*The main navigation menu of the system
			 * its has two options
			 * + change password
			 * - open door
			 * depending on the user inputs its sends the proper command to the CONTROL MCU to make the needed actions*/
			action=UART_recieveByte();
			if(action==1)
			{
				/*confirm pass again for safety*/
				confirm_pass();
				f=UART_recieveByte(); /*recieve the flag of the BUZZER*/
				if(f==BUZZER_ON)
				{
					/*Move to the buzzer state*/

					buzzer=0;
					confirmPass=1;
					changePass=1;
					mainMenu=1;


					BUZZ=0;
				}
				else{
					/*move to change password*/
					UART_sendByte(CPASS);
					changePass=0;
					confirmPass=1;
					mainMenu=1;

				}
				f=0;
				action=0;

			}
			else if(action==2)
			{
				confirm_pass();
				f=UART_recieveByte();/*recieve the flag of the BUZZER*/
				if(f==BUZZER_ON)
				{
					/*Move to the buzzer state*/
					buzzer=0;
					confirmPass=1;
					changePass=1;
					mainMenu=1;


					BUZZ=0;
				}
				else{
					/*INit the timer
					 * and call the open door function to open door
					 * then move to mainmenu*/
					Timer0_clock_init(&Timer0_config);
					open_door();
					mainMenu=0;
					changePass=1;
					confirmPass=1;

				}
				f=0;
				action=0;
			}
		}
		else if(buzzer==0)
		{
			/*THE BUZZER STATE
			 * its the state when the user forget the password he entered
			 * or a theif tries to steal
			 * so it send a command to the CONTROL MCU to make the correct action*/
			while(UART_recieveByte()!=BUZZER_ON){}/*send the BUZZER on command to the CONTROL MCU to notify it that the user entered the wrong password 3 times*/
			BuzzerON(); /*calls the function of the buzzer to work */
			UART_sendByte(BUZZER_OFF); /*send command to off the buzzers*/
			BuzzerOFF(); /*Shut-Down for buzzer*/
			mainMenu=0;
			changePass=1;
			confirmPass=1;
			buzzer=1;
			/*back to main menu*/

		}
	}

}

void delay_in_ms_x_100(uint8 x)
{
	uint8 i;
	x=x*10;
	for (i=0; i<x; i++) _delay_ms(10);
}
uint8 passwordCompare(uint8 password[ArraySize],uint8 passwordconfirmation[ArraySize])

{
	/*A function that copmapre to two entered passwords
	 * and check for their correctness*/
	uint8 i=0;
	for (i = 0; i < ArraySize; i++)
	{
		if (password[i] != passwordconfirmation[i])
		{
			return 0; /*two pass not equal*/
		}
	}
	return 1; /*two pass equal*/
}
void password_register(void)
{
	uint8 h=0;
	uint8 counter=0;
	uint8 pass=0;
	while(UART_recieveByte() !=NEW_PASS){} /*wait for the UI MCU to send the NEW_PASS command to tchange the password with a new entered one*/
	while(1){
		/*takes the password 1*/

		if(h==1)
		{
			counter=UART_recieveByte();
			if(counter==WRONG)
			{
				for(int i=0;i<ArraySize;i++)
				{
					password[i]=UART_recieveByte();
				}
				for(int i=0;i<ArraySize;i++)
				{
					passwordconfirmation[i]=UART_recieveByte();
				}
			}

		}
		if(h==0){
			for(int i=0;i<ArraySize;i++)
			{
				password[i]=UART_recieveByte();
			}
			for(int i=0;i<ArraySize;i++)
			{
				passwordconfirmation[i]=UART_recieveByte();
			}
			h=1;
		}
		while(UART_recieveByte() !=IS_PASS_EQUAL){} /*wait for the UI MCU to send the iS pass eqaul ? command to tchange the password with a new entered one*/
		pass=passwordCompare(password,passwordconfirmation);

		if(pass==1)
		{
			/*if the two passwords are confirmed
			 * then write in the EEPROM
			 * and leave function*/
			UART_sendByte(PASS_CONFIRM);
			while(UART_recieveByte() != PASS_RE_CONFIRM){}
			UART_sendString("Confirmed$");
			for(int i=0;i<ArraySize;i++)
			{

				EEPROM_writeByte(((0x0000)|(i)),passwordconfirmation[i]);
				_delay_ms(100);
			}
			h=0;
			counter=0;
			pass=0;
			break;

		}
		else
		{
			/*always here till the user enters the coreect password*/
			UART_sendByte(PASS_NOT_CONFIRM);
		}
	}
}
void confirm_pass(void)
{
	/*Function that takes the entered password from the user
	 * for safety and the free navigation inside the main menu of the system
	 * it compares the value of the password to the stored eeporm value and from it
	 * decide what to do next */
	uint8 i=0,pass=0;
	while(UART_recieveByte() != ENTER_PASS){} /*wait for the new coommand to be snet from the UI MCU*/
	while(1)
	{
		/*takes the first password*/
		for(i=0;i<ArraySize;i++)
		{
			password[i]=UART_recieveByte();
		}
		for(int i=0;i<ArraySize;i++)
		{
			/*read the EEPROM and store in a readpassword*/

			EEPROM_readByte(((0x0000)|(i)),&passwordRead[i]);
			_delay_ms(100);
		}
		pass=passwordCompare(password,passwordRead);
		if(pass==1)
		{/*if the two passwords are confirmed
		 * then write in the EEPROM
		 * and leave function*/
			UART_sendByte(PASS_CONFIRM);
			while(UART_recieveByte() != PASS_RE_CONFIRM){}
			UART_sendString("Pass Confirm$");
			pass=0;
			BUZZ=0;
			return;
		}
		else
		{
/*always here till the buzz variable = 3*/
			UART_sendByte(PASS_NOT_CONFIRM);
			BUZZ++;
			if(BUZZ==3)
			{
				return;
			}

		}
	}
}
void open_door(void)
{/*
fucnction that enables the motor to move depending on its interrupts*/
	UART_sendByte(ODOOR); /*send a byte to the UI to synchornize with the motor */
	Motor_OFF=0;

	while(Motor_OFF==0)
	{
		if(TIMER_0_OVF_int_software_flag==1)
		{
			doorAction();
			TIMER_0_OVF_int_software_flag=0;
		}
	}
}
