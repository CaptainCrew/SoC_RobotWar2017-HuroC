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

const int BASE = 1, WALK = 2;
const int LEFT_SMALL = 3, LEFT_LARGE = 4;
const int RIGHT_SMALL = 5, RIGHT_LARGE = 6;
const int TURN_LEFT_SMALL = 7, TURN_LEFT_LARGE = 8;
const int TURN_RIGHT_SMALL = 9, TURN_RIGHT_LARGE = 10;
const int CAMERA_45 = 11, CAMERA_60 = 12;
const int CAMERA_RIGHT = 13;

void Order_to_Robot(int motion_number)
{
  Motion(motion_number);
  printf("give motion_number %d\n", motion_number);
  double delaySecond = 0;
  if(motion_number == BASE) delaySecond = 10;
  if(motion_number == WALK) delaySecond = 10;
  if(motion_number == LEFT_SMALL) delaySecond = 10;
  if(motion_number == LEFT_LARGE) delaySecond = 10;
  if(motion_number == RIGHT_SMALL) delaySecond = 10;
  if(motion_number == RIGHT_LARGE) delaySecond = 10;
  if(motion_number == TURN_LEFT_SMALL) delaySecond = 10;
  if(motion_number == TURN_LEFT_LARGE) delaySecond = 10;
  if(motion_number == TURN_RIGHT_SMALL) delaySecond = 10;
  if(motion_number == TURN_RIGHT_LARGE) delaySecond = 10;
  if(motion_number == CAMERA_45) delaySecond = 10;
  if(motion_number == CAMERA_60) delaySecond = 10;
  if(motion_number == CAMERA_RIGHT) delaySecond = 10;

  DelayLoop(5);
  /*
  switch(motion_number)
  {
     case F_WALK:
      Motion(F_WALK);
      DelayLoop(4500000);
     break;
     case F_WALK_5:
      Motion(F_WALK_5);
      DelayLoop(16000000);
    break;
    case F_WALK_20:
      Motion(F_WALK_20);
      DelayLoop(55000000);
    break;
    case STAIR_LOL:
      Motion(STAIR_LOL);
      DelayLoop(165000000);
    break;
    case L_TURN:
      Motion(L_TURN);
      DelayLoop(10000000);
    break;
    case R_TURN:
      Motion(R_TURN);
      DelayLoop(10000000);
    break;
    case L_WALK:
      Motion(L_WALK);
      DelayLoop(2500000);
    break;
    case R_WALK:
      Motion(R_WALK);
      DelayLoop(2500000);
    break;
  }
*/
  return;
}
