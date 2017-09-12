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
#define CAMERA_60 (12)
#define CAMERA_RIGHT (13)
#define GO_UP (14)
#define GO_DOWN (15)
*/ 

void Order_to_Robot(int motion_number)
{
    printf("Motion Start [%d] : ", motion_number);
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
        case CAMERA_60: puts("CAMERA_60"); break;
        case CAMERA_RIGHT: puts("CAMERA_RIGHT"); break;
        case GO_UP: puts("GO_UP"); break;
        case GO_DOWN: puts("GO_DOWN"); break;
        default: puts("PLEASE EDIT CODE!!"); break;
    }

    Motion(motion_number);

    int delayMilliSecond = 5000;
    switch(motion_number) {
        case BASE:
            delayMilliSecond = 1000; break;
        case WALK: 
            delayMilliSecond = 1000; break;
        case LEFT_SMALL: 
            delayMilliSecond = 1000; break;
        case LEFT_LARGE: 
            delayMilliSecond = 1000; break;
        case RIGHT_SMALL: 
            delayMilliSecond = 1000; break;
        case RIGHT_LARGE: 
            delayMilliSecond = 1000; break;
        case TURN_LEFT_SMALL: 
            delayMilliSecond = 1000; break;
        case TURN_LEFT_LARGE: 
            delayMilliSecond = 1000; break;
        case TURN_RIGHT_SMALL: 
            delayMilliSecond = 1000; break;
        case TURN_RIGHT_LARGE: 
            delayMilliSecond = 1000; break;
        case CAMERA_45: 
            delayMilliSecond = 1000; break;
        case CAMERA_60: 
            delayMilliSecond = 1000; break;
        case CAMERA_RIGHT: 
            delayMilliSecond = 1000; break;
        case GO_UP: 
            delayMilliSecond = 1000; break;
        case GO_DOWN: 
            delayMilliSecond = 1000; break;
        default:
            delayMilliSecond = 1000; break;
    }


    DelayLoop(delayMilliSecond);
    return;
}