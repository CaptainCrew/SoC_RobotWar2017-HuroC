#ifndef __ROBOT_MOTION_H__
#define __ROBOT_MOTION_H__
#include "amazon2_sdk.h"
void Order_to_Robot(int motion_number);

#define F_WALK 0
#define F_WALK_5 1
#define F_WALK_20 2
#define STAIR_LOL 3
#define L_TURN 4
#define R_TURN 5
#define L_WALK 6
#define R_WALK 7
#define HURDLE_LOL 8

#endif
