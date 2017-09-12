#ifndef __ROBOT_MOTION_H__
#define __ROBOT_MOTION_H__
#include "amazon2_sdk.h"
void Order_to_Robot(int motion_number);

//
#define BASE (1)
#define WALK (2)
#define WALK_5 (3)
#define WALK_10 (4)
#define LEFT_SMALL (5)
#define LEFT_LARGE (6)
#define RIGHT_SMALL (7)
#define RIGHT_LARGE (8)
#define TURN_LEFT_SMALL (9)
#define TURN_LEFT_LARGE (10)
#define TURN_RIGHT_SMALL (11)
#define TURN_RIGHT_LARGE (12)
#define CAMERA_0 (13)
#define CAMERA_45 (14)
#define CAMERA_60 (15)
#define CAMERA_RIGHT (16)
#define CAMERA_RIGHT_END (17)
#define GO_UP (18)
#define GO_DOWN (19)


/*
#define F_WALK 0
#define F_WALK_5 1
#define F_WALK_20 2
#define STAIR_LOL 3
#define L_TURN 4
#define R_TURN 5
#define L_WALK 6
#define R_WALK 7
#define HURDLE_LOL 8
*/

#endif
