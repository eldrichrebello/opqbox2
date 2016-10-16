#include <stm32f3xx_hal.h>
#include <stdio.h>
#include <string.h>
#include <runtime_config.h>
#include <peripheral_config.h>



/**
 * @brief Callback for the conversion completion
 * @param hsdadc handle of the adc.
 */
void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef *hsdadc) {
    uint32_t InjChannel = 8;
    int16_t InjConvValue = HAL_SDADC_InjectedGetValue(&hsdadc2, (uint32_t *) &InjChannel);
    frameBuffer.frames[frameBuffer.head].data[frameBuffer.currentSample] = InjConvValue;
    frameBuffer.currentSample++;
    if (frameBuffer.currentSample >= 200) {
        frameBuffer.currentSample = 0;
        frameBuffer.head++;
        if (frameBuffer.head >= FRAME_BUFFER_SIZE) {
            frameBuffer.head = 0;
        }
    }
}

int main(void) {

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI3_Init();
    MX_USART1_UART_Init();
    MX_SDADC2_Init();

    //Set up the runtime and globals
    init_OPQ_RunTime();
    //Reset the Data ready pin
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    while (1) {
        //Check if the ringbuffer has data
        if ((HAL_SPI_GetState(&hspi3) == HAL_SPI_STATE_READY) && (frameBuffer.head != frameBuffer.tail)) {
            //Queue data for transmission
            HAL_SPI_Transmit_IT(&hspi3, (uint8_t *) &frameBuffer.frames[frameBuffer.tail], sizeof(OPQ_Frame));
            frameBuffer.tail++;
            if (frameBuffer.tail >= FRAME_BUFFER_SIZE) {
                frameBuffer.tail = 0;
            }
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        }
    }
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

}

#endif