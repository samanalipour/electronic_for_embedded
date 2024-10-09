#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"
/**
 * @brief LED GPIO Configuration
 */
void gpio_LED_config(void);

/**
 * @brief On board LED write/toggle
 */
void gpio_LED_write(bool state);
void gpio_LED_toggle(void);

#endif /* INC_GPIO_H_ */
