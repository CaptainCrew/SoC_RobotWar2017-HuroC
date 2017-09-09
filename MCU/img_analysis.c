#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "robot_motion.h"
#include "img_analysis.h"


typedef char bool;
typedef char Color;

const char RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6, NONE = 7;
const int HEIGHT = 120, WIDTH = 180;

const int START = 0;

const int INF = 0x3f2f1f0f;

/*

���÷� �������� (�������� ���� Ư�� ����(����Ʈ ����) ����)
1. �ٸ����̵� ����
-> ������ �ȱ�
2. �ٸ� ���� (����)
-> �ö󰡱�
-> ���� ���� (����)
-> �ȱ�
3. �ٸ� �� (�Ͼ�)
-> ��������
-> ���ڸ� �丮���� ���Ѵ� (low priority)
4. �Ķ� �� ���� ���δ� (70��?)
-> ��� ���� �� �ȱ�
-> ������
-> �Ѿ������� �Ͼ�� ���� ����
-> �ʷ��� ���϶����� ���� ȸ�� + ���� �� ���� ���� �Ǻ�
-> �ȱ�
5. �������
-> ������
-> �� �ȴ´� (���� ���� �� ����)
6. ���ΰ���
-> ��������
-> �ȴ´�
7. ��Ȳ�� ���δ�
-> �� ����
-> ����� ���϶����� ���� ȸ�� + ���� �� ���� ���� �Ǻ�
-> �ȱ�
8. ���
-> ������
-> ������ ���� �¿��
-> �� �ȴ´� (���� ���� �� ����)
9. ���
-> ������
-> �¿� ���� ���߱�
-> �ȴ´�
10. �ٸ����̵� ��
-> �ڴ�
-> �Ӹ��� ������

*/

U16 *buf;
Color *labelData;
Color getColor(int j, int i) {
    int k;
    k = START;
    // = START;
    return labelData[i+180*j];
}
void setBufColor(int j, int i, Color color) {
    U16 lbc[8]={0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFC00, 0xFFE0, 0xF81F};
    buf[i+180*j] = lbc[color];
}

int max(int a, int b) { return a>b?a:b; }
int min(int a, int b) { return a<b?a:b; }


bool isStartBarricadeOpen();
bool isEndBarricadeOpen();
void lineCheck();

bool isHorizontalZebra(Color colorA, Color colorB, int colorWidth, int zebraLimit, int sumHeight, double percentInHeight) {
    int i, j, k, l;
    int all = 0;
    bool state[HEIGHT + 5] ={0, };
    for(j=0; j<HEIGHT; j++) {
        int cnt = 0, now = -1, stackCnt = 10;
        for(i=0; i<WIDTH; i++) {
            int type = -1;
            Color c = getColor(j, i);
            if(c == colorA) {
                if(now == 0) type = now, stackCnt++;
                else if(stackCnt > colorWidth) type = 0, stackCnt = 1;
            }
            if(c == colorB) {
                if(now == 1) type = now, stackCnt++;
                else if(stackCnt > colorWidth) type = 1, stackCnt = 1;
            }
			if(type != -1 && now != type) {
				if(type == -1) {
					if(c == colorA) now = type, cnt++;
				} else now = type, cnt++;
			}
		}
        if(cnt > zebraLimit) all++, state[j] = TRUE;
        if(j >= sumHeight && state[j-sumHeight]) all--;
        if(all >= sumHeight * percentInHeight) return FALSE;
    }
    return TRUE;
}
bool isVerticalZebra(Color colorA, Color colorB, int colorHeight, int zebraLimit, int sumWidth, double percentInWidth) {
    int i, j, k, l;
    int all = 0;
    bool state[WIDTH + 5] ={0, };
    for(i=0; i<WIDTH; i++) {
        int cnt = 0, now = -1, stackCnt = INF;
        for(j=0; j<HEIGHT; j++) {
            int type = -1;
            Color c = getColor(j, i);
            if(c == colorA) {
                if(now == 0) type = now, stackCnt++;
                else if(stackCnt > colorHeight) type = 0, stackCnt = 1;
            }
            if(c == colorB) {
                if(now == 1) type = now, stackCnt++;
                else if(stackCnt > colorHeight) type = 1, stackCnt = 1;
            }
            if(type != -1 && now != type) {
				if(type == -1) {
					if(c == colorA) now = type, cnt++;
				}else now = type, cnt++;
			}
        }
        if(cnt > zebraLimit) all++, state[j] = TRUE;
        if(j >= sumWidth && state[j-sumWidth]) all--;
        if(all >= sumWidth * percentInWidth) return FALSE;
    }
    return TRUE;
}

bool isStartBarricadeOpen() {
	return isHorizontalZebra(YELLOW, BLACK, 7, 5, 10, 0.7);
}

const double PI = atan(1) * 4;
double getColorLineSlope(Color color) {
    int i, j, k, l;
    double xbar = (WIDTH-1) / 2., ybar = 0;
    int Sxx = 0, Sxy = 0, xl = WIDTH / 4, xr = WIDTH * 3 / 4;
    for(i=xl; i<xr; i++) Sxx += (i - xbar) * (i - xbar);
    for(i=xl; i<xr; i++) {
        int firstJ = 0;
        for(j=HEIGHT-1; j>=0; j--) {
            Color c = getColor(j, i);
            if(c == color) { firstJ = j; break; }
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
	return isVerticalZebra(YELLOW, BLACK, 3, 3, WIDTH, (double)100 / WIDTH);
}

int cnt = 0;
int MCU_analysis(U16 *_buf, Color *_labelData, int* state) {
    buf = _buf;
    labelData = _labelData;

	printf("[cnt %d]\n", ++cnt);

    if(cnt >= 21 && cnt <= 33) {
        Order_to_Robot(cnt - 20);
    }else{
        printf("StartBarricade is %s\n", isStartBarricadeOpen()?"Open":"Close");
        printf("LineSlope is %.0f\n", getColorLineSlope(BLACK));
        printf("EndBarricade is %s\n", isEndBarricadeOpen()?"Open":"Close");
    }

    return 0;
}

