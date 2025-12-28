

#ifndef _SPWM_H_
#define _SPWM_H_

#include "stm32g431xx.h"





#define  SPWM_INTERRUPT_RATE         10000
#define  SPWM_CARRIER_FREQ_HZ        20000
#define  SPWM_PHASE_SW_CYCLE         10
#define  SPWM_POWER_DIV_FACT         1
#define  SPWM_SVPWM_STEP_SIZE        1
#define  SPWM_SOFT_START_STEP_SIZE   1




void     SPWM_Struct_Init(void);
void     SPWM_GPIO_Init(void);
void     SPWM_PWM_Timer_Init(void);
void     SPWM_Sequencer_Timer_Init(uint32_t UpdateRateHz);
void     SPWM_Sine_Table_Init(void);

void     SPWM_Output1H_General_Purpose_Output_Low(void);
void     SPWM_Output1L_General_Purpose_Output_Low(void);
void     SPWM_Output2H_General_Purpose_Output_Low(void);
void     SPWM_Output2L_General_Purpose_Output_Low(void);
void     SPWM_Output1H_General_Purpose_Output_High(void);
void     SPWM_Output1L_General_Purpose_Output_High(void);
void     SPWM_Output2H_General_Purpose_Output_High(void);
void     SPWM_Output2L_General_Purpose_Output_High(void);
void     SPWM_Output1H_Alternate_Function(void);
void     SPWM_Output1L_Alternate_Function(void);
void     SPWM_Output2H_Alternate_Function(void);
void     SPWM_Output2L_Alternate_Function(void);
void     SPWM_All_Output_Disable(void);

void     SPWM_Set_Val(uint16_t out1, uint16_t out2);
void     SPWM_Update(void);
void     TIM4_IRQHandler(void);

void     SPWM_Init(void);

#endif


