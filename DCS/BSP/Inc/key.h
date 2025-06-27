#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"
#include "modbus_rtu.h"
#include "tm1722.h"

#define   PowerKey          0
#define   PowerUp           1
#define   PowerDown         5
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
void power_on_off( uint8_t key_ctrl );
void power_ctrl( uint8_t key_ctrl );
void fan_ctrl( uint8_t key_ctrl );
void channel_ctrl( void );
void mode_ctrl( void );
void sync_ctrl( void );
void alarm_temp_set( void );

#endif