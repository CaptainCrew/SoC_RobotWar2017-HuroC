#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_process.h"
inline double fastPow(double a, double b)
{
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}
void rgbToXyz(int rgbI[3], double xyz[3])
{
	int i,ik,jk;
	double rgb[3]; for(i=0; i<3; i++) rgb[i] = (double)rgbI[i] / 256.;
	for(i=0; i<3; i++) if(rgb[i] > 0.04045) rgb[i] = fastPow(( (rgb[i] + 0.055) / 1.055), 2.4); else rgb[i] = rgb[i] / 12.92;
	for(i=0; i<3; i++) rgb[i] *= 100;
	double mat[3][3] ={{0.4124, 0.3576, 0.1805}, {0.2126, 0.7152, 0.0722}, {0.0193, 0.1192, 0.9505}};
	for(ik=0; ik<3; ik++) for(jk=0; jk<3; jk++) xyz[ik] += mat[ik][jk] * rgb[jk];
}
void xyzToLab(double xyzD[3], double lab[3])
{
	int i;
	double xyz[3] = {0, 0, 0};
	for(i=0; i<3; i++) xyz[i] = xyzD[i];
	double div[3] = {95.047, 100.000, 108.883};
	for(i=0; i<3; i++) xyz[i] /= div[i];
	for(i=0; i<3; i++) if(xyz[i] > 0.008856) xyz[i] = fastPow(xyz[i], 1./3.); else xyz[i] = (7.787 * xyz[i]) + (16. / 116.);
	double x = xyz[0], y = xyz[1], z = xyz[2];
	lab[0] = 116. * y - 16.; lab[1] = 500. * (x - y); lab[2] = 200. * (y - z);
}
void MCU_process(U16* buf, char label[256/8][256/4][256/8])
{
	/*
	int i,j,k,s,ik;
	int colorRGB[8][3] = {{0}};
	double colorXYZ[8][3] ={{0}};
	double colorLAB[8][3] ={{0}};
	for(s=0; s<8; s++) {
		for(ik=0; ik<3; ik++) if(s & (1<<ik)) colorRGB[s][ik] = 200; else colorRGB[s][ik] = 20;
		rgbToXyz(colorRGB[s], colorXYZ[s]);
		xyzToLab(colorXYZ[s], colorLAB[s]);
	}
	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11)<<3,((*color&0x07E0)>>5)<<2,(*color&0x001F)<<3};
		double xyz[3]={0},lab[3]={0};
		double minV = 1e18; int minIx = -1;
		rgbToXyz(rgb, xyz);
		xyzToLab(xyz, lab);
		for(s=0; s<8; s++) {
			int cnt = 0; for(k=0; k<3; k++) if(s & (1<<k)) cnt++;
			double nowV = 0, mul = 1;
			if(cnt != 0) mul = 2; if(s == 2) mul = 4;
			for(ik=0; ik<3; ik++) nowV += (colorLAB[s][ik] - lab[ik]) * (colorLAB[s][ik] - lab[ik]);
			nowV /= mul;
			if(nowV < minV) minV = nowV, minIx = s;
		}
		if(minV > 2000) *color=((128>>3)<<11)|((128>>2)<<5)|(128>>3);
		else *color=((colorRGB[minIx][0]>>3)<<11)|((colorRGB[minIx][1]>>2)<<5)|(colorRGB[minIx][2]>>3);
	}*/
	int i,j;
	char RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6;
	U16 lbc[7]={0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFC00, 0xFFE0};
	for(j=0;j<120;++j)for(i=0;i<180;++i)
	{
		U16 *color=&buf[i+180*j];
		int rgb[3]={((*color&0xF800)>>11),((*color&0x07E0)>>5),(*color&0x001F)};
		*color=lbc[label[rgb[0]][rgb[1]][rgb[2]]];
	}
	return;
}
