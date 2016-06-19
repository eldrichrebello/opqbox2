/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <runtime_config.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SDADC_HandleTypeDef hsdadc2;

SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void Error_Handler(void);

static void MX_GPIO_Init(void);

static void MX_SDADC2_Init(void);

static void MX_SPI3_Init(void);

static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

extern __IO OPQ_Frame_Buffer frameBuffer;
__IO int16_t InjConvValue;
/**
  * @brief  Injected conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_SDADC_InjectedGetValue or HAL_SDADC_InjectedMultiModeGetValue.
  * @param  hsdadc : SDADC handle.
  * @retval None
  */

void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef* hsdadc)
{
    uint32_t InjChannel = 8;
    InjConvValue = HAL_SDADC_InjectedGetValue(&hsdadc2, (uint32_t *) &InjChannel);
    frameBuffer.frames[frameBuffer.head].data[frameBuffer.currentSample] = InjConvValue;

    frameBuffer.currentSample++;
    if(frameBuffer.currentSample >= 200) {
        frameBuffer.currentSample = 0;
        frameBuffer.head++;
        if (frameBuffer.head >= FRAME_BUFFER_SIZE) {
            frameBuffer.head = 0;
        }
    }
}
/* USER CODE END 0 */

int main(void) {

    /* USER CODE BEGIN 1 */

    char out[10];

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI3_Init();
    MX_USART1_UART_Init();
    MX_SDADC2_Init();
    /* USER CODE BEGIN 2 */
    init_OPQ_RunTime();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {

        /* USER CODE END WHILE */
        sprintf(out, "%hd\r\n", InjConvValue);
        int16_t len = strlen(out);
        HAL_UART_Transmit(&huart1, (uint8_t*)out, len, 0xFFFF);

        /* USER CODE BEGIN 3 */

    }
    /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void) {

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_SDADC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    PeriphClkInit.SdadcClockSelection = RCC_SDADCSYSCLK_DIV12;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }

    HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG2);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SDADC2 init function */
static void MX_SDADC2_Init(void) {

    SDADC_ConfParamTypeDef confParam;

    /**Configure the SDADC low power mode, fast conversion mode,
    slow clock mode and SDADC2 reference voltage
    */
    hsdadc2.Instance = SDADC2;
    hsdadc2.Init.IdleLowPowerMode = SDADC_LOWPOWER_NONE;
    hsdadc2.Init.FastConversionMode = SDADC_FAST_CONV_DISABLE;
    hsdadc2.Init.SlowClockMode = SDADC_SLOW_CLOCK_DISABLE;
    hsdadc2.Init.ReferenceVoltage = SDADC_VREF_EXT;
    hsdadc2.RegularContMode = SDADC_CONTINUOUS_CONV_OFF;
    hsdadc2.InjectedContMode = SDADC_CONTINUOUS_CONV_OFF;

    if (HAL_SDADC_Init(&hsdadc2) != HAL_OK)
        Error_Handler();

    /* -2- Prepare the channel configuration */
    confParam.CommonMode = SDADC_COMMON_MODE_VDDA_2;
    confParam.Gain = SDADC_GAIN_1;
    confParam.InputMode = SDADC_INPUT_MODE_DIFF;
    confParam.Offset = 0x00000000;

    if (HAL_SDADC_PrepareChannelConfig(&hsdadc2, SDADC_CONF_INDEX_0, &confParam) != HAL_OK)
        Error_Handler();

    /* associate channel 8 to the configuration 0 */
    if (HAL_SDADC_AssociateChannelConfig(&hsdadc2, SDADC_CHANNEL_8, SDADC_CONF_INDEX_0) != HAL_OK)
        Error_Handler();

    /* select channel 8 for injected conversion and not for continuous mode */
    /* select channel 8 for injected conversion and not for continuous mode */
    if (HAL_SDADC_InjectedConfigChannel(&hsdadc2, SDADC_CHANNEL_8, SDADC_CONTINUOUS_CONV_OFF) != HAL_OK)
        Error_Handler();

    /* Select external trigger for injected conversion */
    if (HAL_SDADC_SelectInjectedTrigger(&hsdadc2, SDADC_EXTERNAL_TRIGGER) != HAL_OK)
        /* An error occurs during the selection of the trigger */
        Error_Handler();

    /* Select timer 2 channel 3 as external trigger and rising edge */
    if (HAL_SDADC_SelectInjectedExtTrigger(&hsdadc2, SDADC_EXT_TRIG_TIM2_CC3, SDADC_EXT_TRIG_RISING_EDGE) != HAL_OK) {
        /* An error occurs during the selection of the trigger */
        Error_Handler();
    }

    /* Start Calibration in polling mode */
    if (HAL_SDADC_CalibrationStart(&hsdadc2, SDADC_CALIBRATION_SEQ_1) != HAL_OK)
        /* An error occurs during the starting phase of the calibration */
        Error_Handler();

    /* Pool for the end of calibration */
    if (HAL_SDADC_PollForCalibEvent(&hsdadc2, HAL_MAX_DELAY) != HAL_OK)
        /* An error occurs while waiting for the end of the calibration */
        Error_Handler();

    /* Start injected conversion in interrupt mode */
    if (HAL_SDADC_InjectedStart_IT(&hsdadc2) != HAL_OK) {
        /* An error occurs during the configuration of the injected conversion in interrupt mode */
        Error_Handler();
    }

    /* Start the TIMER's Channel */
    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK) {
        /* An error occurs during the configuration of the timer in PWM mode */
        Error_Handler();
    }

}

/* SPI3 init function */
static void MX_SPI3_Init(void) {

    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_SLAVE;
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize = SPI_DATASIZE_4BIT;
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi3.Init.NSS = SPI_NSS_HARD_INPUT;
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial = 7;
    hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if (HAL_SPI_Init(&hspi3) != HAL_OK) {
        Error_Handler();
    }

}


/* USART1 init function */
static void MX_USART1_UART_Init(void) {

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void) {

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PF6 PF7 */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pin : PB6 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
    }
    /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
