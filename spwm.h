

#ifndef _SPWM_H_
#define _SPWM_H_

#include "stm32g431xx.h"



#define  SPWM_INTERRUPT_RATE    10000
#define  SPWM_CARRIER_FREQ_HZ   30000
#define  SPWM_POWER_DIV_FACT    4
#define  SPWM_SVPWM_STEP_SIZE   1



void     SPWM_GPIO_Init(void);
void     SPWM_Timer_Init(void);
void     SPWM_Sine_Table_Init(void);
void     SPWM_Set_Val(uint16_t u, uint16_t v);
void     SPWM_Update(void);

#endif


