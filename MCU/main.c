#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <math.h>
#include "amazon2_sdk.h"
#include "graphic_api.h"
#include "img_process.h"
#include "img_analysis.h"
#include "robot_motion.h"
#include "robot_protocol.h"
#include "uart_api.h"
#include <termios.h>
#define AMAZON2_GRAPHIC_VERSION		"v0.5"
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

int main(int argc, char **argv)
{
	int state = Start;

	if (open_graphic() < 0) {
		return -1;
	}

	SURFACE* bmpsurf = 0;
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);

	int ret;

init_console();

ret = uart_open();
if (ret < 0) return EXIT_FAILURE;

uart_config(UART1, 115200, 8, UART_PARNONE, 1);

	direct_camera_display_off();
	init_robot();

	printf("go\n");

	DelayLoop(5000000);	// 1second delay
	Order_to_Robot(F_WALK); // 걷기

	while (1)
	{
//		clear_screen();
		int motion = 0xff;
		//printf("read fpga video data\n");
		read_fpga_video_data(fpga_videodata); //fpga로부터 처리된 영상데이터를 fpga_videodata에 받아옴

		//MCU_process(fpga_videodata); // MCU를 이용한 영상처리
		motion = MCU_analysis(fpga_videodata, &state); // 영상에대한 분석작업
  //  printf("state : %d, motiom : %d\n",state, motion);

  draw_fpga_video_data_full(fpga_videodata);	//보드에 fpga영상 데이터를 출력
  flip();

    Order_to_Robot(motion); // 가장 적절한 모션을 수행
/*
		if(direct_camera_display_stat() > 0) {
			printf("direct camera display on\n");
			printf("please direct camera diplay off\n");
			break;
		}
		printf("draw fpga video data\n");
*/
	}
	free(fpga_videodata);
	if (bmpsurf != 0)
		release_surface(bmpsurf);
	close_graphic();
	return 0;
}
