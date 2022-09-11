/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the BUZZER
 Date        : 21/7/2021
 ============================================================================
 */

#include "BUZZER.h"
#include "timer_0.h"

void BuzzerON(void)
{
	SET_BIT(DDRD,PD6); //OUTPUT PIN
	uint16 i=0;

	while(1)
	{
		/*To get a delay of 60 sec
		 * (60000ms = T period * i )
		 * 60000 = 40 * i
		 * i=1500*/
		i++;
		_delay_ms(50);
		CLEAR_BIT(PORTD,PD6);
		_delay_ms(100);
		SET_BIT(PORTD,PD6);

		if(i==REQUIRED_NUMBER_OF_ITERATIONS_PER_60_SEC)
		{
			return;
		}
	}
}
void BuzzerOFF(void)
{
	CLEAR_BIT(PORTD,PD6);
	}
