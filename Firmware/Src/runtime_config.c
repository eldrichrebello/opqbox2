#include <runtime_config.h>
//This is the runtime settings data structure organized as a register map.


typedef struct {
    //register map
    int16_t vals[OPQ_CONFIG_REG_NUM];
} OPQ_Runtime_Config;

__IO OPQ_Runtime_Config opq_config;

void init_OPQ_RunTime(){
    int i = 0;
    for(; i< OPQ_CONFIG_REG_NUM; i++){
        opq_config.vals[i] = 0;
    }
}

void OPQ_setReg(uint8_t regNum, int16_t val){
    if(regNum > OPQ_CONFIG_REG_NUM) return;
    opq_config.vals[regNum] = val;
}

int16_t OPQ_getReg(uint8_t regNum){
    if(regNum > OPQ_CONFIG_REG_NUM) return 0;
    return opq_config.vals[regNum];
}


