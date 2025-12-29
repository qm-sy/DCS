#ifndef __TM1722_H_
#define __TM1722_H_

#include "sys.h" 
#include "gpio.h"
#include "delay.h"
#include "modbus_rtu.h"

#define COM1_L 0X01
#define COM2_L 0X02
#define COM3_L 0X04
#define COM4_L 0X08
#define COM1_H 0X10
#define COM2_H 0X20
#define COM3_H 0X40
#define COM4_H 0X80

#define ADDR_03 0
#define ADDR_06 1
#define ADDR_07 2
#define ADDR_0A 3
#define ADDR_0B 4
#define ADDR_0E 5
#define ADDR_0F 6

#define DIS_ON  1
#define DIS_OFF 0

#define LED_OPEN  1
#define LED_SLEEP 2
#define LED_ON    0
#define LED_OFF   1

extern uint8_t value_03;  //该地址的值
extern uint8_t value_06;
extern uint8_t value_07;
extern uint8_t value_0A;
extern uint8_t value_0B;
extern uint8_t value_0E;
extern uint8_t value_0F;

extern uint8_t addr_tab[7];
extern uint8_t num_tab[10];

typedef struct 
{
    uint8_t  power_level;       
    uint8_t  fan_level;  
    uint8_t  sync_switch;
    uint8_t  mode_num;
    uint8_t  channel_num;
    uint8_t  OTP1_alarm_flag;
    uint8_t  OTP_temp1;

    uint8_t  signal_in;
    
    uint8_t lcd_connect_flag;
    uint8_t sync_icon_ctrl_flag;
    uint8_t alarm_set_flag;
    uint8_t alarm_temp_flick_flag;
    uint8_t Power_Swtich;
}LCD_INFO;

extern LCD_INFO lcd_info;

void screen_init( void );
void lcd_info_init( void );
void screen_clear( void );
void screen_display( void );
void screen_write_val(uint8_t addr,uint8_t dat);
void TM1722_Write_Byte(uint8_t dat);

void num_dis(uint8_t num);
void wind_dis(uint8_t num);
void num_dis(uint8_t num);
void channel_dis(uint8_t num);
void sun_dis(uint8_t on_off);
void sync_dis(uint8_t on_off);
void alarm_dis(uint8_t on_off);
void Celsius_dis(uint8_t on_off);
void mode_dis(uint8_t on_off);
void Percentage_dis(uint8_t on_off);
void fan_center_dis(uint8_t on_off);
void fan_leaf1_dis(uint8_t on_off);
void fan_leaf2_dis(uint8_t on_off);
void sync_icon_ctrl( void );
void screen_all_dis( void );
void led_status_ctrl( uint8_t status); 
void fan_rotate_dis( uint8_t on_off );

#endif