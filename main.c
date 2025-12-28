
#include "stm32g431xx.h"
#include "spwm.h"

int main(void){
	
	SPWM_Init();
	//SPWM_GPIO_Init();
	
	//GPIOA->MODER |= GPIO_MODER_MODE8_0;
	//GPIOC->MODER |= GPIO_MODER_MODE13_0;
	
	while(1){
		
		//GPIOA->ODR ^= GPIO_ODR_OD8;
		//GPIOC->ODR ^= GPIO_ODR_OD13;
		//for(uint32_t i=0; i<10000; i++){
		//	__NOP();
		//}
		
	}
	
}


