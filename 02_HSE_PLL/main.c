/* CMSIS */
#include "CMSIS/Device/stm32g4xx.h"

/* User */
#include "user/inc/RCC.h"


/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  RCC_InitSystemClock(); /* Config SYSCLK for 170 MHz with HSE and PLL */

  while (1) {
    continue;
  }
}
