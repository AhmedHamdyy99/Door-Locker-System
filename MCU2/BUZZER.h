/*
 ============================================================================
 Name        : Final Project Door-locking system
 Author      : Ahmed Hamdy
 Description : Header file for the BUZZER
 Date        : 21/7/2021
 ============================================================================
 */


/*****************GUARDING *****************/
#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define REQUIRED_NUMBER_OF_ITERATIONS_PER_60_SEC 400

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void BuzzerON(void);
void BuzzerOFF(void);
#endif /* EXTERNAL_EEPROM_H_ */
