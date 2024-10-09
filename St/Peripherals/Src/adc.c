#include "adc.h"
/**
 * @brief ADC channels GPIO configuration
 */
void adc_GPIO_config(void)
{
  //Enable port A clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //ADC pins; PA1
  //Mode: Analog
  GPIOA->CRL &= ~(GPIO_CRL_MODE1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF1);
  //ADC pins; PA2
  //Mode: Analog
  GPIOA->CRL &= ~(GPIO_CRL_MODE2);
  GPIOA->CRL &= ~(GPIO_CRL_CNF2);
  //ADC pins; PA3
  //Mode: Analog
  GPIOA->CRL &= ~(GPIO_CRL_MODE3);
  GPIOA->CRL &= ~(GPIO_CRL_CNF3);
}

/**
 * @brief ADC Start
 */
void adc_start(void)
{
  ADC1->SR = 0;
  ADC1->CR2 |= (1UL << 22);
  ADC1->CR2 |= (1UL << 0); //Page 218
}


/**
 * @brief ADC Multi-channel
 */
void adc_multiChannelConfig(void)
{
  //Enable ADC clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  //Right Align DR
  ADC1->CR2 &= ~(ADC_CR2_ALIGN);
  //Sample time to 28 Cycles = (12 + 28)/12MHz = 3.3us
  ADC1->SMPR2 &= ~(7UL << 3); //Rank1
  ADC1->SMPR2 |= (3UL << 3);
  ADC1->SMPR2 &= ~(7UL << 6); //Rank2
  ADC1->SMPR2 |= (3UL << 6);
  ADC1->SMPR2 &= ~(7UL << 9); //Rank3
  ADC1->SMPR2 |= (3UL << 9);
  //Software trigger (SWSTART)
  ADC1->CR2 |= (ADC_CR2_EXTSEL);
  //Sequencer
  ADC1->SQR3 = (1UL << 0*5); //Rank 1 --> PA1, Channel1
  ADC1->SQR3 |= (2UL << 1*5); //Rank 2 --> PA2, Channel2
  ADC1->SQR3 |= (3UL << 2*5); //Rank 3 --> PA3, Channel3
  //No. of conversion = 3
  ADC1->SQR1 &= ~(ADC_SQR1_L);
  ADC1->SQR1 |= (2UL << 20);//2: 3 Conversions

  //Enable Scan mode
  ADC1->CR1 |= (ADC_CR1_SCAN);
  //Enable DMA
  ADC1->CR2 |= (ADC_CR2_DMA);
  //Enable ADC Single conversion cycle
  ADC1->CR2 &= ~(ADC_CR2_CONT);
  //Power up ADC
  ADC1->CR2 |= (ADC_CR2_ADON);
  //Wait for ADC clock to stabilise (couple micro seconds)
  for(uint16_t i=0; i<36;i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36
}

void adc_multiChannel_DMA_config(uint16_t *pAdcBuffer)
{
  //Enable DMA1 Clock
  RCC->AHBENR |= (RCC_AHBENR_DMA1EN);

  //DMA1_Channel1 clear flags
  DMA1->IFCR = 0xF;

  //Peripheral address
  DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); //address 32bit adc register data
  //Memory address
  DMA1_Channel1->CMAR = (uint32_t)pAdcBuffer;
  //no. of transfers baraye ma 3ta
  DMA1_Channel1->CNDTR = 3;
  //Circular data az 0 ta 2 miravad va sepas reset mishavad agar normal bashad edame miyabad ta reset konim
  DMA1_Channel1->CCR |= (DMA_CCR_CIRC);
  //Enable memory increment yani array 0 bad 1 bad 2 hey increment mikonad
  DMA1_Channel1->CCR |= (DMA_CCR_MINC);
  //Disable Peripheral increment chon y adc darim increment pheripheral nadarim
  DMA1_Channel1->CCR &= ~(DMA_CCR_PINC);
  //Set peripheral data size to 16
  DMA1_Channel1->CCR &= ~(DMA_CCR_PSIZE);
  DMA1_Channel1->CCR |= (DMA_CCR_PSIZE);
  //Set Memory data size to 16
  DMA1_Channel1->CCR &= ~(DMA_CCR_MSIZE);
  DMA1_Channel1->CCR |= (DMA_CCR_MSIZE_0);
  //Direction = Periph to Mem
  DMA1_Channel1->CCR &= ~(DMA_CCR_DIR);
  //Enable DMA Channel
  DMA1_Channel1->CCR |= (DMA_CCR_EN);

  /** Enable Transfer Complete Interrupt **/
  DMA1_Channel1->CCR |= DMA_CCR_TCIE;
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}


/**
 * @brief ADC analog watchdog configure
 */
void adc_AWD_Config(uint8_t channel, uint16_t lowThreshold, uint16_t highThreshold)
{
  //Set AWD Low Threshold
  ADC1->LTR = lowThreshold;
  //Set AWD High Threshold
  ADC1->HTR = highThreshold;
  //Select AWD channel
  ADC1->CR1 &= ~(0x0000001F);
  ADC1->CR1 |= (channel << 0);
  //Enable AWDIE Interrupt
  ADC1->CR1 |= ADC_CR1_AWDIE;
  //AWD on Single Channel
  ADC1->CR1 |= ADC_CR1_AWDSGL;
  //Disable Injected
  ADC1->CR1 &= ~(ADC_CR1_JAWDEN);
  //Enable Regular Channel AWD
  ADC1->CR1 |= ADC_CR1_AWDEN;
  NVIC_EnableIRQ(ADC1_IRQn);
}

//void adc_singleConfig(ADC_SingleSelect_e channel)
//{
//  //Enable ADC clock
//  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//
//  //Right Align DR
//  ADC1->CR2 &= ~(1UL << 11);
//  //Single conversion
//  ADC1->CR2 &= ~(1UL << 1);
//  //Software trigger (SWSTART)
//  ADC1->CR2 |= (7UL << 17);
//  //Sample time to 28 Cycles = (12 + 28)/12MHz = 3.3us
//  ADC1->SMPR2 &= ~(7UL << 3);
//  ADC1->SMPR2 |= (3UL << 3);
//  //Rank 1 --> PA1, PA2 or PA3, Channel1
//  ADC1->SQR3 = channel;
//  //Power up ADC
//  ADC1->CR2 |= (1UL << 0);
//  //Wait for ADC clock to stabilise (couple micro seconds)
//  for(uint16_t i=0; i<36;i++); //72 cycles = 1uSec, for loop 4 cycles each, 72*2/4 = 36
//}

//bool adc_pollForEndOfConversion(uint32_t msTimeout)
//{
//  uint32_t initTicks = rcc_msGetTicks();
//  while(((ADC1->SR)&0x02)==0)
//  {
//    if((rcc_msGetTicks() - initTicks) > msTimeout)
//    {
//      return 0;
//    }
//  }
//  return 1;
//}

/**
 * @brief Read value
 */
uint16_t adc_readValue(void)
{
  return (ADC1->DR)&0xFFF; //12-bits resolution Right Aligned
}
