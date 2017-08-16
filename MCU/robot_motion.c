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

void Order_to_Robot(int motion_number)
{
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

  return;
}
