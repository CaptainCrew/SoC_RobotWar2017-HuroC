#ifndef __IMG_ANALYSIS_H__
#define __IMG_ANALYSIS_H__
#include "amazon2_sdk.h"
void MCU_analysis(U16 *_buf, char *_labelData, int* state);


/*
#define START 0
#define Right_Angle_Barricade_Wait 1
#define Right_Angle_Barricade_Go 2
#define Stair 3
#define Stair_READY 4
#define Hurdle 5
#define Hurdle_Lol 6
#define Bridge_Ready 7
*/

#endif


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