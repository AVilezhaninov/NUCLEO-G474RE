#ifndef RCC_H_
#define RCC_H_


/* CMSIS */
#include "CMSIS/Device/stm32g4xx.h"
#include "CMSIS/Device/system_stm32g4xx.h"


/******************************************************************************/
/* Exported functions prototypes **********************************************/
/******************************************************************************/
void RCC_InitSystemClock(void);

uint32_t RCC_GetHCLKFrequency(void);
uint32_t RCC_GetPCLK1Frequency(void);
uint32_t RCC_GetPCLK2Frequency(void);

void RCC_StupidDelay(volatile uint32_t delay);


#endif /* RCC_H_ */
