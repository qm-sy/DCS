#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer3：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19
{
    static uint8_t  signal_up_cnt   = 0;
    static uint8_t  signal_down_cnt = 0;

    static uint16_t temp_scan_cnt = 0;

    static uint16_t OTP1_cnt = 0;
    static uint16_t OTP2_cnt = 0;
    static uint8_t Buzzer_status = 1;
    static uint16_t fan_delay_cnt = 0;

    /* 1, Signal_IN状态查询 + 防抖      */
    if( Signal_IN == 0 )
    {
        signal_up_cnt++;
        signal_down_cnt = 0;
        if( signal_up_cnt == 5 )    //50ms防抖处理
        {
            signal_up_cnt = 0;
            power_ctrl.signal_flag = 1;
        }
    }else
    {
        signal_down_cnt++;
        signal_up_cnt = 0;
        if( signal_down_cnt == 5 )
        {
            signal_down_cnt = 0;
            power_ctrl.signal_flag = 0;
        }
    }

    /* 2, temp 上报 1s/次                             */
    if( temp.temp_scan_flag == 0 )
    {
        temp_scan_cnt++;
        if( temp_scan_cnt == 100 )
        {
            temp_scan_cnt = 0;
            temp.temp_scan_flag = 1;
        }
    }
    
    /* 3, 过温保护蜂鸣器叫声控制                        */
    if( power_ctrl.OTP1_alarm_flag == 1 )   //过温保护1
    {
        OTP1_cnt++;
        if( OTP1_cnt == 50 )
        {
            Buzzer_status = 1- Buzzer_status;
            if( Buzzer_status == 0 )
            {
                Buzzer = BUZZER_ON;
            }else
            {
                Buzzer = BUZZER_OFF;
            }
            OTP1_cnt = 0;
        }
    }

    if( power_ctrl.OTP2_alarm_flag == 1 )   //过温保护2
    {
        OTP2_cnt++;
        if( OTP2_cnt == 200 )
        {
            OTP2_cnt = 1;   // 重置计数器 
        }

        // 判断是否在蜂鸣器开启的区间
        if( (OTP2_cnt >= 20 && OTP2_cnt <= 39) || 
            (OTP2_cnt >= 60 && OTP2_cnt <= 79) || 
            (OTP2_cnt >= 100 && OTP2_cnt <= 119) )
        {
            Buzzer = BUZZER_ON;
        }
        else
        {
            Buzzer = BUZZER_OFF;
        }
    }

    /* 4, 风扇延时关闭                        */
    if( power_ctrl.fan_delay_flag == 1 )
    {
        fan_delay_cnt++;
        if( fan_delay_cnt == 1500 )
        { 
            fan_delay_cnt = 0;
            power_ctrl.FAN_enable = 0;
            power_ctrl.fan_delay_flag = 0;
        }
    }else
    {
        fan_delay_cnt = 0;
    }
}
