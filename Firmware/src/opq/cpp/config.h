#ifndef __OPQ_CONFIG_H
#define __OPQ_CONFIG_H

#include "stm32f30x.h"
#include <stdio.h>

//LED PINS
#define LED0_PIN                         GPIO_Pin_6
#define LED0_GPIO_PORT                   GPIOF
#define LED0_GPIO_CLK                    RCC_AHBPeriph_GPIOF

#define LED1_PIN                         GPIO_Pin_7
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOF

//Flow control flag for the RPI
#define G1_FLOW_CONTROL_PIN				 GPIO_Pin_8
#define G1_FLOW_CONTROL_GPIO_PORT		 GPIOD
#define G1_FLOW_CONTROL_GPIO_CLK         RCC_AHBPeriph_GPIOD


//Frame buffer stuff
//Frames in buffer
#define SAMPLING_FB_SIZE 				 60
//What we expect the frequency to be. 60Hz domestic
#define SAMPLING_FREQUENCY_TARGET		 60
//Samples per frame
#define SAMPLING_FRAME_SIZE 			 128
//sampling rate
#define SAMPLING_PLL_DIVIDER			 (SAMPLING_FREQUENCY_TARGET*SAMPLING_FRAME_SIZE)
#define SAMPLING_PLL_INIT				 (SystemCoreClock/SAMPLING_PLL_DIVIDER)

//Histogram stuff
//Number of samples
#define HISTOGRAM_SIZE 					 64
//Step size
#define HISTOGRAM_STEPSIZE 				 (0xFFFF/HISTOGRAM_SIZE)
//Offset
#define HISTOGRAM_OFFSET 				 0x7fff


//Sets up LEDs
void SetupGPIO(void);

//Sets up uart
void SetupUart(void);

//Set up SPI
void SetupSPI(void);

//Sets up the time source for frequency estimation.
void setUpSyncTimer(void);

//Read external ADC via SPI
int16_t ReadEXTADC(void);

#endif
