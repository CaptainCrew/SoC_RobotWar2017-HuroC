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
#define CAMERA_55 (15)
#define CAMERA_RIGHT (16)
#define CAMERA_RIGHT_END (17)
#define KICK (18)
#define GO_UP (19)
#define GO_DOWN (20)
#define GO_UP_4CM (21)
#define STAND_UP (22)
/*
1. base
2. walk1
3. walk5
4. walk10
5. left_small
6. left_large
7.right_small
8. right_large
9. tleft_small
10. tleft_large
11. tright_small
12. tright_large
13. cfront0
14. cfront45
15. cfront55
16. cright_s
17. cright_e
18. kick
*/
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
