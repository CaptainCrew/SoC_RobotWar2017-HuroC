#ifndef __IMG_ANALYSIS_H__
#define __IMG_ANALYSIS_H__
#include "amazon2_sdk.h"
int MCU_analysis(U16* buf);

enum MOTION
{
  WAIT = 0;
  F_WALK;
  L_TURN;
  R_TURN;
}

enum STATE
{
  Start = 0;
  Right_Angle_Barricade;
  Stair;
};

#endif
