#ifndef _NTC_H_
#define _NTC_H_

#include "sys.h"
#include "adc.h"
#include "power_crl.h"
#include "stdio.h"

#define NTC        4

typedef struct 
{
    uint8_t  NTC1_value;   
    uint8_t  NTC2_value;  
    uint8_t  NTC3_value;  
    uint8_t  temp_alarm_value;
    uint8_t  temp_scan_flag;

    uint8_t  dht11_temp;
    uint8_t  dht11_humidity;
}TEMP;

extern TEMP temp;

uint16_t get_temp( uint8_t channle_x );

#endif