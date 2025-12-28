#include "power_crl.h"

AC_DC ac_dc;
SLAVE_06 slave_06;
POWER_CTRL power_ctrl;

/**
 * @brief	移相触发调用结构体初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_Statu_Init( void )
{
    power_ctrl.zero_flag = 0;
    temp.temp_scan_flag  = 0;
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
    TL1 = power_ctrl.time_delay;				
	TH1 = power_ctrl.time_delay >> 8;				

    power_ctrl.zero_flag = 1;

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
    if( power_ctrl.zero_flag == 1 )
    {
        power_ctrl.zero_flag = 0;

         /* 2, 温度允许，使能为1时可开启输出          */
        AC_Out1 = 1 - power_ctrl.AC1_enable;
        AC_Out2 = 1 - power_ctrl.AC2_enable;
        AC_Out3 = 1 - power_ctrl.AC3_enable;

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
void power_level_crl( void )
{
    power_ctrl.time_delay = 58000 + 74 * slave_06.power_level;
}


/**
 * @brief	24V PWM风扇档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan_ctrl( void )
{
    PWMB_CCR7 = PWMB_CCR8 = slave_06.fan_level * 184;
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
    if( temp.temp_scan_flag == 1 )
    {
        temp.NTC1_value =  get_temp(NTC);

        if( temp.NTC1_value >= FAN_START_TEMP )      //小风扇启动温度
        {
            FAN_TMEP = 1;
        }else
        {
            FAN_TMEP = 0;
        }

        if( temp.NTC1_value >= power_ctrl.OTP_temp2 )    //过温保护2  蜂鸣器报警 滴-滴-滴-滴滴
        {
            power_ctrl.OTP2_alarm_flag = 1;
            power_ctrl.OTP1_alarm_flag = 0;
        }
        if( temp.NTC1_value < (power_ctrl.OTP_temp2 - 5) )
        {
            power_ctrl.OTP2_alarm_flag = 0;

            if( temp.NTC1_value >= slave_06.OTP_temp1 )    //过温保护1  蜂鸣器报警 滴-滴
            {
                power_ctrl.OTP1_alarm_flag = 1;
            }
            if( temp.NTC1_value < (slave_06.OTP_temp1 - 2))
            {
                power_ctrl.OTP1_alarm_flag = 0;
                Buzzer = BUZZER_OFF;
            }
        }

        temp.temp_scan_flag = 0;
    }
}

void mode_select( void )
{
    eeprom_mode_write();
    eeprom_data_read();
    eeprom_data_init();
}

void fan_delay_ctrl( void )
{
    static uint8_t signal_flag = 0;

    if( signal_flag != power_ctrl.signal_flag )
    {
        signal_flag = power_ctrl.signal_flag;

        power_ctrl.fan_delay_flag = (( slave_06.sync_switch == 1 ) && ( signal_flag == 0 )) ? 1 : 0;
    }
}              

/**
 * @brief AC DC输出使能控制
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void ac_dc_ctrl( void )
{
    if( power_ctrl.Power_Swtich == 1 )
    {
        power_ctrl.AC1_enable = (ACout_enable_judge() & (slave_06.channel_num & 0x01))? ENABLE : DISABLE;         // 根据函数返回值设置使能状态
        power_ctrl.AC2_enable = (ACout_enable_judge() & (slave_06.channel_num & 0x02))? ENABLE : DISABLE;         // 根据函数返回值设置使能状态
        power_ctrl.AC3_enable = (ACout_enable_judge() & (slave_06.channel_num & 0x04))? ENABLE : DISABLE;         // 根据函数返回值设置使能状态

        power_ctrl.FAN_enable = (slave_06.sync_switch      == 0 ||    //同步关闭
                                 power_ctrl.signal_flag    == 1 ||    //有24V信号进来
                                 power_ctrl.fan_delay_flag == 1)      //风扇延时输出信号
                                 ? ENABLE : DISABLE;
    }else
    {
        power_ctrl.AC1_enable = power_ctrl.AC2_enable = power_ctrl.AC3_enable = power_ctrl.FAN_enable = DISABLE;
    }
    
    PWMB_BKR = (power_ctrl.FAN_enable == ENABLE) ? FAN_ON : FAN_OFF;
}


/**
 * @brief AC输出使能判断
 *
 * @param[in]
 * 
 * @return  
 * 
**/
uint8_t ACout_enable_judge( void )                 
{
    // 第1层检查：过温保护2
    // 如果温度超过过温保护阈值，立即返回 0（不加热）
    if ( temp.NTC1_value >= power_ctrl.OTP_temp2 ) 
    {
        return DISABLE;
    }

    // 第2层检查：功率是否为0
    if( slave_06.power_level == 0 )
    {
        return DISABLE;
    }
    
    // 第3层检查：工作模式
    if ( slave_06.sync_switch == SW_OPEN ) 
    {  
        if ( power_ctrl.signal_flag == 0 )         // 同步模式且无信号时，只有在预加热状态下才允许加热
        {
            return DISABLE;
        }
    }
    
    // 其他情况：允许加热
    return ENABLE;
}