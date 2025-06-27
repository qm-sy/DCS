/*
|P0^0|         |P1^0|Analog1    |P2^0|           |P3^0|RX1        |P4^0|           |P5^0|     
|P0^1|         |P1^1|Analog2    |P2^1|Signal_OUT |P3^1|TX1        |P4^1|AC_Out1    |P5^1|     
|P0^2|RX_485   |P1^2|           |P2^2|PWM        |P3^2|ZERO       |P4^2|AC_Out2    |P5^2|     
|P0^3|TX_485   |P1^3|Signal_IN  |P2^3|LED        |P3^3|           |P4^3|AC_Out3    |P5^3|     
|P0^4|DR_485   |P1^4|NTC1       |P2^4|           |P3^4|           |P4^4|           |P5^4|
|P0^5|         |P1^5|NTC2       |P2^5|           |P3^5|           |P4^5|           |P5^5|             
|P0^6|         |P1^6|NTC3       |P2^6|           |P3^6|FAN_TMEP   |P4^6|           |P5^6|     
|P0^7|         |P1^7|NTC4       |P2^7|           |P3^7|Signal_3V3 |P4^7|Buzzer     |P5^7|     
*/ 
#include "gpio.h"

/**
 * @brief	GPIO初始化
 *
 * @param   void
 *
 * @return  void
 */
void GPIO_Init( void )
{
    P0M0 = 0x18;    P0M1 = 0x00;        //P0.3、P0.4为推挽输出
    P1M0 = 0x00;    P1M1 = 0xf8;        //P1.4、P1.5、P1.6、P1.7为高阻输入
    P2M0 = 0x0e;    P2M1 = 0x00;        //P2.0、P2.1、P2.2、P2.3为推挽输出
    P3M0 = 0Xc2;    P3M1 = 0X00;        //P3.6、P3.7为推挽输出
    P4M0 = 0x8e;    P4M1 = 0x00;        //P4.1、P4.2、P4.3、P4.7为推挽输出       
    P5M0 = 0X00;    P5M1 = 0X00;    
}