#include "key.h"

KEY key;

volatile uint8_t key_value = 0;

/*
    key1        key6
    key2        key5
    key3        key4
*/
void key_scan( void )
{
    uint8_t key_continue_cnt = 100;

    if( key.key_read_allow == 1 )
    {
        switch (key_value)
        {
            case PowerKey:                                      //key6  短按-开     长按-关
                while((key_value == PowerKey) && (key_continue_cnt > 0))
                {
                    key_continue_cnt--;
                    delay_ms(10);
                }

                if( key_continue_cnt == 0 )
                {
                    Power_Ctrl(POWER_OFF);
                }else
                {
                    Power_Ctrl(POWER_ON);
                }

                break;

            case Channel:                                       //key1  短按-通道选择   长按-温度设置
                while((key_value == Channel) && (key_continue_cnt > 0))
                {
                    key_continue_cnt--;
                    delay_ms(10);
                }
                if( key_continue_cnt == 0 )
                {
                    alarm_temp_set();
                }else
                {
                    channel_ctrl();
                }

                break;
                
            case Mode:                                          //key2  短按-切换模式     长按-同步开/关
                while((key_value == Mode) && (key_continue_cnt > 0))
                {
                    key_continue_cnt--;
                    delay_ms(10);
                }
                if( key_continue_cnt == 0 )
                {
                    sync_ctrl();
                }else
                {
                    mode_ctrl();
                }

                break;

            case Num_Up:    power_temp_set(Num_Up);     break;  //key5          温度/功率 ++
                
            case Num_Down:  power_temp_set(Num_Down);   break;  //key4          温度/功率 --

            case FanUp:     fan_level_ctrl(FanUp);      break;  //key3 + key5   风速++
                
            case FanDown:   fan_level_ctrl(FanDown);    break;  //key3 + key4   风速--

            default:                                    break;
        }
        key.key_read_allow = 0;
    }
}

void Power_Ctrl( uint8_t power_switch )
{
    if( power_switch == POWER_ON )
    {
        led_status_ctrl(LED_OPEN);
        lcd_info.Power_Swtich = 1;
        write_slave_06(0x06,0x00,lcd_info.Power_Swtich);
    }else
    {
        led_status_ctrl(LED_SLEEP);
        lcd_info.Power_Swtich = 0;
        write_slave_06(0x06,0x00,lcd_info.Power_Swtich);
        delay_ms(1000);                             //避免误触再次开启
    }
}

void power_temp_set( uint8_t key_val )
{
    if( key_val == Num_Up )
    {
        if( lcd_info.power_level < 100 )
        {
            lcd_info.power_level += 5;
        }
    }
    if( key_val == Num_Down )
    {
        if( lcd_info.power_level > 0 )
        {
            lcd_info.power_level -= 5;
        }
    }
    num_dis(lcd_info.power_level);
    write_slave_06(0x03,0x00,lcd_info.power_level);
}

void fan_level_ctrl( uint8_t key_val )
{
    if( key_val == FanUp )
    {
        if( lcd_info.fan_level < 6 )
        {
            lcd_info.fan_level++;
        }
    }

    if( key_val == FanDown )
    {
        if( lcd_info.fan_level > 0 )
        {
            lcd_info.fan_level--;
        }    
    }

    wind_dis(lcd_info.fan_level);
    write_slave_06(0x02,0x00,lcd_info.fan_level);
}

void channel_ctrl( void )
{
    if(lcd_info.channel_num == 7)
    {
        lcd_info.channel_num = 1;
    }else
    {
        lcd_info.channel_num += 1;
    }
    channel_dis(lcd_info.channel_num);
    write_slave_06(0x00,0x00,lcd_info.channel_num);
}

void mode_ctrl( void )
{
    if( lcd_info.mode_num == 5 )
    {
        lcd_info.mode_num = 1;
    }else
    {
        lcd_info.mode_num += 1;
    }

    screen_clear();                 //一次清屏
    mode_dis(DIS_ON);               //显示模式序号和标志
    num_dis(lcd_info.mode_num);     
    delay_ms(500);
    screen_clear();
    write_slave_06(0x05,0x00,lcd_info.mode_num);
}

void alarm_temp_set( void )
{
    screen_clear();
    delay_ms(1000);
    while(key_value != Channel)
    {
        Celsius_dis(DIS_ON);
        if( lcd_info.alarm_temp_flick_flag == 1 )
        {
            num_dis(lcd_info.OTP_temp1);
        }else
        {
            screen_write_val(addr_tab[ADDR_0B],0x00);
            screen_write_val(addr_tab[ADDR_0A],0X00);
            screen_write_val(addr_tab[ADDR_07],0X00);
            screen_write_val(addr_tab[ADDR_06],0X00);
        }

        if( key_value == Num_Up )
        {
            if( lcd_info.OTP_temp1 < 120 )
            {
                lcd_info.OTP_temp1 += 1;
                write_slave_06(0x04,0x00,lcd_info.OTP_temp1);
                delay_ms(300);
            }
        }

        if( key_value == Num_Down )
        {
            if( lcd_info.OTP_temp1 > 40 )
            {
                lcd_info.OTP_temp1 -= 1;
                write_slave_06(0x04,0x00,lcd_info.OTP_temp1);
                delay_ms(300);
            }
        }
    }
    
    screen_all_dis();
    delay_ms(500);
}

void sync_ctrl( void )
{
    lcd_info.sync_switch = 1 - lcd_info.sync_switch;
    sync_dis(lcd_info.sync_switch);
    write_slave_06(0x01,0x00,lcd_info.sync_switch);
    delay_ms(1000);
}