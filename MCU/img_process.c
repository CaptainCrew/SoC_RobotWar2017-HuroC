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
	int x,y,k, a, b;
	char RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6;
	U16 lbc[7]={0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFC00, 0xFFE0};

	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};
		if(j == 60 && i == 90) printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
//		rgb[1] >>= 1;
//		int val = (rgb[0] + rgb[1] + rgb[2]) / 3;
//		for(k=0; k<3; k++) if(rgb[k] > val + 3) rgb[k] = min(31, rgb[k] + 5);
//		rgb[1] <<= 1;
		*color=lbc[labeledData[i+180*j] = label[rgb[0]][rgb[1]][rgb[2]]];
	}

/*
	int sum = 0;
	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};
//		*color=lbc[labeledData[i+180*j] = label[rgb[0]][rgb[1]][rgb[2]]];
		rgb[2] = rgb[2] + 10;
		if(rgb[2] >= 32) rgb[2] = 31;
		*color = (rgb[0] << 11) + (rgb[1] << 5) + rgb[2];

		sum += rgb[0] >> 1;
		sum += rgb[1];
		sum += rgb[2] >> 1;
	}


	sum /= (120 * 180);
	printf("%d\n", sum);
*/

/*
	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};

	}
*/

	/*
	U16* temp = (U16*)malloc(180 * 120);
	for(j=0;j<120;++j)for(i=0;i<180;++i) temp[i+180*j] = 0;	

	for(j=0;j<120;++j)for(i=0;i<180;++i) {
		int rgb[3]={0, 0, 0}, cnt = 0;
		for(y=-1; y<=1; y++) for(x=-1; x<=1; x++)
		{
			a = i+x, b = j+y;
			if(a<0 || a>=180 || b<0 || b>=120) continue;
			cnt++;
			U16 *color=&buf[a+180*b];
			int now[3] = {((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};
			for(k=0; k<3; k++) rgb[k] += now[k];
		}
		for(k=0; k<3; k++) rgb[k] /= cnt;
		if(i == 0 && j == 0) {
			printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
		}
		U16 *color=&temp[i+180*j];
		*color=lbc[labeledData[i+180*j] = label[rgb[0]][rgb[1]][rgb[2]]];
	}
	for(j=0;j<120;++j)for(i=0;i<180;++i) buf[i+180*j] = temp[i+180*j];	
	*/
	return;
}
