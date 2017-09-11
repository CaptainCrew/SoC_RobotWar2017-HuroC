
#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__
// 아래의 프로토콜은 상위(Head B/D)에서 로봇 제어 보드 쪽으로의 전달 프로토콜
#define SCI_BUF_SIZE   4

#define START_CODE    0xFF
#define START_CODE1   0x55
#define Hdata	      0x00
#define Hdata1        0xFF
/* command definitions */
#define CONT_FWD_LEFT	0x01 /* Left Motor move foward Continuously */
#define CONT_FWD_RIGHT  0x02 /* Right Motor move foward Continuously */
#define CONT_FWD_ALL	0x03 /* All Motor move foward Continuously */
#define CONT_BWD_LEFT   0x04 /* Left Motor move backward Continuously */
#define CONT_BWD_RIGHT  0x05 /* Right Motor move backward Continuously */
#define CONT_BWD_ALL	0x06 /* All Motor move backward Continuously */
#define CONT_STOP		0x07 /* All Motor stop */


#define LASER_SHOOT		0x14 /* Shoot the laser pointer */
#define GET_ENERGY		0x15 /* Get the remains energy */
#define GET_SHOT		0x16 /* Get the remains bullet number */

#define SET_V_LEFT		0x17
#define SET_V_RIGHT		0x18
#define SET_V_ALL		0x19
#define GET_V_LEFT		0x1a
#define GET_V_RIGHT		0x1b

#define GET_V_ALL		0x1c ////////////////////////////////////////////


#define CHECK_BOTTOM	0x20		// 바닥 센서의 감지 여부와 감지시 위치 정보 받음
#define CHECK_DAMAGE_POINT	0x30	// 피격되었는지 여부와 맞았을 경우 어느 위치인지 대한 정보 받음
#define LASER_READY	0x40	// 준비 되어있으면 data byte에 1, 준비 안되면 0으로 올라 옵니다.

#define MOTOR_ALARM	0x41	// 앞에 장애물이 있어서 모터가 과부하가 걸려서 동작할수 없을 경우 임.
#define GAME_OVER	0x42	// 게임이 끝났을 경우 임. 이때는 어떠한 명령을 내려도 리턴 값으로 이값만 올라옴.
#define DEAD 0x44

void DelayLoop(int delay_time);
void Send_Command(unsigned char Ldata, unsigned char Ldata1);
unsigned char Receive_Ack(void);
void Motion(int number);
void motion1();
void motion2();
void motion3();
////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOT_PROTOCOL_H__
