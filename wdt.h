

#ifndef _WDT_H_
#define _WDT_H_

#include "stm32g431xx.h"

void    WDT_Init(uint32_t timeout_ms);
void    WDT_Reload(void);

#endif

