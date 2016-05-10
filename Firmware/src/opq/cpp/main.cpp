/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    20-September-2012
 * @brief   Main program body
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

#include "main.h"
#include  "config.h"
#include "datastructures.h"
#include "stdint.h"
#include <string.h>

#include <string>
#include <sstream>
#include <cstdint>

int main()
{
	//Store our zero crossing here
	static uint32_t crossing[SAMPLING_FB_SIZE];
	uint16_t i = 0;
	//Set up frame buffer
	OPQFrameBuffer.back = 0;
	OPQFrameBuffer.front = 0;


	/* SysTick end of count event each 10ms */
	SystemInit();
	//Set up clock
	SystemCoreClockUpdate();
	//Set up LEDS
	SetupGPIO();
	//Set up UART
	SetupUart();
	//Set up SPI
	SetupSPI();
	//Set up synchronization counter to 1MHz
	setUpSyncTimer();
	//Set up system tick (This is where the sampling takes place)
	SysTick_Config(SAMPLING_PLL_INIT);

	while(1)
	{
		//Check if new data is available.
		if(OPQFrameBuffer.front != OPQFrameBuffer.back)
		{
			//Pointer to the frame we want to process.
			OpqFrame *frame = &OPQFrameBuffer.frames[OPQFrameBuffer.back];
			//Average value.
			float Average = 0;
			//Clear the histogram
			memset(frame->histogram, 0, HISTOGRAM_SIZE);
			//Iterate all samples in frame.
			for(i = 0; i< SAMPLING_FRAME_SIZE; i++)
			{
				//Compute the average
				Average += frame->samples[i];
				//Histogram the values.
				uint8_t bin = (0x7fff + (int32_t)frame->samples[i])/HISTOGRAM_STEPSIZE;
				frame->histogram[bin]++;
			}
			//Compute average
			Average /=SAMPLING_FRAME_SIZE;
			//Store the crossings.
			crossing[OPQFrameBuffer.back] = frame->crossing;
			//Mark frame as read.
			OPQFrameBuffer.back++;
			//Wrap around the frame buffer.
			if(OPQFrameBuffer.back >= SAMPLING_FB_SIZE)
			{
				OPQFrameBuffer.back = 0;
			}
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

                        std::stringstream ss{};
                        ss << frame->crossing << '\n';

                        for(auto c : ss.str()) {
                            USART_SendData(USART3, c); // Echo Char
                            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
                        }

		}
	}
}



/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
