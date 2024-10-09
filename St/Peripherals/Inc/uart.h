#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

/**
 * @brief UART1 GPIO configuration
 */
void uart_UART_GPIO_config(void);

/**
 * @brief UART1 configuration
 */
void uart_UART1_config(void);

/**
 * @brief UART1 transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout);



/**
 * @brief UART2 configuration
 */
void uart_UART3_config(void);

/**
 * @brief UART2 transmit
 */
bool uart_UART3_transmit(uint8_t *data, uint8_t len, uint32_t timeout);



#endif /* INC_UART_H_ */
