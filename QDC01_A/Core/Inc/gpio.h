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
#ifndef _GPIO_H_
#define _GPIO_H_

#include "sys.h"

sbit RX_485      = P0^2;
sbit TX_485      = P0^3;
sbit DR_485      = P0^4;

sbit Temp_Hum    = P1^0;
sbit Analog2     = P1^1;
sbit Signal_IN   = P1^3;
sbit NTC1        = P1^4;
sbit NTC2        = P1^5;
sbit NTC3        = P1^6;
sbit NTC4        = P1^7;

sbit Signal_OUT  = P2^1;
sbit PWM         = P2^2;
sbit LED         = P2^3;

sbit RX1         = P3^0;
sbit TX1         = P3^1;
sbit ZERO        = P3^2;
sbit FAN_TMEP    = P3^6;
sbit Signal_3V3  = P3^7;

sbit AC_Out1     = P4^1;
sbit AC_Out2     = P4^2;
sbit AC_Out3     = P4^3;
sbit Buzzer      = P4^7;

void GPIO_Init( void );

#endif