#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_analysis.h"

int MCU_analysis(U16* buf, int state)
{
  int i, j;
  int Y_cnt = 0,K_cnt = 0, L_K_cnt = 0,R_K_cnt = 0;

  for(i = 0;i<=60;i++)
  {
    for(int j = 115;j>10;j--)
    {
      if(buf[j*180 + i] == 0x0000)
      {
        L_K_cnt++;
        break;
      }
    }
  }

  for(i = 120;i< 180;i++)
  {
    for(int j = 115;j>10;j--)
    {
      if(buf[j*180 + i] == 0x0000)
      {
        R_K_cnt++;
        break;
      }
    }
  }

  for(i = 60;i<120;i++)
  {
    for(j = 115;j > 10;j--)
    {
        if(buf[j*180 + i] == 0x0000)
        {
          K_cnt++;
          break;
        }
        else if(buf[j * 180 + i] == 0xffe0)
        {
          Y_cnt++;
          berak;
        }
    }
  }

  if(K_cnt > 15 && Y_cnt > 15)
  {
      return WAIT;
  }
  else if(L_K_cnt > 30)
  {
    return R_TURN;
  }
  else if(R_L_cnt > 30)
  {
    return L_TURN;
  }
  else
  {
    return F_WALK;
  }
}
