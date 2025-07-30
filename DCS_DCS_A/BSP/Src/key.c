#include "key.h"

KEY key;

volatile uint8_t key_value = 0;

void key_scan( void )
{
    uint8_t key1_continue_cnt;
    uint8_t key3_continue_cnt;
    uint8_t key5_continue_cnt;

    if( key.key_read_allow == 1 )
    {
        switch (key_value)
        {
        case PowerKey:
            key1_continue_cnt = 100;
            while((key_value == PowerKey) && (key1_continue_cnt > 0))
            {
                key1_continue_cnt--;
                delay_ms(10);
            }
            if( key1_continue_cnt == 0 )
            {
                power_on_off(POWER_OFF);
            }else
            {
                power_on_off(POWER_ON);
            }
            
            key.key_read_allow = 0;

            break;

        case PowerUp:
            if( lcd_info.power_on == 1 )
            {
                power_ctrl(PowerUp);
            } 
               
            key.key_read_allow = 0;

            break;
            
        case PowerDown:
            if( lcd_info.power_on == 1 )
            {
                power_ctrl(PowerDown);
            }
                        
            key.key_read_allow = 0;

            break;

        case FanUp:
            if( lcd_info.power_on == 1 )
            {
                fan_ctrl(FanUp);
            }
                        
            key.key_read_allow = 0;

            break;
            
        case FanDown:
            if( lcd_info.power_on == 1 )
            {
                fan_ctrl(FanDown);
            }
             
            key.key_read_allow = 0;

            break;

        case Channel:
            if( lcd_info.power_on == 1 )
            {
                key3_continue_cnt = 100;
                while((key_value == Channel) && (key3_continue_cnt > 0))
                {
                    key3_continue_cnt--;
                    delay_ms(10);
                }
                if( key3_continue_cnt == 0 )
                {
                    alarm_temp_set();
                }else
                {
                    if( lcd_info.alarm_set_flag == 1 )
                    {
                        lcd_info.alarm_set_flag = 0;
                    }else
                    {
                        channel_ctrl();
                    }
                }
            }
                      
            key.key_read_allow = 0;

            break;
            
        case Mode:
            if( lcd_info.power_on == 1 )
            {
                key5_continue_cnt = 100;
                while((key_value == Mode) && (key5_continue_cnt > 0))
                {
                    key5_continue_cnt--;
                    delay_ms(10);
                }
                if( key5_continue_cnt == 0 )
                {
                    sync_ctrl();
                }else
                {
                    mode_ctrl();
                }
            }
                      
            key.key_read_allow = 0;

            break;

        default:
            break;
        }
    }
}

void power_on_off( uint8_t key_ctrl )
{
    if( key_ctrl == POWER_ON )
    {
        if( lcd_info.power_on == 0 )
        {
            led_status(LED_OPEN);
            lcd_info.power_on = 1;
            slave_work_ctrl();
        }
    }else
    {
            led_status(LED_SLEEP);
            lcd_info.power_on = 0;
            slave_sleep_ctrl();                   //开始发送
            delay_ms(1000);                             //避免误触再次开启
    }

}

void power_ctrl( uint8_t key_ctrl )
{
    if( key_ctrl == PowerUp )
    {
        if( lcd_info.alarm_set_flag == 0 )
            {
                if( lcd_info.power_level < 100 )
                {
                    lcd_info.power_level += 5;
                }
                num_dis(lcd_info.power_level);
            }else
            {
                if( lcd_info.alarm_temp_val < 120 )
                {
                    lcd_info.alarm_temp_val += 1;
                }
                num_dis(lcd_info.alarm_temp_val);
            }
    }else
    {
        if( lcd_info.alarm_set_flag == 0 )
        {
            if( lcd_info.power_level > 0 )
            {
                lcd_info.power_level -= 5;
            }

            num_dis(lcd_info.power_level);
        }else
        {
            if( lcd_info.alarm_temp_val > 40 )
            {
                lcd_info.alarm_temp_val -= 1;
            }
            num_dis(lcd_info.alarm_temp_val);
        }
        
    }
    send_to_slave_16();
}

void fan_ctrl( uint8_t key_ctrl )
{
    if( key_ctrl == FanUp )
    {
        if( lcd_info.fan_level < 6 )
        {
            lcd_info.fan_level++;
        }

        wind_dis(lcd_info.fan_level);
        

    }else
    {
        if( lcd_info.fan_level > 0 )
        {
            lcd_info.fan_level--;
        }

        wind_dis(lcd_info.fan_level);
    }
    send_to_slave_16();
}

void channel_ctrl( void )
{
    if(lcd_info.channel_num == 7)
    {
        lcd_info.channel_num = 1;
    }
    else
    {
        lcd_info.channel_num += 1;
    }
    channel_dis(lcd_info.channel_num);
    send_to_slave_16();
}

void mode_ctrl( void )
{
    if(lcd_info.mode_num < 6)
    {
        lcd_info.mode_num++;
		if( lcd_info.mode_num == 6 )
		{
			lcd_info.mode_num = 1;
		}
    }
    
    screen_clear();                //一次清屏

    mode_dis(DIS_ON);       
    num_dis(lcd_info.mode_num);           //显示模式序号和标志
         
    delay_ms(500);
    
    screen_clear();                 //二次清屏
    
    mode_dis(DIS_OFF);

    sync_dis(lcd_info.sync_flag);
    num_dis(lcd_info.power_level);
    channel_dis(lcd_info.channel_num);
    wind_dis(lcd_info.fan_level);       
	percentage_dis(DIS_ON);  

    send_to_slave_06();
}

void alarm_temp_set( void )
{
    send_to_slave_16();
    delay_ms(500);
    while(key_value != Channel)
    {
        if( lcd_info.alarm_temp_flick_flag == 1 )
        {
            percentage_dis(0);
            num_dis(lcd_info.alarm_temp_val);
            Celsius_dis(DIS_ON);
        }else
        {
            screen_write_val(addr_tab[ADDR_0F],0X00);
            screen_write_val(addr_tab[ADDR_0E],0X00);
            screen_write_val(addr_tab[ADDR_0B],0x00);
            screen_write_val(addr_tab[ADDR_0A],0X00);
            screen_write_val(addr_tab[ADDR_07],0X00);
            screen_write_val(addr_tab[ADDR_06],0X00);
            screen_write_val(addr_tab[ADDR_03],0x40);   
            sync_dis(0); 
            delay_ms(5);
        }

        if( key_value == PowerUp )
        {
            if( lcd_info.alarm_temp_val < 120 )
            {
                lcd_info.alarm_temp_val += 1;
            }
            send_to_slave_16();
            delay_ms(300);
        }
        if( key_value == PowerDown )
        {
            if( lcd_info.alarm_temp_val > 40 )
            {
                lcd_info.alarm_temp_val -= 1;
            }
            send_to_slave_16();
            delay_ms(300);
        }
    }
    send_to_slave_16();
    screen_all_dis();
    
    delay_ms(1000);
}
void sync_ctrl( void )
{
    lcd_info.sync_flag = 1 - lcd_info.sync_flag;
    sync_dis(lcd_info.sync_flag);
    send_to_slave_16();
    delay_ms(1000);
}