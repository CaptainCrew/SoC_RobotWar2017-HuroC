 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_analysis.h"
#include "robot_motion.h"

int MCU_analysis(U16* buf, int* state)
{
  int i, j;
  int R_cnt = 0,G_cnt = 0,B_cnt = 0,Y_cnt = 0,K_cnt = 0;
  int R_avg = 0,K_avg = 0;

  switch(*state)
  {
    case Start:
      for(i = 30;i<150;i++)
      {
        for(j = 118;j > 60;j--)
        {
          switch(buf[j*180+i])
          {
            case 0x0000:
             buf[j*180+i] = 0x7ff;
             K_cnt++;
             j = 0;
             break;
            case 0xffe0:
            case 0xf800:
             buf[j*180+i] = 0x7ff;
             Y_cnt++;
             j = 0;
             break;
          }
        }
      }

      if(Y_cnt > 40 && K_cnt > 40)
      {
          *state = Right_Angle_Barricade_Wait;
          return F_WALK;
      }
      return 0xff;

    case Right_Angle_Barricade_Wait:
      for(i = 30;i<150;i++)
      {
        for(j = 118;j > 60;j--)
        {
          switch(buf[j*180+i])
          {
            case 0x0000:
             buf[j*180+i] = 0x7ff;
             K_cnt++;
             j = 0;
             break;
            case 0xffe0:
            case 0xf800:
             buf[j*180+i] = 0x7ff;
             Y_cnt++;
             j = 0;
             break;
          }
        }
      }

      if(Y_cnt < 40 && K_cnt < 40)
      {
        *state = Right_Angle_Barricade_Go;
        return F_WALK_20;
      }

    return 0xff;

    case Right_Angle_Barricade_Go:
      *state = Stair_READY;
    return F_WALK_5;

    case Stair:
    for(i = 30;i<60;i++)
    {
      for(j = 119;j>60;j++)
      {
        if(buf[j*180+i] == 0x0000)
        {
          K_cnt++;
          break;
        }
      }
    }

    if(K_cnt > 20)return R_TURN;

    K_cnt = 0;

    for(i = 150;i < 180;i++)
    {
      for(j = 119;j>60;j++)
      {
        if(buf[j*180+i] == 0x0000)
        {
          K_cnt++;
          break;
        }
      }
    }

    if(K_cnt > 20)return L_TURN;

    R_cnt = 0;
    for(i = 60;i<120;i++)
    {
      for(j = 110;j<60;j--)
      {
        if(buf[j*180+i] == 0xf800)
        {
            R_cnt++;
            break;
        }
      }
    }

    if(R_cnt > 40)
    {
        *state = Stair_READY;
        return F_WALK_5;
    }

    return F_WALK;

    case Stair_READY:

    for(i = 0;i<180;i++)
    {
      if(buf[80*180+i] == 0xf800)
      {
        R_cnt++;
        R_avg += i;
      }
    }

    R_avg /= R_cnt;

    if(R_avg < 70)
      return L_WALK;
    else if(R_avg > 110)
      return R_WALK;
    else
    {
      *state = Hurdle;
      return STAIR_LOL;
    }
    break;

    case Hurdle:

      K_cnt = K_avg = 0;

      for(i = 70;i<110;i++)
      {
        for(j = 110;j<60;j++)
          if(buf[j*180+i]==0x0000)
          {
            K_cnt++;
            K_avg += i;
          }
          else if(buf[j*180+i] == 0x001f)
            B_cnt++;
      }

      if(B_cnt > 30)
      {
        *state = Hurdle_Lol;
        return F_WALK_20;
      }
      else if(K_cnt < 20)
        return F_WALK_5;
      else
      {
          K_avg /= K_cnt;
          if(K_avg < 90)
            return R_TURN;
          else
            return L_TURN;
      }

    break;

    case Hurdle_Lol:
    *state = Bridge_Ready;
    return HURDLE_LOL;

    return 0xff;
  }

  return 0xff;
}
