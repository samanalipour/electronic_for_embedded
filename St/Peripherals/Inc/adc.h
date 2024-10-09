#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

typedef enum ADC_SingleSelect_TAG
{
  ADC_SingleSelect_Potentiometer=1,
  ADC_SingleSelect_LM35_1,
  ADC_SingleSelect_LM32_2,
}ADC_SingleSelect_e;

/**
 * @brief ADC Calibrate
 */
void adc_calibrate(void);

/**
 * @brief ADC Start
 */
void adc_start(void);


/**
 * @brief ADC channels GPIO configuration
 */
void adc_GPIO_config(void);


/**
 * @brief ADC Multi-channel
 */
void adc_multiChannelConfig(void);
void adc_multiChannel_DMA_config(uint16_t *pAdcBuffer);

/**
 * @brief ADC analog watchdog configure
 */
void adc_AWD_Config(uint8_t channel, uint16_t lowThreshold, uint16_t highThreshold);


#endif /* INC_ADC_H_ */
