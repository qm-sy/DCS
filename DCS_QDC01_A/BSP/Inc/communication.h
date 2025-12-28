#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "uart.h"
#include "stdio.h"
#include "gpio.h"


#define S4TI    0X02
#define S4RI    0X01

typedef struct 
{
    uint8_t     rcv_end_Flag;       //数据包接收完毕标志
    uint8_t     send_buf[128];      //SBUF TI缓冲区
    uint8_t     rcv_buf[128];       //SBUF RI缓冲区
    uint8_t     send_bytelength;    //发送字节数
    uint8_t     send_cnt;           //发送计数
    uint16_t    rcv_timeout;        //接收超时
    uint8_t     rcv_cnt;            //接收计数
}RS485;

extern RS485 rs485;

void Uart4_Send_Statu_Init( void );

char putchar(char c);

#endif