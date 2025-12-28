#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"
#include "ntc.h"
#include "eeprom_crl.h"

#define FAN_START_TEMP  45

#define FAN_ON    1
#define FAN_OFF   0

#define BUZZER_ON      0
#define BUZZER_OFF     1

#define SW_OPEN      1
#define SW_CLOSE     0

#define ENABLE     1
#define DISABLE    0

typedef struct 
{
    uint8_t  zero_flag;             //220V输入触发标志位
    uint16_t time_delay;            //移相延时

    uint8_t connect_flag;
    uint8_t alarm_flag;
    uint8_t all_ctrl_flag;
    uint8_t buzzer_call_flag1;
    uint8_t buzzer_call_flag2;
    uint8_t fan_delay_flag;
    uint8_t mode_set_flag;
}AC_DC;

typedef struct 
{
    uint8_t  mode_num;

    uint8_t  channel_num;
    uint8_t  sync_switch;         
    uint8_t  fan_level;  
    uint8_t  power_level; 
    uint8_t  OTP_temp1;  
}SLAVE_06;

typedef struct 
{
    uint8_t  AC1_enable;             //220V输入触发标志位
    uint8_t  AC2_enable;
    uint8_t  AC3_enable;         
    uint8_t  FAN_enable;  
    uint8_t  Power_Swtich; 

    uint8_t  zero_flag;
    uint16_t time_delay;
    uint8_t  OTP_temp2;  
    uint8_t  signal_flag;         //24V输入检测
    uint8_t  fan_delay_flag;

    uint8_t  OTP1_alarm_flag;
    uint8_t  OTP2_alarm_flag;
}POWER_CTRL;


extern AC_DC ac_dc;
extern SLAVE_06 slave_06;
extern POWER_CTRL power_ctrl;

void Power_Statu_Init( void );
void power_level_crl( void );
void fan_ctrl( void );
void temp_scan( void );
void mode_select( void );
void fan_delay_ctrl( void );
void ac_dc_ctrl( void );
uint8_t ACout_enable_judge( void ); 

#endif