#ifndef FIRMWARE_RUNTIME_CONFIG_H_H
#define FIRMWARE_RUNTIME_CONFIG_H_H
#include "stm32f3xx_hal.h"
#define OPQ_CONFIG_REG_NUM 10

//Some uart protocol description
#define OPQ_UART_CMD_DELIMITER 0x21
#define OPQ_UART_LONGEST_COMMAND 10

//Register buffer implementation.
void init_OPQ_RunTime();
void OPQ_setReg(uint8_t regNum, int16_t val);
int16_t OPQ_getReg(uint8_t regNum);

#endif //FIRMWARE_RUNTIME_CONFIG_H_H
