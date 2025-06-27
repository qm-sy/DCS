#include "common.h"

uint16_t modbus_04scan_cnt = 0;

/**
 * @brief	一些定时执行的事件   Timer3：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19
{
    static uint16_t key_scan_cnt = 0;
    static uint16_t key_read_cnt = 0;
    static uint16_t fan_rotate_cnt = 0;
    static uint16_t alarm_temp_flick_cnt = 0;
    static uint16_t modbus_04_scan_cnt = 0;
    static uint16_t modbus_04_rcv_over_cnt = 0;
    /* 1, 键值扫描           */
    key_scan_cnt++;
    if( key_scan_cnt >= 5 )
    {
        key_value  = (P2 & 0X0F);
        key_scan_cnt = 0;
    }

    /* 2, 按键有效间隔         */
    if( key.key_read_allow == 0 )
    {
        key_read_cnt++;
        if( key_read_cnt == 20 )
        {
            key_read_cnt = 0;
            key.key_read_allow = 1;
        }
    }
    /* 3, 风扇旋转           */
    fan_rotate_cnt++;
    if(fan_rotate_cnt == 15)
    {
        lcd_info.fan_rotate_flag = 1 - lcd_info.fan_rotate_flag;
        fan_rotate_cnt = 0;
    }

    /* 4, 04查询           */
    modbus_04scan_cnt++;
    if( modbus_04scan_cnt == 100 )
    {
        modbus.scan_flag_04 = 1;
        modbus_04scan_cnt = 0;
    }

    /* 5, 报警温度闪烁           */
    alarm_temp_flick_cnt++;
    if( alarm_temp_flick_cnt == 50 )
    {
        lcd_info.alarm_temp_flick_flag = 1 - lcd_info.alarm_temp_flick_flag;
        alarm_temp_flick_cnt = 0;
    }

    /* 6, 按键操作忙           */
    if( key_value == 15 )
    {
        modbus_04_scan_cnt++;
        if(( modbus_04_scan_cnt == 500 ) && ( lcd_info.power_on == 1 ))
        {
            modbus.scan_flag_04_allow = 1;
        }
    }else
    {
        modbus_04_scan_cnt = 0;
        modbus.scan_flag_04_allow = 0;
    }

    if( modbus.modbus_04_rcv_over == 0 )
    {
        modbus_04_rcv_over_cnt++;
        if( modbus_04_rcv_over_cnt == 2 )
        {
            modbus.modbus_04_rcv_over = 1;
            modbus_04_rcv_over_cnt = 0;
        }
    }
}
