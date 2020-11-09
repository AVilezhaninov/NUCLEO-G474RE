/* CMSIS */
#include "CMSIS/Device/stm32g4xx.h"

/* User */
#include "user/inc/RCC.h"


/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define USART2_BAUDRATE       115200u
#define USART2_IRQ_PRIORITY   5u


/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitUsart2(const uint32_t baudrate, const uint32_t irq_priority);
static void SetUsartBaudRate(USART_TypeDef *const usart,
                             const uint32_t baudrate);


/******************************************************************************/
/* Interrupts *****************************************************************/
/******************************************************************************/
/**
 * USART2 interrupt handler
 * Receive data from USART2 and send it back.
 */
void USART2_IRQHandler() {
  uint8_t received_data;

  if ((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
    received_data = USART2->RDR;  /* Receive data */
    USART2->TDR = received_data;  /* Send it back */
  }
}


/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  RCC_InitSystemClock();
  InitUsart2(USART2_BAUDRATE, USART2_IRQ_PRIORITY);

  while (1) {
    continue;
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
/**
 * Init USART2
 * PA2->Tx, PA3->Rx
 */
static void InitUsart2(const uint32_t baudrate, const uint32_t irq_priority) {
  /* Init USART2 pins */
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;    /* Enable GPIOA clock */
  GPIOA->AFR[0] |= (7u << 8u);            /* PA2 in USART2_Tx AF1 */
  GPIOA->AFR[0] |= (7u << 12u);           /* PA3 in USART2_Rx AF1 */
  GPIOA->MODER &= ~GPIO_MODER_MODER2;     /* Clear PA2 MODER bits */
  GPIOA->MODER |= GPIO_MODER_MODER2_1;    /* PA2 in AF mode */
  GPIOA->MODER &= ~GPIO_MODER_MODER3;     /* Clear PA3 MODER bits */
  GPIOA->MODER |= GPIO_MODER_MODER3_1;    /* PA3 in AF mode */

  /* Init USART2 */
  RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; /* Enable USART2 clock */
  USART2->CR1 |= USART_CR1_RE;            /* Receive enable */
  USART2->CR1 |= USART_CR1_TE;            /* Transmit enable */
  USART2->CR1 |= USART_CR1_RXNEIE;        /* Receive interrupt enable */
  SetUsartBaudRate(USART2, baudrate);     /* Set baud rate */

  NVIC_SetPriority(USART2_IRQn, irq_priority); /* Set USART2 IRQ priority */
  NVIC_EnableIRQ(USART2_IRQn);                 /* Enable USART2 interrupt */

  USART2->CR1 |= USART_CR1_UE;            /* Enable USART2 */
}

/**
 * Set USART baud rate
 * @param usart    USART instance
 * @param baudrate Required baud rate
 */
static void SetUsartBaudRate(USART_TypeDef *const usart,
                             const uint32_t baudrate) {
  usart->BRR = RCC_GetPCLK1Frequency() / baudrate;
}

