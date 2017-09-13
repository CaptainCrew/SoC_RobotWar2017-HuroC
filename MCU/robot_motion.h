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
#define UP_2CM (19)
#define DOWN_2CM (20)
#define UP_4CM (21)
#define GO_4CM (22)
#define STAND_UP (23)

/*
1. base
2. walk_1
3. walk_5
4. walk_10
5. left_small
6. left_large
7. right_small
8. right_large
9. tleft_small
10. tleft_large
11. tright_small
12. tright_large
13. cfront_0
14. cfront_45
15. cfront_55
16. cright_s
17. cright_e
18. kick
19. 2cm_up
20. 2cm_down
21. 4cm_up
22. 4cm_go
*/

#endif
