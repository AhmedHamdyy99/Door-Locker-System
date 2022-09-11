/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : THE USER INTERFACE MCU
 Date        : 27/7/2021
 ============================================================================
 */
/********************************THE USER INTERFACE MCU********************************/
#include "Keypad.h"
#include "LCD.h"
#include"USART.h"
#define ArraySize 5

uint8 password[ArraySize];
uint8 passwordconfirmation[ArraySize];
uint8 string[17];
uint8 key;
uint8 key2;
uint8 BUZZ=0;
/*BUZZ acts as a flag for the buzzer*/

void password_register(void);
void confirm_pass(void);
void main_menu(void);
void open_door(void);
void delay_in_ms_x_100(uint8 x);


int main()
{
	uint8 changePass=0,confirmPass=0,mainMenu=0,action=0,buzzer=1;
	/*finite states that the shall be interacting with each other depending on the USART
	 * commands*/

	LCD_init(); /*init the LCD*/

	USART_config_type USART_CONFIG = {_8_bits,disabled,_1_bit,9600};
	UART_init(&USART_CONFIG);
	/*init the USART using the config structure*/

	password_register();

	confirmPass=0;
	changePass=1;
	mainMenu=1;
	/*After the User enters a new password
	 * he must confirm it to navigate throughout the system freely
	 * so he move to the confirmPass
	 */

	while(1)
	{
		if(confirmPass==0)
		{
			/*The confirmPass is the state in which the user is asked for the password
			 * that is saved in the eeprom
			 * he must confirm it to navigate throughout the system freely
			 * if he entered the wrong pass for 3 times it moves to the BUZZER State
			 * if he entered it correct he move to the next phase the main menu phase
			 * depending on the user input the USART sends commands to the CONTROL MCU
			 * to send the correct actions needed
			 */
			confirm_pass();
			if(BUZZ==3)
			{
				UART_sendByte(BUZZER_ON); /*send the BUZZER on command to the CONTROL MCU to notify it that the user entered the wrong password 3 times*/
				buzzer=0;
				confirmPass=1;
				changePass=1;
				mainMenu=1;
				/*Move to the buzzer state*/

				BUZZ=0;
			}
			else{
				UART_sendByte(BUZZER_OFF);/*send the BUZZER off command to the CONTROL MCU to notify it that the user entered the the corrected password*/

				confirmPass=1;
				changePass=1;
				mainMenu=0;
				/*Move to the main menu*/
			}
		}
		else if(changePass==0)
		{
			/*if the user wanted to
			 * change his old password in the storage
			 * he must enter the two correct passwords for such an action to occur */
			password_register();
			changePass=1;
			confirmPass=0;
			mainMenu=1;
			/*move to confirmPass state*/
		}

		else if (mainMenu==0){
			/*The main navigation menu of the system
			 * its has two options
			 * + change password
			 * - open door
			 * depending on the user inputs its sends the proper command to the CONTROL MCU to make the needed actions*/

			main_menu();

			if(key2==CHANGE_PASSWORD_KEY)
			{
				/*The user here entered (+)
				 * which makes the UI MCU sends the USART commands to the CONTROL MCU to take the proper action
				 * but he must enter the older password saved in the eeporm for safety
				 * if he entered the wrong pass for 3 times it moves to the BUZZER State
				 * if he entered it correct he move to the next phase the main menu phase
				 * depending on the user input the USART sends action commands to the CONTROL MCU
				 * to take the correct neededed approach
				 * move to change pass phase*/
				action=1; /*if action =1 then its the change password key */
				delay_in_ms_x_100(250);
				UART_sendByte(action); /*send the action to CONTROL MCU*/
				confirm_pass();
				/*MUST CONFIRM OLD PASSWORD FOR SAFETY*/
				if(BUZZ==3)
				{
					UART_sendByte(BUZZER_ON);/*send the BUZZER on command to the CONTROL MCU to notify it that the user entered the wrong password 3 times*/
					buzzer=0;
					confirmPass=1;
					changePass=1;
					mainMenu=1;
					/*Move to buzzer state*/
					BUZZ=0;
				}
				else{
					UART_sendByte(BUZZER_OFF);/*send the BUZZER off command to the CONTROL MCU to notify it that the user entered the the corrected password*/
					while(UART_recieveByte()!=CPASS);{}// wait for the CONTROL MCU to send the change PASS command}
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,0,"You choose + ");
					delay_in_ms_x_100(250);

					changePass=0;
					confirmPass=1;
					mainMenu=1;
					/*After the user changed his password he must confirm it
					 * to navigate through the system again
					 * so back to confirmPass state*/
				}
				action=0;
			}
			else if(key2==OPEN_DOOR_KEY)
			{
				/*the key entered here is (-)
				 * so the user wantes to open the door
				 * which makes the UI MCU sends the USART commands to the CONTROL MCU to take the proper action
				 * but he must enter the older password saved in the eeporm for safety
				 * if he entered the wrong pass for 3 times it moves to the BUZZER State
				 * if he entered it correct he move to the next phase the main menu phase
				 * depending on the user input the USART sends action commands to the CONTROL MCU
				 * to take the correct neededed approach* */
				action=2; /*if action =2 then its the open door key */
				delay_in_ms_x_100(250);
				UART_sendByte(action);
				confirm_pass();
				/*MUST CONFIRM OLD PASSWORD FOR SAFETY*/
				if(BUZZ==3)
				{
					UART_sendByte(BUZZER_ON);/*send the BUZZER on command to the CONTROL MCU to notify it that the user entered the wrong password 3 times*/
					buzzer=0;
					confirmPass=1;
					changePass=1;
					mainMenu=1;
					/*Move to buzzer state*/
					BUZZ=0;
				}
				else{
					/*the user enter the corrected password so
					 * open door action will happen
					 * then back to main menu*/
					UART_sendByte(BUZZER_OFF); /*send the BUZZER off command to the CONTROL MCU to notify it that the user entered the the corrected password*/

					open_door();

				}
				action=0;
			}
		}
		else if(buzzer==0)
		{
			/*THE BUZZER STATE
			 * its the state when the user forget the password he entered
			 * or a theif tries to steal
			 * so it send a command to the CONTROL MCU to make the correct action*/
			UART_sendByte(BUZZER_ON);/*send the BUZZER on command to the CONTROL MCU to notify it that the user entered the wrong password 3 times*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"ERROR");
			while(UART_recieveByte()!=BUZZER_OFF){}/*waiting till the CONTROL MCU CLOSES THE BUZZER*/
			/*back to main menu */
			changePass=1;
			confirmPass=1;
			mainMenu=0;
			buzzer=1;

		}
	}
}
void delay_in_ms_x_100(uint8 x)
{
	/*a delay function
	 * since the _delay_ms had some errors with me after values
	 * of 32 ms in some parts of the code*/
	uint8 i;
	x=x*100;

	for (i=0; i<x; i++) _delay_ms(10);
}
void password_register(void)
{
	/*A function that takes two passwords from the user
	 * then sends it to the CONTROL MCU
	 * and wait for its confirmation
	 * if the two passwords are correct then its goes on
	 * if the two passwords are wrong it waits till the user enter the two new passwords correct*/
	uint8 i=0,counter=0;
	UART_sendByte(NEW_PASS); /*send the New Pass command to the CONTROL MCU to notify it that the user wants to enter a new pass*/
	while(1)
	{
		/*
		 * enter the first password*/

		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"New Password");
		LCD_goToRowColumn(1,0);
		for(i=0;i<ArraySize;i++)
		{
			key = KeyPad_getPressedKey(); /* get the pressed key number */
			if((key <= 9) && (key >= 0))
			{
				password[i]=key;             /*store in an array*/
				LCD_displayCharacter('*'); /*Display * on screen while user enters pass*/
				UART_sendByte(password[i]); /*send the password to the CONTROL MCU*/
				delay_in_ms_x_100(250);
			}
			delay_in_ms_x_100(5);

		}
		delay_in_ms_x_100(10);

		/*
		 * enter the second password*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Confirm Password");
		LCD_goToRowColumn(1,0);
		for(i=0;i<ArraySize;i++)
		{
			key2 = KeyPad_getPressedKey(); /* get the pressed key number */

			if((key2 <= 9) && (key2 >= 0))
			{passwordconfirmation[i]=key2;/*store in an array*/
			LCD_displayCharacter('*'); /*Display * on screen while user enters pass*/
			UART_sendByte(passwordconfirmation[i]);/*send the password to the CONTROL MCU*/
			delay_in_ms_x_100(250);
			}
			delay_in_ms_x_100(5);

		}
		delay_in_ms_x_100(10);



		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"C.H.E.C.K.I.N.G");
		delay_in_ms_x_100(100);

		UART_sendByte(IS_PASS_EQUAL); /*sends is pass equal to the CONTROL MCU to know if the two pass are correct*/
		counter =UART_recieveByte();  /*recieve the result of the the two compared passwords*/
		if (counter == PASS_CONFIRM)
		{
			/*if its the correct two passwords
			 * then the users move on to the next phase*/
			LCD_clearScreen();
			UART_sendByte(PASS_RE_CONFIRM);
			UART_receivestring(string); /*receive the string*/
			LCD_displayString(string); /* display the string on LCD*/
			_delay_ms(3000);
			counter=0;
			break;
		}
		else
		{
			/*if its the wrong two passwords
			 * then the users has  to the be in same phase
			 * till he enters the correct password*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Try again");
			LCD_displayStringRowColumn(1,0,"No match");
			delay_in_ms_x_100(200);
			UART_sendByte(WRONG);
		}
	}
}
void confirm_pass(void)
{
	/*Function that takes the entered password from the user
	 * for safety and the free navigation inside the main menu of the system
	 * if he enter a password its sent to control mcu then
	 * waits for confirmation*/
	uint8 i=0,state=0;
	UART_sendByte(ENTER_PASS); /*sends is enter passeto the CONTROL MCU to tell it to recieve the upcoming password*/
	while(1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter Password");
		LCD_goToRowColumn(1,0);
		for(i=0;i<ArraySize;i++)
		{
			key = KeyPad_getPressedKey(); /* get the pressed key number */
			if((key <= 9) && (key >= 0))
			{
				password[i]=key; /*store in array*/
				LCD_displayCharacter('*'); /*display * on Screen */
				UART_sendByte(password[i]); /*send the password via USART*/
				delay_in_ms_x_100(250);
			}
			delay_in_ms_x_100(5);
		}
		delay_in_ms_x_100(10);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"CHECKING");
		delay_in_ms_x_100(250);
		//	_delay_ms(2500);

		state =UART_recieveByte(); /*check for the state of the USART*/
		if (state == PASS_CONFIRM)
		{
			/*the user has entered the correct password*/
			LCD_clearScreen();
			UART_sendByte(PASS_RE_CONFIRM); /*further reconfirmation*/
			UART_receivestring(string); /* receive the string*/
			LCD_displayString(string); /* display the string on LCD */
			_delay_ms(3000);
			state=0;
			BUZZ=0;
			break;
		}
		else
		{
			/*the user entered the wrong pass
			 * check if the user entered 3 times the wrong password
			 * then it triggers a return from the function with a global variable value saved in it which
			 * tracks the buzzer state in the main function */
			BUZZ++;
			if(BUZZ==3)
			{
				return;
			}
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Try again");
			LCD_displayStringRowColumn(1,0,"No match");
			delay_in_ms_x_100(20);
			//		_delay_ms(2000);
		}
	}
}
void main_menu(void)
{
	/*Function for the navigation of the menu */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ Change Pass");
	LCD_displayStringRowColumn(1,0,"- Open Door");
	key2 = KeyPad_getPressedKey(); /* get the pressed key number */
	delay_in_ms_x_100(5);
}
void open_door(void)
{
	/*function that after the user choose the
	 * open door action
	 * it sends and receives via uart with control mcu
	 * to synchronize the values on the screen with acutal timing of the motor rotation
	 * */

	while(UART_recieveByte()!=ODOOR);{}// MC2 is ready}

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"You choose - ");
	_delay_ms(2000);
	UART_sendByte(DOOR_LOCKING); /* DOOR LOCKING*/
	UART_receivestring(string); /* receive the string*/
	LCD_clearScreen();
	LCD_displayString(string); /*display the string on LCD*/
	UART_sendByte(DOOR_STOP); /*DOOR STOPPED*/
	UART_receivestring(string); /* receive the string*/
	LCD_clearScreen();
	LCD_displayString(string); /*display the string on LCD*/
	UART_sendByte(DOOR_UNLOCKING); /* DOOR UNLOCKING*/
	UART_receivestring(string); /* receive the string */
	LCD_clearScreen();
	LCD_displayString(string); /* display the string on LCD*/
	while(UART_recieveByte() != DOOR_OFF){} /* wait until MC1 is ready*/
	UART_sendByte(DOOR_OFF); /*Door is off */
	UART_receivestring(string); /*receive the string*/
	LCD_clearScreen();
	LCD_displayString(string); /* display the string on LCD*/

	LCD_displayStringRowColumn(1,0,"Back to menu");
	_delay_ms(2000);
}
