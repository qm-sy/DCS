#include "communication.h"

RS485 rs485;

uint8_t TX1_busy_Flag = 0;

/**
 * @brief	串口4调用结构体 rs485 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart4_Send_Statu_Init( void )
{
    rs485.RX4_rev_end_Flag = 0;
    rs485.TX4_buf[128] = 0;
    rs485.RX4_buf[128] = 0;
    rs485.TX4_send_bytelength = 0;
    rs485.TX4_send_cnt = 0;
    rs485.RX4_rev_timeout = 0;
    rs485.RX4_rev_cnt = 0;
    DR_485 = 0;
}

/**
 * @brief	串口4中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart4_ISR() interrupt 18 
{   

    /* 1, 检测到S4TI置1，即发送完毕                       */
    if( S4CON & S4TI )          //
    {
        /* 2, 软件将S4TI清零，等待发送标志位重置，可继续发送    */
        S4CON &= ~S4TI;         
        
        /* 3, 依次将TX4_buf中数据送出（写S4BUF操作即为发送）    */
        if( rs485.TX4_send_bytelength != 0 )
        {
            S4BUF = rs485.TX4_buf[rs485.TX4_send_cnt++];
            rs485.TX4_send_bytelength--;
        }else
        {
            rs485.TX4_send_cnt = 0;
            DR_485 = 0;
        }
    }
    
    /* 1, 检测到S2RI置1，即接收完毕                       */
    if( S4CON & S4RI )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        S4CON &= ~S4RI;
        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485.RX4_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485.RX4_rev_cnt > 128 )
            {
                rs485.RX4_rev_cnt = 0;
            }

            /* 5, 依次将RX4_buf中数据接收（读S2BUF操作即为接收）*/
            rs485.RX4_buf[rs485.RX4_rev_cnt] = S4BUF;
            rs485.RX4_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485.RX4_rev_timeout = 5;
    }
}

/**
 * @brief	是否接收完毕判断函数
 *
 * @param   
 *
 * @return  void
**/
void Tim0_ISR( void ) interrupt 1   //1ms
{
    /* 1, 如果接收未超时                                             */
    if ( rs485.RX4_rev_timeout != 0 )  
    {
        rs485.RX4_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485.RX4_rev_timeout == 0 )  
        {
            if( rs485.RX4_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485.RX4_rev_end_Flag = 1;    
            }
        }
    } 
}


/**
 * @brief	串口1中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart1_ISR() interrupt 4
{
    if(TI == 1)
    {
        TI = 0;
        TX1_busy_Flag = 0;
    }
    if(RI == 1)
    {
        RI = 0;
    }
}

/**
 * @brief	串口重定向
 *
 * @param   c:字符串
 *
 * @return  c
**/
char putchar(char c)  // 串口重定向需要添加头文件stdio.h
{
    SBUF = c;
    while(TX1_busy_Flag);
    TX1_busy_Flag = 1;
    return c;
}
