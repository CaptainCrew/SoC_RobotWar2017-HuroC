#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_process.h"

void MCU_process(U16* buf, char label[256/8][256/4][256/8], char *labeledData)
{
	int i,j;
	char RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6;
	U16 lbc[7]={0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFC00, 0xFFE0};
	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};
		*color=lbc[labeledData[i+180*j] = label[rgb[0]][rgb[1]][rgb[2]]];
	}
	return;
}
