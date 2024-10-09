/*
 * @brief Adding necessary libraries
 */
#include "main.h"
#include "uart.h"
#include "gpio.h"
#include "adc.h"
#include "flash.h"
#include <string.h>
#include <math.h>
/*
 * @brief Define some variables
 */
uint16_t AdcArray[3] = {0};
bool adcEoc = false;
bool adcAW = false;
int emergency = 0;
float temp=0;
char data[]= "19.09*0*192";
int main(void)
{
	/*
	 * @brief Call functions
	 */
	rcc_HSE_config();
	rcc_SysTick_config(72000);
	uart_UART_GPIO_config();
	uart_UART1_config();
	uart_UART3_config();
//	printf("Start up....!\n");
	gpio_LED_config();
	gpio_LED_write(1);
	adc_GPIO_config();
	adc_multiChannelConfig();
	adc_multiChannel_DMA_config(AdcArray);
	adc_AWD_Config(3,0,3700);
	adc_start();
	//3000 ms delay
	rcc_msDelay(3000);
//	//Define variable for data in memory
	char Mem_data[sizeof(data)];
//	//Reading data of memory at address 0x08007C00
	flash_read16(0x08007C00,(uint16_t *)Mem_data,sizeof(data));
	//Sending the value of memory as initial value to ESP8266
	printf("%s\n",Mem_data);
	while(1){
		//if end of conversion flag is set
		if(adcEoc == true){
			//clearing flag of EOC
			adcEoc = false;
			//Converting input analog voltage of LM35 in temprature
			temp = (float)(AdcArray[1] * 3300) / (4096);
			temp /= 10;
			temp-=273;
			//Converting temp value in string with 2 fraction
			char Temp_buffer[5];
			sprintf (Temp_buffer, "%.2f", temp);
			//printf("Temp value = %s\n",Temp_buffer);
			//Copy temp which is string now into data variable
			strcpy(data,Temp_buffer);
			//Adding a '*' at the end of the data
			char ch[2] = "*\0";
			strcat(data,ch);
			//initialize the value of emergency alarm to 0
			emergency = 0;
			//in this part we check if we have interrupt of AW
			//set the value of emergency alarm to 1
			if(adcAW == true){
				adcAW = false;
				emergency=1;
			}
			//Converting the bool value of emergency alaram into
			//char and add it to data varaiable
			char emergency_buffer[2];
			sprintf(emergency_buffer,"%d",emergency);
			strcat(data,emergency_buffer);
			//Adding a '*' at the end of the data
			char ch2[2] = "*\0";
			strcat(data,ch2);
			//Reading the value of pot
			//Converting it to a 8 bit value
			//add it to data variable
			//Sending 8 bit values through UART3 to FPGA
			uint8_t pot= (uint32_t)(AdcArray[0]*255)/4095;
			uint8_t pp=pot;
			uart_UART3_transmit((uint8_t *)&pp,sizeof(pp),5);
			char pot_buffer [3];
			sprintf (pot_buffer, "%d", pot);
			strcat(data,pot_buffer);
			//Sending data variable to ESP8266
			printf("%s\n",data);
			//Starting ADC
			adc_start();
			rcc_msDelay(1000);
		}
		//Toggling the built-in LED
		gpio_LED_toggle();
		//Unlocking memory
		flash_unlock();
		//Erasing it
		flash_erase(0x08007C00);
		//Writing data variable into memory
		flash_write16(0x08007C00,(uint16_t *)data,sizeof(data));
		//locking memory
		flash_lock();
	}
}


void DMA1_Channel1_IRQHandler(void){
	//Check if transfer complete
	if(DMA1->ISR & DMA_ISR_TCIF1){
		//clear pending interrupt
		NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
		//clear interrupt tansfer flag
		DMA1->IFCR |= DMA_IFCR_CGIF1;
		//Set bool variable
		adcEoc = true;
	}
}

void ADC1_2_IRQHandler(void){
	//Checking AW flag
	if(ADC1->SR & ADC_SR_AWD){
		//we clear the flag
		ADC1->SR &= ~(ADC_SR_AWD);
		//Set bool variable
		adcAW = true;
	}
	//clear pending interrupt
	NVIC_ClearPendingIRQ(ADC1_IRQn);

}
