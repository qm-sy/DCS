#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"
#include "modbus_rtu.h"
#include "tm1722.h"

#define   PowerKey          0
#define   Num_Up           1
#define   Num_Down         5
#define   Function          3
#define   Mode              4
#define   Channel           2
#define   FanUp             9
#define   FanDown           13

#define   POWER_ON          1
#define   POWER_OFF         0

extern uint8_t key_value;


typedef struct 
{
    uint8_t  key_read_allow;       
}KEY;

extern KEY key;

void key_scan( void );
void Power_Ctrl( uint8_t power_switch );
void power_temp_set( uint8_t key_val );
void fan_level_ctrl( uint8_t key_val );
void channel_ctrl( void );
void mode_ctrl( void );
void sync_ctrl( void );
void alarm_temp_set( void );

#endif