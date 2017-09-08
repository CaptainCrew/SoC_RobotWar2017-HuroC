#ifndef __IMG_ANALYSIS_H__
#define __IMG_ANALYSIS_H__
#include "amazon2_sdk.h"
int MCU_analysis(U16 *_buf, char *_labelData, int* state);

#define START 0
/*
#define Right_Angle_Barricade_Wait 1
#define Right_Angle_Barricade_Go 2
#define Stair 3
#define Stair_READY 4
#define Hurdle 5
#define Hurdle_Lol 6
#define Bridge_Ready 7
*/

#endif
