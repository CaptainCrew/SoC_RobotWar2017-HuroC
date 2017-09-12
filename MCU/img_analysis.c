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
const int INF = 0x3f2f1f0f;

/*

수시로 방향조정 (오른쪽을 보고 특정 색깔(디폴트 검정) 각도)
1. 바리케이드 시작
-> 앞으로 걷기
2. 다리 시작 (빨강)
-> 올라가기
-> 방향 조정 (빨강)
-> 걷기
3. 다리 끝 (하양)
-> 내려가기
-> 지뢰를 요리조리 피한다 (low priority)
4. 파랑 두 줄이 보인다 (70도?)
-> 살살 조금 더 걷기
-> 구른다
-> 넘어졌으면 일어나고 방향 조정
-> 초록이 보일때까지 왼쪽 회전 + 조금 더 돌고 방향 판별
-> 걷기
5. 검흰검흰
-> 오른다
-> 잘 걷는다 (방향 조정 빈도 증가)
6. 세로검흰
-> 내려간다
-> 걷는다
7. 주황이 보인다
-> 잘 찬다
-> 노랑이 보일때까지 왼쪽 회전 + 조금 더 돌고 방향 판별
-> 걷기
8. 노랑
-> 오른다
-> 검정을 피해 좌우로
-> 잘 걷는다 (방향 조정 빈도 증가)
9. 흰색
-> 내린다
-> 좌우 간격 맞추기
-> 걷는다
10. 바리케이드 끝
-> 뛴다
-> 머리를 만진다

*/

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


bool isStartBarricadeOpen();
bool isEndBarricadeOpen();
void lineCheck();

int getPercentColor(Color color) {
    int i, j, k, l;

    int cnt = 0;
    for(j=0; j<HEIGHT; j++) for(i=0; i<WIDTH; i++) cnt += (getColor(j, i) == color);
    return cnt * 100 / (HEIGHT * WIDTH);
}
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

