#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"
//////////////////////////////////////////////////// Protocol Test

void DelayLoop(int delay_time)
{
	//AMAZON-II = 166MHz
	// ms 단위로 카운트 하도록 제작
	delay_time += 300; // 모션 간 추가 딜레이 (모션 씹힘방지)
	delay_time *= 1660;

	while(delay_time)
		delay_time--;
}

void Send_Command(unsigned char Ldata, unsigned char Ldata1)
{
	unsigned char Command_Buffer[6] = {0,};

	Command_Buffer[0] = START_CODE;	// Start Byte -> 0xff
	Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
        Command_Buffer[2] = Ldata;
	Command_Buffer[3] = Ldata1;
	Command_Buffer[4] = Hdata;  // 0x00
	Command_Buffer[5] = Hdata1; // 0xff

	uart1_buffer_write(Command_Buffer, 6);
}

#define ERROR	0
#define OK	1


void Motion(int number) {
	printf("[%d] motion started\n", number);
	Send_Command(number, 0xff - number);
	DelayLoop(5000);
	printf("[%d] motion end\n", number);
}

void motion1()
{
	printf("\nmotion started\n");
	Send_Command(0x01, 0xfe);
	DelayLoop(5000); // 5000ms
	printf("motion end\n");
}

void motion2()
{
	printf("\nmotion started\n");
	Send_Command(0x02, 0xfd);
	DelayLoop(5000); // 5000ms
	printf("motion end\n");
}

void motion3()
{
	printf("\nmotion started\n");
	Send_Command(0x03, 0xfc);
	DelayLoop(5000); // 5000ms
	printf("motion end\n");
}
