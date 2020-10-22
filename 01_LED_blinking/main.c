/* CMSIS */
#include "CMSIS/Device/stm32g4xx.h"


/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY 500000u


/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitGpio(void);
static void ToggleLed(void);
static void DummyDelay(int delay);


/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  InitGpio();

  while (1) {
    ToggleLed();
    DummyDelay(LED_TOGGLE_DELAY);
  }
}


/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitGpio(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  /* Enable GPIOA clock */
  GPIOA->MODER &= ~GPIO_MODER_MODER5;   /* Clear PA5 MODER bits */
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  /* PA5 in push-pull mode */
}

static void ToggleLed(void) {
  GPIOA->ODR ^= GPIO_ODR_OD5;           /* Toggle PA5 */
}

static void DummyDelay(int delay) {
  while (delay-- > 0) {
    continue;
  }
}
