

#include "stm32g431xx.h"
#include "spwm.h"
#include "wdt.h"


int main(void){
	
	WDT_Init(500);
	SPWM_Init();
	
	while(1){
		
		WDT_Reload();
		
	}
}



