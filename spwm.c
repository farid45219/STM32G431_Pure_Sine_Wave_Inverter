

#include "stm32g431xx.h"
#include "cdefs.h"
#include "spwm.h"
#include "math.h"

typedef struct swpm_t{
	volatile uint16_t SineTable[361];
	volatile uint16_t Output1;
	volatile uint16_t Output2;
	volatile uint16_t Angle;
}spwm_t;


spwm_t SPWM;


void SPWM_Struct_Init(void){
	SPWM.Angle = 0;
	SPWM.Output1 = 0;
	SPWM.Output2 = 0;
	for (uint16_t i = 0; i < 360; i++){
		SPWM.SineTable[i] = 0;
	}
}


void SPWM_GPIO_Init(void){
	//Enable clock for respective PORTS
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//Config OUT1H->PA8->TIM1_CH1->AFSEL8_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD8;
	GPIOA->MODER &=~GPIO_MODER_MODE8;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL8_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL8_Pos;
	
	//Config OUT1L->PC13->TIM1_CH1N->AFSEL13_AF4
	GPIOC->ODR   &=~GPIO_ODR_OD13;
	GPIOC->MODER &=~GPIO_MODER_MODE13;
	GPIOC->AFR[1]&=~GPIO_AFRH_AFSEL13_Msk;
	GPIOC->AFR[1]|= 4 << GPIO_AFRH_AFSEL13_Pos;
	
	//Config OUT2H->PA9->TIM1_CH2->AFSEL9_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD9;
	GPIOA->MODER &=~GPIO_MODER_MODE9;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL9_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL9_Pos;
	
	//Config OUT2L>PA12->TIM1_CH2N->AFSEL12_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD12;
	GPIOA->MODER &=~GPIO_MODER_MODE12;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL12_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL12_Pos;
}


void SPWM_Timer_Init(void){
	//Config Timer to generate PWM
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->PSC     = 0;
	TIM1->ARR     = 16000000/SPWM_CARRIER_FREQ_HZ;
	TIM1->CR1    |= TIM_CR1_ARPE;
	
	//Config TIM1_CH1
	TIM1->CCR1    = 0;
	TIM1->CCMR1  &=~TIM_CCMR1_OC1M;
	TIM1->CCMR1  |= (6 << TIM_CCMR1_OC1M_Pos);
	TIM1->CCMR1  |= TIM_CCMR1_OC1PE;
	
	//Config TIM1_CH2
	TIM1->CCR2    = 0;
	TIM1->CCMR1  &=~TIM_CCMR1_OC2M;
	TIM1->CCMR1  |= (6 << TIM_CCMR1_OC2M_Pos);
	TIM1->CCMR1  |= TIM_CCMR1_OC2PE;
	
	//Config complementary channels
	TIM1->CCER   |= TIM_CCER_CC1E | TIM_CCER_CC1NE;
	TIM1->CCER   |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
  
	//Config dead time
	TIM1->BDTR   |= (100 << TIM_BDTR_DTG_Pos);
	TIM1->BDTR   |= TIM_BDTR_MOE;
	
	//Enable counter
	TIM1->CR1    |= TIM_CR1_CEN;
	
	//Config UH->PA8->TIM1_CH1->AFSEL8_AF6
  GPIOA->MODER |= GPIO_MODER_MODE8_1;
	
	//Config UL->PC13->TIM1_CH1N->AFSEL13_AF4
  GPIOC->MODER |= GPIO_MODER_MODE13_1;
	
	//Config VH->PA9->TIM1_CH2->AFSEL9_AF6
  GPIOA->MODER |= GPIO_MODER_MODE9_1;
	
	//Config VL->PA12->TIM1_CH2N->AFSEL12_AF6
  GPIOA->MODER |= GPIO_MODER_MODE12_1;
	
}


void SPWM_Sine_Table_Init(void){
	uint16_t arr_max;
	
	//clear whole table
	for(int i = 0; i <= 360; i++){
    SPWM.SineTable[i] = 0;
  }
	
	//assign sine table values
	for(int i = 0; i <= 360; i++){
    float s = sinf(i * 3.14159f / 180.0f);
		arr_max = TIM1->ARR - 20;
		arr_max /= SPWM_POWER_DIV_FACT;
    SPWM.SineTable[i] = (uint16_t)(s * arr_max);
  }
	
}


void SPWM_Set_Val(uint16_t out1, uint16_t out2){
	TIM1->CCR1 = SPWM.SineTable[out1];
	TIM1->CCR2 = SPWM.SineTable[out2];
}


void SPWM_Update(void){
	
	if(SPWM.Angle <= 179){
		//disable v pwm
		//set u val
		//enable u pwm
	}
	else{
		//disable u pwm
		//set v val
		//enable v pwm
	}
	
	SPWM.Angle += SPWM_SVPWM_STEP_SIZE;
	if(SPWM.Angle >= 360){
		SPWM.Angle = 0;
	}
	
	
	SPWM_Set_Val(SPWM.Output1, SPWM.Output2);
}


