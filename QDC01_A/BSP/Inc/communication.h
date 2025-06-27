#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "uart.h"
#include "stdio.h"
#include "gpio.h"


#define S4TI    0X02
#define S4RI    0X01
extern uint8_t cnt1 ;
extern uint8_t cnt2 ;
typedef struct 
{
    uint8_t     RX4_rev_end_Flag;       //数据包接收完毕标志
    uint8_t     TX4_buf[128];           //SBUF TI缓冲区
    uint8_t     RX4_buf[128];           //SBUF RI缓冲区
    uint8_t     TX4_send_bytelength;    //发送字节数
    uint8_t     TX4_send_cnt;           //发送计数
    uint16_t    RX4_rev_timeout;        //接收超时
    uint8_t     RX4_rev_cnt;            //接收计数
    uint8_t     DR_Flag;                //DR
}RS485;

extern RS485 rs485;

void Uart4_Send_Statu_Init( void );

char putchar(char c);

#endif