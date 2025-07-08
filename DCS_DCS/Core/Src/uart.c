/*
	tim0:串口4不定长数据接收
	tim1:移相斩波
	tim2:串口1
	tim3：
	tim4:串口4
*/
#include "uart.h"

/**
 * @brief	串口1初始化函数--tim2	用作串口通讯
 *
 * @param   
 *
 * @return  void
**/
void Uart1_Init(void)	//115200bps@11.0592MHz
{
	P_SW1 &= 0X3F;   //选择P3.0 P3.1为UART引脚

	SCON  = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器

	AUXR |= 0x04;		//定时器时钟1T模式

	T2L   = 0xE8;			//设置定时初始值
	T2H   = 0xFF;			//设置定时初始值

	AUXR |= 0x10;		//定时器2开始计时

	IE    = 0X10;		//ES = 1 
}

/**
 * @brief	串口4初始化函数--tim4	用作485通讯
 *
 * @param   
 *
 * @return  void
**/
void Uart4_Init(void)	//115200bps@11.0592MHz
{
	S4CON  = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器

	T4T3M |= 0x20;		//定时器时钟1T模式

	T4L    = 0xE8;			//设置定时初始值
	T4H    = 0xFF;			//设置定时初始值

	T4T3M |= 0x80;		//定时器4开始计时

	IE2   |= 0X10;		//ES4 = 1
}

