#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_analysis.h"
#include "robot_motion.h"


typedef char bool;
typedef char Color;

const char RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6, NONE = 7;
const int HEIGHT = 120, WIDTH = 180;


U16 *buf;
Color *labelData;
Color getColor(int j, int i) {
	return labelData[i+180*j];
}
void setBufColor(int j, int i, Color color) {
	U16 lbc[8]={0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFC00, 0xFFE0, 0xF81F};
	buf[i+180*j] = lbc[color];
}

int max(int a, int b) { return a>b?a:b; }
int min(int a, int b) { return a<b?a:b; }


int i, j, k, l;
bool isStartBarricadeOpen();
bool isEndBarricadeOpen();
void lineCheck();

bool isStartBarricadeOpen() {
	int all = 0;
	bool state[HEIGHT + 5] = {0, };
	for(j=0; j<HEIGHT; j++) {
		int cnt = 0, now = -1, stackCnt = 10;
		for(i=0; i<WIDTH; i++) {
			int type = -1;
			Color c = getColor(j, i);
			if(c == YELLOW) {
				if(now == 0) type = now, stackCnt++;
				else if(stackCnt > 7) type = 0, stackCnt = 1;
			}
			if(c == BLACK) {
				if(now == 1) type = now, stackCnt++;
				else if(stackCnt > 7) type = 1, stackCnt = 1;
			}
			if(type != -1 && now != type) now = type, cnt++;
		}
		if(cnt > 5) all++, state[j] = TRUE;
		if(j >= 10 && state[j-10]) all--;
		if(all >= 7) return FALSE;
	}
	return TRUE;
}

const double PI = atan(1) * 4;
double getBlackLineSlope() {
	double xbar = (WIDTH-1) / 2., ybar = 0;
	int Sxx = 0, Sxy = 0, xl = WIDTH / 4, xr = WIDTH * 3 / 4;
	for(i=xl; i<xr; i++) Sxx += (i - xbar) * (i - xbar);
	for(i=xl; i<xr; i++) {
		int firstJ = 0;
		for(j=HEIGHT-1; j>=0; j--) {
			Color c = getColor(j, i);
			if(c == BLACK) { firstJ = j; break; }
		}
		Sxy += (i - xbar) * firstJ; ybar += firstJ;

		//bottom BLACK -> coloring PINK!
		for(j=max(firstJ-4, 0); j<=min(firstJ+4, HEIGHT-1); j++) setBufColor(j, i, NONE);
	}
	ybar /= (xr-xl);

	double beta = (double)Sxy / Sxx, alpha = ybar - beta * xbar;
	//printf("%.2f %.2f\n", alpha, beta);
	//printf("%.2f\n", atan(beta) / PI * 180);
	return atan(beta) / PI * 180;
}

bool isEndBarricadeOpen() {
	int cntBYB = 0;
	for(i=0; i<WIDTH; i++) {
		int cnt = 0, now = 1, stackCnt = 0;
		for(j=0; j<HEIGHT; j++) {
			int type = -1;
			Color c = getColor(j, i);
			if(c == YELLOW) {
				if(now == 0) type = now, stackCnt++;
				else if(stackCnt > 3) type = 0, stackCnt = 1;
			}
			if(c == BLACK) {
				if(now == 1) type = now, stackCnt++;
				else if(stackCnt > 3) type = 1, stackCnt = 1;
			}
			if(type != -1 && now != type) now = type, cnt++;
		}
		if(cnt >= 2) {
			cntBYB++;
			if(cntBYB >= 100) return FALSE;
		}
	}
	return TRUE;
}


int MCU_analysis(U16 *_buf, Color *_labelData, int* state) {
	buf = _buf;
	labelData = _labelData;

	printf("StartBarricade is %s\n", isStartBarricadeOpen()?"Open":"Close");
	printf("LineSlope is %.0f\n", getBlackLineSlope());
	printf("EndBarricade is %s\n", isEndBarricadeOpen()?"Open":"Close");

	return 0;
}

