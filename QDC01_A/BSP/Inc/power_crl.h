#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"
#include "ntc.h"
#include "eeprom_crl.h"

#define FAN_ON    1
#define FAN_OFF   0

typedef struct 
{
    uint8_t  zero_flag;             //220V输入触发标志位
    uint16_t time_delay;            //移相延时

    uint8_t  channel_num;
    uint8_t  sync_flag;         
    uint8_t  fan_level;  
    uint8_t  power_level; 
    uint8_t  alarm_temp_val;

    uint8_t  mode_num;             //模式选择

    uint8_t signal_in_flag;         //24V输入检测

    uint8_t ac_out1_flag;
    uint8_t ac_out2_flag;
    uint8_t ac_out3_flag;

    uint8_t connect_flag;
    uint8_t alarm_flag;
    uint8_t all_ctrl_flag;
    uint8_t buzzer_call_flag1;
    uint8_t buzzer_call_flag2;
    uint8_t fan_delay_flag;
    uint8_t mode_set_flag;
}AC_DC;

extern AC_DC ac_dc;

void Power_Statu_Init( void );
void led_ctrl( uint8_t on_off );
void ac_220v_crl( uint8_t power_level );
void sync_ctrl( void );
void temp_scan( void );
void fan_ctrl( uint8_t level );
void mode_ctrl( uint8_t mode_num );
void channel_close( void );
void buzzer_call1( void );
void buzzer_call2( void );
void extern_24_listen( void );

#endif