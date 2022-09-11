/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Source file for the Timer0
 Date        : 21/7/2021
 ============================================================================
 */
#include"timer_0.h"


uint8 volatile TIMER_0_OVF_int_software_flag=0;
/*a global variable that when the ISR exectus it acts as a software flag for the
 * needed action later for the ISR
 * */
/*uint8 volatile TIMER_0_COMP_int_software_flag=0;*/

ISR(TIMER0_OVF_vect)
{
	TIMER_0_OVF_int_software_flag=1;
}
//ISR(TIMER0_COMP_vect)
//{
//	TIMER_0_COMP_int_software_flag=1;
//}
void Timer0_clock_init(const Timer0_config_type * Config_Ptr)
{
	/*Init function for timer 0 clock it takes a configuration structure and executes depending on it*/
	/*TCCR0 config*/
	SET_BIT(TCCR0,FOC0); //for non -pwm modes
	/*****************************/
	/*
	 * WAVEFORM GENERATION
	   NORMAL MODE  0
	WGM01 =0 AND WGM00 =0
	CTC MODE     2
	WGM01 =1 AND WGM00 =0*/
	TCCR0 = (TCCR0 & 0xB7) | (Config_Ptr->mode);


	/******************************/
	/*COMPARE MATCH OUTPUT MODE
	COM01 C0M00
	0   0 NORMAL  0C0 DISCONNECTED     == 0
	0   1 TOGGOLE 0C0 ON COMPARE MATCH == 16
	1   0 CLEAR   0C0 ON COMPARE       == 32
	1   1 SET     0C0 ON COMPARE MATCH == 48*/

	TCCR0 = (TCCR0 & 0xCF) | (Config_Ptr->OC0mode);
	/*****************************/
	/*PRESCALER*/
	/*CS02 CS01 CS00
	 * 0    0     0            NO CLOCK
	 * 0    0     1            NO PRESCALER
	 * 0    1     0            CLOCK/8
	 * 0    1     1            CLOCK/64
	 * 1    0     0            CLOCK/256
	 * 1    0     1            CLOCK/1024
	 * 1    1     0            EXT CLOCK ON T0 ON FALLING EDGES
	 * 1    1     1            EXT CLOCK ON T0 ON RISING  EDGES
	 * */
	/*CLOCK /1024 PRESCALER*/
	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);
	/******************************************/
	/*COUNTER REGISTER*/
	/*INITIALIZE TO ZERO*/
	TCNT0 = (TCNT0 & 0x00) | (Config_Ptr->TCNT0value);
	/******************************************/
	/*INTERRUPT MASK REGISTER*/
	/*TOIE0 FOR TIMER0 OVERFLOW INTTERUPT ENABLE*/
	if (Config_Ptr->mode ==0)
	{
		SET_BIT(TIMSK,TOIE0);
	}
	/*OCIE0 FOR TIMER0 OUPUT COMPARE INTTERUPT ENABLE*/
	else
	{
		OCR0 = (OCR0 & 0x00) | (Config_Ptr->CTCvalue); // Set Compare Value
		SET_BIT(TIMSK,OCIE0);
	}
	/* NORMAL MODE INTTERRUPT*/
	/*TIMER/COUNTER INTERRUPT FLAG REGISTER - TIFR */
	/*TOV0 BIT IN TIFR
	 * IS SET TO 1 WHEN OVER FLOW AND CLEARED BY HARDWARE WHEN EXEUCTING THE INTTERUPT*/
	/*OCF0 BIT IN TIFR*/
	/*OCF0 BIT IN TIFR
	 * IS SET TO 1 WHEN OUTPUT COMPARE AND CLEARED BY HARDWARE WHEN EXEUCTING THE INTTERUPT*/

}
void Timer0_deinit(void)
{
	/*deinit function for timer0*/
	TCCR0=0;
	TCNT0=0;
	OCR0=0;
	CLEAR_BIT(TIMSK,TOIE0);
	CLEAR_BIT(TIMSK,OCIE0);

}
