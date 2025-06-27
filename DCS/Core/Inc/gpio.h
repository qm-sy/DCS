/*
|P0^0|      |P1^0|      |P2^0|B0    |P3^0|RX1       |P4^0|        |P5^0|         
|P0^1|BL    |P1^1|      |P2^1|B1    |P3^1|TX1       |P4^1|        |P5^1|         
|P0^2|RX4   |P1^2|      |P2^2|B2    |P3^2|LED1      |P4^2|        |P5^2|             
|P0^3|TX4   |P1^3|      |P2^3|B3    |P3^3|LED2      |P4^3|        |P5^3|         
|P0^4|DR4   |P1^4|      |P2^4|      |P3^4|LED3      |P4^4|        |P5^4|    
|P0^5|WR    |P1^5|      |P2^5|      |P3^5|LED4      |P4^5|        |P5^5|             
|P0^6|CS    |P1^6|      |P2^6|      |P3^6|LED5      |P4^6|        |P5^6|     
|P0^7|DATA  |P1^7|      |P2^7|      |P3^7|LED6      |P4^7|        |P5^7|     
*/ 
#ifndef _GPIO_H_
#define _GPIO_H_

#include "STC8H.h"

sbit BL             = P0^1;
sbit RX4            = P0^2;
sbit TX4            = P0^3;
sbit DR_485         = P0^4;
sbit TM1722_WR      = P0^5;
sbit TM1722_CS      = P0^6;
sbit TM1722_DATA    = P0^7;

sbit B0             = P2^0;
sbit B1             = P2^1;
sbit B2             = P2^2;
sbit B3             = P2^3;

sbit RX1            = P3^0;
sbit TX1            = P3^1;
sbit LED1           = P3^2;
sbit LED2           = P3^3;
sbit LED3           = P3^4;
sbit LED4           = P3^5;
sbit LED5           = P3^6;
sbit LED6           = P3^7;

void GPIO_Init( void );

#endif