/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Nour Mahmoud
 Description : Header file for the Timer0
 Date        : 21/7/2021
 ============================================================================
 */


/*****************GUARDING *****************/
#ifndef TIMER_0_H_
#define TIMER_0_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         STATIC CONFIGURATIONS                               *
 *******************************************************************************/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_NO_CLOCK 31*/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK 31250*/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK_8 3906*/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK_64 488*/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK_256 122*/
#define NUMBER_OF_OVERFLOWS_PER_SECOND_F_CPU_CLOCK_1024 31
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_EXT_CLOCK_T0_FALLING 80*/
/*#define NUMBER_OF_OVERFLOWS_PER_SECOND_EXT_CLOCK_T0_RISING 13*/
#define OVERFLOWMODE 0
#define CTCMODE 8
#define TOOGLEOC0 16
#define CLEAROC0  32
#define SETOC0  48


extern uint8 volatile TIMER_0_OVF_int_software_flag;
/*extern uint8 volatile TIMER_0_COMP_int_software_flag;*/

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	/*define the prescaler */
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_T0_FALLING,EXTERNAL_T0_RISING
}Timer0_Clock_select;

typedef enum
{

	NORMALMODE = OVERFLOWMODE,COMPAREMODE = CTCMODE
}Timer0_MODE;
typedef enum
{
	NORMAL = OVERFLOWMODE //TOGGOLE = TOOGLEOC0,CLEAR = CLEAROC0,SET = SETOC0
}COMPARE_MATCH_MODE;

typedef struct
{
	Timer0_MODE         mode;
	Timer0_Clock_select clock;
	uint8      TCNT0value;
	COMPARE_MATCH_MODE  OC0mode;
	uint8       CTCvalue;

}Timer0_config_type;

/*******************************************************************************
 *                         FUNCTIONS DEFINTIONS                                  *
 *******************************************************************************/
void Timer0_clock_init(const Timer0_config_type* Config_Ptr);
void Timer0_deinit(void);
#endif
