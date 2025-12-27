

#include "stm32g431xx.h"
#include "cdefs.h"
#include "spwm.h"
#include "math.h"

volatile uint16_t spwm_sine_table[361], u, v, angle = 0;


void SPWM_GPIO_Init(void){
	
	//Enable clock for respective PORTS
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//Config UH->PA8->TIM1_CH1->AFSEL8_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD8;
	GPIOA->MODER &=~GPIO_MODER_MODE8;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL8_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL8_Pos;
	
	//Config UL->PC13->TIM1_CH1N->AFSEL13_AF4
	GPIOC->ODR   &=~GPIO_ODR_OD13;
	GPIOC->MODER &=~GPIO_MODER_MODE13;
	GPIOC->AFR[1]&=~GPIO_AFRH_AFSEL13_Msk;
	GPIOC->AFR[1]|= 4 << GPIO_AFRH_AFSEL13_Pos;
	
	//Config VH->PA9->TIM1_CH2->AFSEL9_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD9;
	GPIOA->MODER &=~GPIO_MODER_MODE9;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL9_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL9_Pos;
	
	//Config VL->PA12->TIM1_CH2N->AFSEL12_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD12;
	GPIOA->MODER &=~GPIO_MODER_MODE12;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL12_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL12_Pos;
	
	/*
	//Config WH->PA10->TIM1_CH3->AFSEL10_AF6
	GPIOA->ODR   &=~GPIO_ODR_OD10;
	GPIOA->MODER &=~GPIO_MODER_MODE10;
	GPIOA->AFR[1]&=~GPIO_AFRH_AFSEL10_Msk;
	GPIOA->AFR[1]|= 6 << GPIO_AFRH_AFSEL10_Pos;
	
	//Config WL->PB15->TIM1_CH3N->AFSEL15_AF4
	GPIOB->ODR   &=~GPIO_ODR_OD15;
	GPIOB->MODER &=~GPIO_MODER_MODE15;
	GPIOB->AFR[1]&=~GPIO_AFRH_AFSEL15_Msk;
	GPIOB->AFR[1]|= 4 << GPIO_AFRH_AFSEL15_Pos;
	*/
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
	
	
	/*
	//Config TIM1_CH3
	TIM1->CCR3    = 0;
	TIM1->CCMR2  &=~TIM_CCMR2_OC3M;
	TIM1->CCMR2  |= (6 << TIM_CCMR2_OC3M_Pos);
	TIM1->CCMR2  |= TIM_CCMR2_OC3PE;
	*/
	
	//Config complementary channels
	TIM1->CCER   |= TIM_CCER_CC1E | TIM_CCER_CC1NE;
	TIM1->CCER   |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
	//TIM1->CCER   |= TIM_CCER_CC3E | TIM_CCER_CC3NE;
  
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
	
	//Config WH->PA10->TIM1_CH3->AFSEL10_AF6
  //GPIOA->MODER |= GPIO_MODER_MODE10_1;
	
	//Config WL->PB15->TIM1_CH3N->AFSEL15_AF4
  //GPIOB->MODER |= GPIO_MODER_MODE15_1;
	
}

void SPWM_Sine_Table_Init(void){
	uint16_t arr_max;
	
	//clear whole table
	for(int i = 0; i <= 360; i++){
    spwm_sine_table[i] = 0;
  }
	
	//assign sine table values
	for(int i = 0; i <= 360; i++){
    float s = sinf(i * 3.14159f / 180.0f);
		arr_max = TIM1->ARR - 20;
		arr_max /= SPWM_POWER_DIV_FACT;
    spwm_sine_table[i] = (uint16_t)(s * arr_max);
  }
	
}

void SPWM_Set_Val(uint16_t u, uint16_t v){
	TIM1->CCR1 = spwm_sine_table[u];
	TIM1->CCR2 = spwm_sine_table[v];
}

void SPWM_Update(void){
	
	if(angle <= 179){
		//disable v pwm
		//set u val
		//enable u pwm
	}
	else{
		//disable u pwm
		//set v val
		//enable v pwm
	}
	
	angle += SPWM_SVPWM_STEP_SIZE;
	if(angle >= 360){
		angle = 0;
	}
	
	
	SPWM_Set_Val(u, v);
}


