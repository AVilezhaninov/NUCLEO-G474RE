#include "user/inc/RCC.h"


/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define PLL_N  85u
#define PLL_M  6u
#define PLL_R  2u
#define HSE_STARTUP_TIMEOUT 0x05000u


/******************************************************************************/
/* Exported functions *********************************************************/
/******************************************************************************/
void RCC_InitSystemClock(void) {
  uint32_t StartUpCounter = 0u;
  uint32_t HSEStatus = 0u;

  /* Set voltage scaling */
  PWR->CR1 |= PWR_CR1_VOS_0;

  /* Enable HSE */
  RCC->CR |= (uint32_t)RCC_CR_HSEON;

  /* Wait till HSE is ready */
  while ((HSEStatus == 0u) && (StartUpCounter < HSE_STARTUP_TIMEOUT)) {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    ++StartUpCounter;
  }

  if (HSEStatus == RCC_CR_HSERDY) {
    /* Set flash latency */
    FLASH->ACR |= FLASH_ACR_LATENCY_8WS;

    /* Set PLL N */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= (PLL_N << RCC_PLLCFGR_PLLN_Pos);

    /* Set PLL M */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR |= ((PLL_M - 1) << RCC_PLLCFGR_PLLM_Pos);

    /* Set HSE as PLL sourse */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    /* Enable main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Enable main PLL R */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    /* Wait till main PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0u) {
      continue;
    }

    /* Select main PLL as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_1;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
      continue;
    }

    SystemCoreClockUpdate();
  } else {
    /* If HSE fails to start-up, the application will have wrong clock
    configuration. User can add here some code to deal with this error */
    while (1) {
      continue;
    }
  }
}

/**
 * Get HCLK frequency
 * @return  HCLK frequency
 */
uint32_t RCC_GetHCLKFrequency(void) {
  return SystemCoreClock;
}

/**
 * Get PCLK1 frequency
 * @return  PCLK1 frequency
 */
uint32_t RCC_GetPCLK1Frequency(void) {
  return RCC_GetHCLKFrequency() >>
         APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];
}

/**
 * Get PCLK2 frequency
 * @return  PCLK2 frequency
 */
uint32_t RCC_GetPCLK2Frequency(void) {
  return RCC_GetHCLKFrequency() >>
         APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos];
}

/**
 * Blocking delay
 * @param delay
 */
void RCC_StupidDelay(volatile uint32_t delay) {
  while (delay-- > 0u) {
    continue;
  }
}
