#ifndef __IMG_PROCESS_H__
#define __IMG_PROCESS_H__
#include "amazon2_sdk.h"
void MCU_process(U16* buf, char label[256/8][256/4][256/8], char *labeledData);
#endif
