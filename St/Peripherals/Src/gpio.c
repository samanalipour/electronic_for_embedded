
#include "gpio.h"

/**
 * @brief LED GPIO Configuration
 */
void gpio_LED_config(void)
{
  //LED: PC13
  //Enable Port C clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  //output mode, low-speed
  GPIOC->CRH &= ~GPIO_CRH_CNF13;
  //Push-pull
  GPIOC->CRH &= ~GPIO_CRH_MODE13;
  GPIOC->CRH |= GPIO_CRH_MODE13_1;
}

/**
 * @brief On board LED write/toggle
 */
void gpio_LED_write(bool state)
{
  if(state)
  {
    GPIOC->ODR |= (1UL << 13);
  }
  else
  {
    GPIOC->ODR &= ~(1UL << 13);
  }
}

void gpio_LED_toggle(void)
{
  GPIOC->ODR ^= (1UL << 13);
}
