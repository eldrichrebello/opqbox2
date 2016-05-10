#include "stm32f30x.h"
#include <stdio.h>
#include "config.h"
#include "datastructures.h"

OpqFB OPQFrameBuffer;

void SetupGPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_AHBPeriphClockCmd(LED0_GPIO_CLK, ENABLE);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = LED0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(LED1_GPIO_CLK, ENABLE);
	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(G1_FLOW_CONTROL_GPIO_CLK, ENABLE);
	/* Configure the G1_FLOW_CONTROL pin */
	GPIO_InitStructure.GPIO_Pin = G1_FLOW_CONTROL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(G1_FLOW_CONTROL_GPIO_PORT, &GPIO_InitStructure);
}


void SetupUart(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_7);

	//Configure USART3 pins:  Rx and Tx ----------------------------
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3,ENABLE);
}


void SetupSPI(void)
{
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// SCK, MISO, MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_6);

	/* CS PA4 */

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI3, &SPI_InitTypeDefStruct);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	SPI_Cmd(SPI3, ENABLE);

	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	 */
}

void setUpSyncTimer(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = SystemCoreClock/1000000 -1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 0xFFFFFFFF;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);

}


int16_t ReadEXTADC(void)
{
	uint16_t data1;
	uint16_t data2;
	SPI_I2S_ReceiveData16(SPI3);
	//SPI_DataSizeConfig(SPI3, SPI_DataSize_5b);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	SPI_I2S_SendData16(SPI3, 0x00);

	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
	data1 = SPI_I2S_ReceiveData16(SPI3);

	//SPI_DataSizeConfig(SPI3, SPI_DataSize_16b);
	SPI_I2S_SendData16(SPI3, 0x00);
	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
	data2 = SPI_I2S_ReceiveData16(SPI3);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	return ((data2 >> 10) & 0x3F) + ((data1 << 6) & 0xFFC0);
}
