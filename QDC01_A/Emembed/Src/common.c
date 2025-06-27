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

    static uint16_t temp_scan_cnt = 0;
    static uint16_t buzzer_cnt1 = 0;
    static uint16_t buzzer_cnt2 = 0;
    static uint8_t buzzer1_status = 1;
    static uint16_t sync_delay_cnt = 0;
    /* 1, Signal_IN状态查询           */
    if( Signal_IN == 0 )
    {
        ac_dc.signal_in_flag = 1;
    }else
    {
        ac_dc.signal_in_flag = 0;
    }

    /* 2, temp 上报 1s/次                             */
    if( temp.temp_scan_flag == 0 )
    {
        temp_scan_cnt++;
        if( temp_scan_cnt == 100 )
        {
            temp.temp_scan_flag = 1;
            temp_scan_cnt = 0;
        }
    }
    
    if( ac_dc.buzzer_call_flag1 == 1 )
    {
        buzzer_cnt1++;
        if( buzzer_cnt1 == 50 )
        {
            buzzer1_status = 1- buzzer1_status;
            if( buzzer1_status == 0 )
            {
                Buzzer = 0;
            }else
            {
                Buzzer = 1;
            }
            buzzer_cnt1 = 0;
        }
    }
    if( ac_dc.buzzer_call_flag2 == 1 )
    {
        buzzer_cnt2++;
        if( buzzer_cnt2 == 20 )
        {
            Buzzer = 0;
        }
        if( buzzer_cnt2 == 40 )
        {
            Buzzer = 1;
        }
        if( buzzer_cnt2 == 60 )
        {
            Buzzer = 0;
        }
        if( buzzer_cnt2 == 80 )
        {
            Buzzer = 1;
        }
        if( buzzer_cnt2 == 100 )
        {
            Buzzer = 0;
        }
        if( buzzer_cnt2 == 120 )
        {
            Buzzer = 1;
        }
        if( buzzer_cnt2 == 200 )
        {
            buzzer_cnt2 = 0;
        }
    }

    if( ac_dc.fan_delay_flag == 1 )
    {
        sync_delay_cnt++;
        if( sync_delay_cnt == 1500 )
        {
            PWMB_BKR = 0x00; 
            sync_delay_cnt = 0;
            ac_dc.fan_delay_flag = 0;
        }
    }else
    {
        sync_delay_cnt = 0;
    }
}
