#ifndef OPQ_RUNTIME_CONFIG_H_H
#define OPQ_RUNTIME_CONFIG_H_H
#include "stm32f3xx_hal.h"

//Size of the frame buffer
#define FRAME_BUFFER_SIZE 4
//OPQ data frame:
typedef struct{
    int16_t data[200];
    int32_t zero_crossing_high;
    int32_t zero_crossing_low;
} OPQ_Frame;


typedef struct{
    OPQ_Frame frames[FRAME_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t currentSample;
} OPQ_Frame_Buffer;


//Some uart protocol description
#define OPQ_CONFIG_REG_NUM 10
#define OPQ_UART_CMD_DELIMITER 0x21
#define OPQ_UART_LONGEST_COMMAND 10

//Register buffer implementation.
void init_OPQ_RunTime();
void OPQ_setReg(uint8_t regNum, int16_t val);
int16_t OPQ_getReg(uint8_t regNum);

#endif //FIRMWARE_RUNTIME_CONFIG_H_H
