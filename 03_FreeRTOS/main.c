/* CMSIS */
#include "CMSIS/Device/stm32g4xx.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* User */
#include "user/inc/RCC.h"


/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY  250u


/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitAll(void);
static void InitHardware(void);
static void InitGpio(void);
static void InitRTOSObjects(void);
static void ToggleLed(void);


/******************************************************************************/
/* Tasks **********************************************************************/
/******************************************************************************/
void LedTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    ToggleLed();
    vTaskDelay(LED_TOGGLE_DELAY);
  }
}


/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  InitAll();
  vTaskStartScheduler();

  while (1) {
    continue;
  }
}


/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitAll(void) {
  InitHardware();
  InitRTOSObjects();
}

static void InitHardware(void) {
  RCC_InitSystemClock();
  InitGpio();
}

static void InitGpio(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  /* Enable GPIOA clock */
  GPIOA->MODER &= ~GPIO_MODER_MODER5;   /* Clear PA5 MODER bits */
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  /* PA5 in push-pull mode */
}

static void InitRTOSObjects(void) {
  /* Create tasks */
  xTaskCreate(LedTask, "", configMINIMAL_STACK_SIZE, NULL, 1u, NULL);
}

static void ToggleLed(void) {
  GPIOA->ODR ^= GPIO_ODR_OD5;           /* Toggle PA5 */
}
