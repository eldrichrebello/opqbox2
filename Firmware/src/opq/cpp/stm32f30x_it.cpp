/**
 ******************************************************************************
 * @file    stm32f30x_it.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    20-September-2012
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_it.h"
#include "config.h"
#include "datastructures.h"
#include "main.h"
#include "math.h"

/** @addtogroup STM32F3-Discovery_Demo
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */


void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
/*
void SysTick_Handler(void)
{
	static int32_t tmrValue[500];
	static int32_t sampleValue[500];
	static int16_t oldAdc;
	static int16_t lastCycleAdc;
	static int locked;
	static int index;
	static int16_t sampleNumber;
	static int8_t lockCounter;

	int16_t adcValue;
	adcValue= ReadEXTADC();
	sampleNumber++;


	if(index >= 500)
	{
		index++;
		return;
	}

	if(locked)
	{
		if(sampleNumber == 256)
		{
			tmrValue[index] = TIM_GetCounter(TIM2);
			sampleValue[index] = adcValue;
			sampleNumber = 0;
			index++;
		}
	}
	if((adcValue > 0) && (oldAdc < 0))
	{
		int cycleDiff = lastCycleAdc-adcValue;
		if(abs(cycleDiff) < 100)
		{
			if(lockCounter > 30)
			{
				locked = 1;
				sampleNumber = 0;
			}
			lockCounter++;
		}
		else
		{
			lockCounter=0;
			int a = SysTick->LOAD;
			if((lastCycleAdc - adcValue) < 0)
			{
				SysTick->LOAD--;
			}
			else
				SysTick->LOAD++;
		}
		lastCycleAdc = adcValue;
	}

	oldAdc = adcValue;
}
 */

/**
 * Sampling happens here
 */
void SysTick_Handler(void)
{
	//Last ADC Sample.
	static int16_t oldADC;
	//Sample Number in the frame buffer.
	static uint16_t sampleNumber;
	//Read the external ADC.
	int16_t readADC = ReadEXTADC();
	//Figure out zero crossings and interpolate.
	if((oldADC < 0) && (readADC >=0))
	{
		OPQFrameBuffer.frames[OPQFrameBuffer.front].crossing = TIM_GetCounter(TIM2);
		float slope = SAMPLING_PLL_DIVIDER*(readADC - oldADC)/(1000000.0f);
		OPQFrameBuffer.frames[OPQFrameBuffer.front].crossing -= readADC/slope;

	}
	//Store the sample in the frame buffer.
	OPQFrameBuffer.frames[OPQFrameBuffer.front].samples[sampleNumber] = readADC;
	//Housekeeping to make sure we dont overrun the frame/framebuffer.
	oldADC = readADC;
	sampleNumber++;
	if(sampleNumber == SAMPLING_FRAME_SIZE)
	{
		sampleNumber = 0;
		OPQFrameBuffer.front++;
		if(OPQFrameBuffer.front >= SAMPLING_FB_SIZE)
			OPQFrameBuffer.front = 0;
	}

}

///******************************************************************************/
///*                 STM32F30x Peripherals Interrupt Handlers                   */
///*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
///*  available peripheral interrupt handler's name please refer to the startup */
///*  file (startup_stm32f30x.s).                                            */
///******************************************************************************/
///**
//  * @brief  This function handles EXTI0_IRQ Handler.
//  * @param  None
//  * @retval None
//  */
//void EXTI0_IRQHandler(void)
//{
//}
//
//#if defined (USB_INT_DEFAULT)
//void USB_LP_CAN1_RX0_IRQHandler(void)
//#elif defined (USB_INT_REMAP)
//void USB_LP_IRQHandler(void)
//#endif
//{
//   USB_Istr();
//}
//
//#if defined (USB_INT_DEFAULT)
//void USBWakeUp_IRQHandler(void)
//#elif defined (USB_INT_REMAP)
//void USBWakeUp_RMP_IRQHandler(void)
//#endif
//{
//  /* Initiate external resume sequence (1 step) */
//  Resume(RESUME_EXTERNAL);
//  EXTI_ClearITPendingBit(EXTI_Line18);
//}
///**
//  * @brief  This function handles PPP interrupt request.
//  * @param  None
//  * @retval None
//  */
///*void PPP_IRQHandler(void)
//{
//}*/

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

