#include "power_crl.h"

AC_DC ac_dc;
/**
 * @brief	移相触发调用结构体初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_Statu_Init( void )
{
    ac_dc.zero_flag  = 0;
    temp.temp_scan_flag = 0;
    AC_Out1 = AC_Out2 = AC_Out3 = 1;
    ac_dc.connect_flag = 0;
    ac_dc.alarm_flag = 0;
    ac_dc.all_ctrl_flag = 1;
    ac_dc.buzzer_call_flag1 = 0;
    ac_dc.buzzer_call_flag2 = 0;
    ac_dc.fan_delay_flag = 0;
    ac_dc.mode_set_flag = 0;
}

/**
 * @brief	外部中断0中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void INT0_ISR( void ) interrupt 0
{
    /* 1, 检测到外部中断后，等待THL\TLI后触发TIM1中断       */
    TL1 = ac_dc.time_delay;				
	TH1 = ac_dc.time_delay >> 8;				

    ac_dc.zero_flag = 1;

    /* 2, 定时器1开始计时，打开TIM1中断                     */
    TR1 = 1;				     
    ET1 = 1; 
}

/**
 * @brief	Timer1中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Tim1_ISR( void ) interrupt 3   //10ms
{
    /* 1, 中断触发后，power_ch电平 由高电平变为低电平           */
    if( ac_dc.zero_flag == 1 )
    {
        ac_dc.zero_flag = 0;

         /* 2, 温度允许，使能为1时可开启输出          */
        AC_Out1 = 1 - ac_dc.ac_out1_flag;
        AC_Out2 = 1 - ac_dc.ac_out2_flag;
        AC_Out3 = 1 - ac_dc.ac_out3_flag;

         /* 3, 设置下一次Timer1中断触发所需时间，即脉冲时间       */
        TL1 = 0xF7;				
        TH1 = 0xFF;				
    }else
    {
        /* 2, 下一次进入Timer1中断，power_ch电平 由低电平变为高电平，完成一次10us脉冲，即斩波*/
        AC_Out1 = AC_Out2 = AC_Out3 = 1;

        /* 3, 定时器1停止计时，关闭TIM1中断，等待下一次外部中断     */
        TR1 = 0;				    
        ET1 = 0; 
    }
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void ac_220v_crl( uint8_t power_level )
{
    ac_dc.time_delay = 58000 + 74*power_level;
}


/**
 * @brief	24V PWM风扇档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan_ctrl( uint8_t level )
{
    PWMB_CCR7 = level * 184;
    PWMB_CCR8 = level * 184;
}

/**
 * @brief	同步控制函数：无外部信号时 PWM风扇和220V输出关闭
 *
 * @param   
 *
 * @return  void
**/
void sync_ctrl( void )
{
    if( ac_dc.sync_flag == 1 )
    {
        extern_24_listen();
        if( ac_dc.signal_in_flag == 1 )
        {
            EX0 = 1;            //外部中断控制
            PWMB_BKR = 0x80;    //PWM控制
            ac_dc.fan_delay_flag = 0;
        }else
        {   
            EX0 = 0;
        }
    }else
    {
        EX0 = 1;            //外部中断控制
        PWMB_BKR = 0x80;    //PWM控制
        ac_dc.fan_delay_flag = 0;
    }
}

/**
 * @brief 温度扫描，DHT11温湿度扫描 1s/次 控制220V输出使能
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void temp_scan( void )
{
    if( temp.temp_scan_flag == 1)
    {
        temp.temp_value1 =  get_temp(NTC);

        if( temp.temp_value1 >= 45 )
        {
            FAN_TMEP = 1;
        }
        if( temp.temp_value1 <= 35 )
        {
            FAN_TMEP = 0;
        }
        if( temp.temp_value1 >= ac_dc.alarm_temp_val )
        {
            ac_dc.alarm_flag = 1;
        }else
        {
            ac_dc.alarm_flag = 0;
        }
        if( temp.temp_value1 >= 120 )
        {
            EX0 = 0;
            ac_dc.all_ctrl_flag = 0;
        }
        temp.temp_scan_flag = 0;
    }
}

void channel_close( void )
{
    switch (ac_dc.channel_num)
    {
        case 1:
        ac_dc.ac_out1_flag = 1;
        ac_dc.ac_out2_flag = ac_dc.ac_out3_flag = 0;
        break;

        case 2:
        ac_dc.ac_out2_flag = 1;
        ac_dc.ac_out1_flag = ac_dc.ac_out3_flag = 0;
        break;

        case 3:
        ac_dc.ac_out3_flag = 1;
        ac_dc.ac_out1_flag = ac_dc.ac_out2_flag = 0;
        break;

        case 4:
        ac_dc.ac_out1_flag = ac_dc.ac_out2_flag = 1;
        ac_dc.ac_out3_flag = 0;
        break;

        case 5:
        ac_dc.ac_out1_flag = ac_dc.ac_out3_flag = 1;
        ac_dc.ac_out2_flag = 0;
        break;

        case 6:
        ac_dc.ac_out2_flag = ac_dc.ac_out3_flag = 1;
        ac_dc.ac_out1_flag = 0;
        break;

        case 7:
        ac_dc.ac_out1_flag = ac_dc.ac_out2_flag = ac_dc.ac_out3_flag = 1;
        break;

    default:
        break;
    }
}

void extern_24_listen( void )
{
    static uint8_t now_val = 1;
    if(now_val != ac_dc.signal_in_flag)
    {
        now_val = ac_dc.signal_in_flag;
        if( now_val == 0 )
        {
            ac_dc.fan_delay_flag = 1;
        }else
        {
            ac_dc.fan_delay_flag = 0;
        }
    }
}              