double getColorLineSlopeUpOrDown(Color color, int isUp) {
    int i, j, k, l;
    double xbar = (WIDTH-1) / 2., ybar = 0;
    int Sxx = 0, Sxy = 0, xl = WIDTH / 4, xr = WIDTH * 3 / 4;
    for(i=xl; i<xr; i++) Sxx += (i - xbar) * (i - xbar);
    for(i=xl; i<xr; i++) {
        int firstJ = 0;
        if(isUp) {
            for(j=0; j<HEIGHT; j++) {
                Color c = getColor(j, i);
                if(c == color) { firstJ = j; break; }
            }
        }else{
            for(j=HEIGHT-1; j>=0; j--) {
                Color c = getColor(j, i);
                if(c == color) { firstJ = j; break; }
            }
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

double getColorLineSlopeDown(Color color) {
    return getColorLineSlopeUpOrDown(color, 0);
}

double getColorLineSlopeUp(Color color) {
    return getColorLineSlopeUpOrDown(color, 1);
}
int getColorDistanceUpOrDown(Color color, int isUp) {
    int i, j, k, l;
    int xl = WIDTH / 4, xr = WIDTH * 3 / 4, sumHeight = 0;
    for(i=xl; i<xr; i++) {
        int firstJ = 0;
        if(isUp) {
            for(j=0; j<HEIGHT; j++) {
                Color c = getColor(j, i);
                if(c == color) { firstJ = j; break; }
            }
        }else{
            for(j=HEIGHT-1; j>=0; j--) {
                Color c = getColor(j, i);
                if(c == color) { firstJ = j; break; }
            }
        }
        sumHeight += firstJ;
    }
    int averageHeight = HEIGHT-1 - sumHeight / (xr - xl);
    double somethingMul = 1.25;
    double baseCM = 7.5;
    return somethingMul * cos(70. / 180 * PI) * (averageHeight + baseCM);
}
int getColorDistanceDown(Color color) {
    return getColorDistanceUpOrDown(color, 0);
}
int getColorDistanceUp(Color color) {
    return getColorDistanceUpOrDown(color, 1);
}

bool isEndBarricadeOpen() {
	return isVerticalZebra(BLACK, YELLOW, 3, 3, WIDTH, (double)100 / WIDTH);
}


//it is start
#define WAIT_START_BARRICADE (0)
#define SEE_FRONT_IN_START (1)
#define OPEN_START_BARRICADE (2)
#define LETS_FIND_RED_BLOCK (3)
#define FOUND_RED_BLOCK (4)
#define BEFORE_RED_BLOCK_UP (5)
#define WALK_ON_RED_BLOCK (6)
#define BEFORE_RED_BLOCK_DOWN (7)
#define BEFORE_BOMB (8)
#define BEFORE_BEFORE_BLUE (9)
#define BEFORE_BLUE (10)
#define AFTER_BLUE (11)
#define LETS_TURN_LEFT (12)
#define LETS_FIND_GREEN_BLOCK (13)
#define FOUND_GREEN_BLOCK (14)
#define BEFORE_GREEN_BLOCK_UP (15)
#define WALK_ON_GREEN_BLOCK (16)
#define BEFORE_GREEN_BLOCK_DOWN (17)

int nowState = -1, nextState = -1;

#define LETS_MAKE_CAMERA_RIGHT (2)
#define MAKE_LINE_DEGREE_ZERO (1)
#define NO_INFOMATION (0)

int chkFrontDirection = NO_INFOMATION; Color chkColor;
void chkDirectionFuction() {
    int degree, motionNumber = -1;
    switch(chkFrontDirection) {
        case LETS_MAKE_CAMERA_RIGHT:
            Order_to_Robot(CAMERA_RIGHT);
            chkFrontDirection = MAKE_LINE_DEGREE_ZERO;
            break;
        case MAKE_LINE_DEGREE_ZERO:
            degree = getColorLineSlopeDown(chkColor);
            printf("degree : %d\n", degree);
            Order_to_Robot(CAMERA_RIGHT_END);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT;
            if(abs(degree) < 6) chkFrontDirection = NO_INFOMATION, motionNumber = BASE;
            else if(abs(degree) > 18) motionNumber = (degree < 0 ? TURN_LEFT_LARGE : TURN_RIGHT_LARGE);
            else motionNumber = (degree < 0 ? TURN_LEFT_SMALL : TURN_RIGHT_SMALL);
            Order_to_Robot(motionNumber);
            break;
        default:
            break;
    }
}

int cnt = 0;

int bombCnt = 0;
void BOMB_Function() {
    bombCnt++;
    if(bombCnt % 5 == 0) {
        chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = BLACK;
        return;
    }

    //left
    //Order_to_Robot(LEFT_LARGE);
    
    //right
    //Order_to_Robot(RIGHT_LARGE);
    nextState = BEFORE_BOMB;
}
void realFunction(int *state) {
    if(cnt == 1) {
        DelayLoop(15000);
    }
    nowState = *state, nextState = -1;

    printf("nowState : %d | chkFrontDirection : %d\n", nowState, chkFrontDirection);

    bool haveInformation = (chkFrontDirection != NO_INFOMATION);
    if(haveInformation) {
        chkDirectionFuction();
        return nowState;
    }

    int iter;
    switch(nowState) {
        case WAIT_START_BARRICADE:
            Order_to_Robot(CAMERA_0);
            nextState = SEE_FRONT_IN_START;
            DelayLoop(1500);            
            break;
        case SEE_FRONT_IN_START:
            printf("isStartOpen : %d\n", (int)isStartBarricadeOpen());
            nextState = isStartBarricadeOpen() ? OPEN_START_BARRICADE : WAIT_START_BARRICADE;
            DelayLoop(500);
            break;
        case OPEN_START_BARRICADE:
            DelayLoop(1500);
            Order_to_Robot(WALK_5);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = BLACK;
            nextState = LETS_FIND_RED_BLOCK;
            break;
        case LETS_FIND_RED_BLOCK:
            Order_to_Robot(WALK_5);
            nextState = (getPercentColor(RED) >= 10 ? FOUND_RED_BLOCK : LETS_FIND_RED_BLOCK);
            break;
        case FOUND_RED_BLOCK:
            Order_to_Robot(WALK_5);
            nextState = BEFORE_RED_BLOCK_UP;
            break;
        case BEFORE_RED_BLOCK_UP:
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
            Order_to_Robot(GO_UP);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = RED;
            nextState = WALK_ON_RED_BLOCK;
            break;
        case WALK_ON_RED_BLOCK:
            for(iter=0; iter<2; iter++) Order_to_Robot(WALK);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = RED;
            nextState = (getPercentColor(RED) > 10 ? WALK_ON_RED_BLOCK : BEFORE_RED_BLOCK_DOWN);
            break;
        case BEFORE_RED_BLOCK_DOWN:
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
            Order_to_Robot(KICK);
//            Order_to_Robot(GO_DOWN);
            nextState = BEFORE_BOMB;
            break;

        case BEFORE_BOMB:
            for(iter=0; iter<2; iter++) Order_to_Robot(WALK);
            BOMB_Function();

            if(isVerticalZebra(BLUE, WHITE, 3, 3, WIDTH, (double)50 / WIDTH))
                nextState = BEFORE_BEFORE_BLUE;
            break;

        case BEFORE_BEFORE_BLUE:
            Order_to_Robot(WALK_5);
            nextState = BEFORE_BLUE;
            break;
        case BEFORE_BLUE:
            Order_to_Robot(GO_UP_4CM);
            nextState = AFTER_BLUE;
            break;
        case AFTER_BLUE:
            Order_to_Robot(STAND_UP);
            nextState = LETS_TURN_LEFT;
            break;
        case LETS_TURN_LEFT:
            for(iter=0; iter<3; iter++) Order_to_Robot(TURN_LEFT_LARGE);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = BLACK;
            nextState = LETS_FIND_GREEN_BLOCK;
            break;
        case LETS_FIND_GREEN_BLOCK:
            for(iter=0; iter<3; iter++) Order_to_Robot(WALK);
            nextState = (getPercentColor(GREEN) >= 30 ? FOUND_GREEN_BLOCK : LETS_FIND_GREEN_BLOCK);
            break;
        case FOUND_GREEN_BLOCK:
            for(iter=0; iter<5; iter++) Order_to_Robot(WALK);
            nextState = BEFORE_GREEN_BLOCK_UP;
            break;
        case BEFORE_GREEN_BLOCK_UP:
            Order_to_Robot(GO_UP);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = GREEN;
            nextState = WALK_ON_GREEN_BLOCK;
            break;
        case WALK_ON_GREEN_BLOCK:
            for(iter=0; iter<2; iter++) Order_to_Robot(WALK);

            chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = GREEN;
            nextState = (getPercentColor(GREEN) > 10 ? WALK_ON_RED_BLOCK : BEFORE_RED_BLOCK_DOWN);
            break;
        case BEFORE_GREEN_BLOCK_DOWN:
            for(iter=0; iter<3; iter++) Order_to_Robot(WALK);
            Order_to_Robot(GO_DOWN);
            nextState = BEFORE_BOMB;
            break;



        default:
            break;
    }
    if(nextState == -1) {
        printf("kajebiii's code is wrong! [state : %d]", nowState);
        exit(0);
    }

    *state = nextState;
}
void watchColor();
void onlyChkLine();
void printValues();
void motionTest();

void MCU_analysis(U16 *_buf, Color *_labelData, int* state) {
    buf = _buf; labelData = _labelData;
    cnt++;
    if(cnt == 1) {
        Order_to_Robot(BASE);
        DelayLoop(1000);
    }

    //return watchColor();
    //return onlyChkLine();
    //return printValues();
    //return motionTest();
    return realFunction(state);
}


void watchColor() {
    if(cnt == 1) {
        Order_to_Robot(CAMERA_0);
//        Order_to_Robot(CAMERA_RIGHT);
        DelayLoop(1000);
    }
    return;
}
void onlyChkLine() {
    if(cnt == 1) {
        DelayLoop(15000);
        chkFrontDirection = LETS_MAKE_CAMERA_RIGHT; chkColor = BLACK;        
        DelayLoop(2000);
    }
    chkDirectionFuction();    
}
void printValues() {
    printf("[cnt %d] %f\n", cnt, cos(60. / 180. * PI));
//    Order_to_Robot(CAMERA_RIGHT);
    Order_to_Robot(CAMERA_0);
    DelayLoop(2000);
//    printf("DistanceUp : %d\n", getColorDistanceUp(BLACK));
//    printf("DistanceDown : %d\n", getColorDistanceDown(BLACK));
    printf("StartBarricade is %s\n", isStartBarricadeOpen()?"Open":"Close");
    printf("LineSlope is %.0f\n", getColorLineSlopeDown(BLACK));
    printf("EndBarricade is %s\n", isEndBarricadeOpen()?"Open":"Close");
}
void motionTest() {
    if(cnt == 1) {
        DelayLoop(15000);
    }
    Order_to_Robot(cnt);
    DelayLoop(3000);
}