#include "uart.h"

/**
 * @brief UART1 GPIO config
 */
void uart_UART_GPIO_config(void)
{
	//PA9,PA10
  //Enable Port A clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //Mode to AF and Fast speed
  GPIOA->CRH &= ~GPIO_CRH_CNF9;
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
  GPIOA->CRH &= ~GPIO_CRH_CNF10;
  GPIOA->CRH |= GPIO_CRH_CNF10_0;//Input Floating
  //Output max 10Mhz
  GPIOA->CRH &= ~GPIO_CRH_MODE9;
  GPIOA->CRH |= GPIO_CRH_MODE9_0;
  GPIOA->CRH &= ~GPIO_CRH_MODE10;
  //Map to PA9, PA10
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP);


  //PB10,PB11
  //Enable Port B clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  //Mode to AF and Fast speed
  GPIOB->CRH &= ~(GPIO_CRH_CNF10);
  GPIOB->CRH |= GPIO_CRH_CNF10_1;
  GPIOB->CRH &= ~GPIO_CRH_CNF11;
  GPIOB->CRH |= GPIO_CRH_CNF11_0;//Input Floating
  //Output max 10Mhz
  GPIOB->CRH &= ~GPIO_CRH_MODE10;
  GPIOB->CRH |= GPIO_CRH_MODE10_0;
  GPIOB->CRH &= ~GPIO_CRH_MODE11;
  //Map to PB10, PB11
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR &= ~(AFIO_MAPR_USART3_REMAP);
}

/**
 * @brief UART1 configuration
 */
void uart_UART1_config(void)
{
  //UART clock enable
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  //Transmit Enable
  USART1->CR1 |= USART_CR1_TE;
  //Parity - Even
  USART1->CR1 &= ~(USART_CR1_PS);
  //Parity Control Enable
  USART1->CR1 &= ~(USART_CR1_PCE);
  //Word length = 8bit
  USART1->CR1 &= ~(USART_CR1_M);
  //Stop bits = 1
  USART1->CR2 &= ~(USART_CR2_STOP);
  //Disable Hardware flow control (RTS, CTS)
  USART1->CR3 &= ~(USART_CR3_CTSE);
  USART1->CR3 &= ~(USART_CR3_RTSE);
  //page 798
  //Set Baud rate to 115200 (72MHz = 39.0625 -> 39 / 1)
  //BRR = 72MHz/115200/16 = 39.0625
  //Mantissa = 39
  //Fraction = .0625*16 = 1
  USART1->BRR =0;
  USART1->BRR |= (39UL << 4);
  USART1->BRR |= (1UL << 0);
  //Clear LINEN and CLKEN in CR2
  USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  //Clear SCEN, HDSEL and IREN in CR3
  USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
  //Enable UART
  USART1->CR1 |= USART_CR1_UE;
}



/**
 * @brief UART1 transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout)
{
  //Wait on TXE to start transmit
  //Write to DR as TXE flag is HIGH (Tx buffer Empty)
  uint8_t dataIdx = 0;
  uint32_t startTick = rcc_msGetTicks();
  while(dataIdx<len)
  {
    if(USART1->SR & USART_SR_TXE) //Tx buffer empty
    {
      USART1->DR = data[dataIdx];
      dataIdx++;
    }
    else //Manage timeout
    {
      if((rcc_msGetTicks() - startTick)>= timeout) return false;
    }
  }
  //Wait for busy flag
  while(USART1->SR & USART_SR_TC)
  {
    if((rcc_msGetTicks() - startTick)>= timeout) return false;
  }
  return true;
}

/**
 * @brief UART1 configuration
 */
void uart_UART3_config(void)
{
  //UART clock enable
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

  //Transmit Enable
  USART3->CR1 |= USART_CR1_TE;
  //Parity - Even
  USART3->CR1 &= ~(USART_CR1_PS);
  //Parity Control Enable
  USART3->CR1 &= ~(USART_CR1_PCE);
  //Word length = 8bit
  USART3->CR1 &= ~(USART_CR1_M);
  //Stop bits = 1
  USART3->CR2 &= ~(USART_CR2_STOP);
  //Disable Hardware flow control (RTS, CTS)
  USART3->CR3 &= ~(USART_CR3_CTSE);
  USART3->CR3 &= ~(USART_CR3_RTSE);
  //Set Baud rate to 9600 (36MHz = 234.375 -> 234 / 6)
  //BRR = 36MHz/9600/16 = 234.375
  //Mantissa = 234
  //Fraction = .375*16 = 6
  USART3->BRR =0;
  USART3->BRR |= (234UL << 4);
  USART3->BRR |= (6UL << 0);
  //Clear LINEN and CLKEN in CR2
  USART3->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  //Clear SCEN, HDSEL and IREN in CR3
  USART3->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
  //Enable UART
  USART3->CR1 |= USART_CR1_UE;
}

bool uart_UART3_transmit(uint8_t *data, uint8_t len, uint32_t timeout)
{
  //Wait on TXE to start transmit
  //Write to DR as TXE flag is HIGH (Tx buffer Empty)
  uint8_t dataIdx = 0;
  uint32_t startTick = rcc_msGetTicks();
  while(dataIdx<len)
  {
    if(USART3->SR & USART_SR_TXE) //Tx buffer empty
    {
      USART3->DR = data[dataIdx];
      dataIdx++;
    }
    else //Manage timeout
    {
      if((rcc_msGetTicks() - startTick)>= timeout) return false;
    }
  }
  //Wait for busy flag
  while(USART3->SR & USART_SR_TC)//transmission complete flag
  {
    if((rcc_msGetTicks() - startTick)>= timeout) return false;
  }
  return true;
}
