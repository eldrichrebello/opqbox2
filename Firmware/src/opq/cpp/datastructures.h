#ifndef __OPQ_DATASTRUCTURES_H
#define __OPQ_DATASTRUCTURES_H

#include "config.h"

typedef struct
{
	int16_t samples[SAMPLING_FRAME_SIZE];
	int32_t crossing;
	int16_t max;
	int16_t min;
	int8_t flags;
	int8_t histogram[HISTOGRAM_SIZE];
	float Average;
} OpqFrame;


typedef struct
{
	OpqFrame frames[SAMPLING_FB_SIZE];
	volatile uint8_t front;
	volatile uint8_t back;
} OpqFB;

extern OpqFB OPQFrameBuffer;

#define OPQ_FRAME_GOOD 0x0
#define OPQ_FRAME_SYNC 0x1
#define OPQ_FRAME_SEND 0x2

#endif
