#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "robot_motion.h"
#include "robot_protocol.h"
#include "uart_api.h"
#include "img_analysis.h"

/*
#define BASE (1)
#define WALK (2)
#define LEFT_SMALL (3)
#define LEFT_LARGE (4)
#define RIGHT_SMALL (5)
#define RIGHT_LARGE (6)
#define TURN_LEFT_SMALL (7)
#define TURN_LEFT_LARGE (8)
#define TURN_RIGHT_SMALL (9)
#define TURN_RIGHT_LARGE (10)
#define CAMERA_45 (11)
#define CAMERA_55 (12)
#define CAMERA_RIGHT (13)
#define GO_UP (14)
#define GO_DOWN (15)
*/ 

void Order_to_Robot(int motion_number)
{
    printf("Motion Start [%d] : ", motion_number);
    /*
    switch(motion_number) {
        case BASE: puts("BASE"); break;
        case WALK: puts("WALK"); break;
        case LEFT_SMALL: puts("LEFT_SMALL"); break;
        case LEFT_LARGE: puts("LEFT_LARGE"); break;
        case RIGHT_SMALL: puts("RIGHT_SMALL"); break;
        case RIGHT_LARGE: puts("RIGHT_LARGE"); break;
        case TURN_LEFT_SMALL: puts("TURN_LEFT_SMALL"); break;
        case TURN_LEFT_LARGE: puts("TURN_LEFT_LARGE"); break;
        case TURN_RIGHT_SMALL: puts("TURN_RIGHT_SMALL"); break;
        case TURN_RIGHT_LARGE: puts("TURN_RIGHT_LARGE"); break;
        case CAMERA_45: puts("CAMERA_45"); break;
        case CAMERA_55: puts("CAMERA_55"); break;
        case CAMERA_RIGHT: puts("CAMERA_RIGHT"); break;
        case GO_UP: puts("GO_UP"); break;
        case GO_DOWN: puts("GO_DOWN"); break;
        default: puts("PLEASE EDIT CODE!!"); break;
    }
    */

    Motion(motion_number);

    int delayMilliSecond = 5000;
    switch(motion_number) {
        case BASE:
            delayMilliSecond = 1000; break;
        case WALK: 
            delayMilliSecond = 1000; break;
        case WALK_5: 
            delayMilliSecond = 3000; break;
        case WALK_10: 
            delayMilliSecond = 5000; break;
        case LEFT_SMALL: 
            delayMilliSecond = 1000; break;
        case LEFT_LARGE: 
            delayMilliSecond = 1500; break;
        case RIGHT_SMALL: 
            delayMilliSecond = 1000; break;
        case RIGHT_LARGE: 
            delayMilliSecond = 1500; break;
        case TURN_LEFT_SMALL: 
            delayMilliSecond = 2500; break;
        case TURN_LEFT_LARGE: 
            delayMilliSecond = 2500; break;
        case TURN_RIGHT_SMALL: 
            delayMilliSecond = 2500; break;
        case TURN_RIGHT_LARGE: 
            delayMilliSecond = 2500; break;
        case CAMERA_0: 
            delayMilliSecond = 1000; break;
        case CAMERA_45: 
            delayMilliSecond = 1000; break;
        case CAMERA_55: 
            delayMilliSecond = 1000; break;
        case CAMERA_RIGHT: 
            delayMilliSecond = 4000; break;
        case CAMERA_RIGHT_END: 
            delayMilliSecond = 4000; break;
        case KICK: 
            delayMilliSecond = 1000; break;
        case UP_2CM: 
            delayMilliSecond = 25000; break;
        case DOWN_2CM: 
            delayMilliSecond = 25000; break;
        case UP_4CM: 
            delayMilliSecond = 25000; break;
        case GO_4CM: 
            delayMilliSecond = 25000; break;
        default:
            delayMilliSecond = 1000; break;
   }
    /*
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
    */


    DelayLoop(delayMilliSecond);
    return;
}