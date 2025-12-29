#include "tm1722.h"

LCD_INFO lcd_info;

uint8_t value_03 = 0;  //该地址的值
uint8_t value_06 = 0;
uint8_t value_07 = 0;
uint8_t value_0A = 0;
uint8_t value_0B = 0;
uint8_t value_0E = 0;
uint8_t value_0F = 0; 

uint8_t addr_tab[7] = { 0xc3,0xc6,0xc7,0xca,0xcb,0xce,0xcf};

uint8_t num_tab[10] = {0xF5,0x05,0xD3,0x97,0x27,0xB6,0xF6,0x15,0xF7,0xB7};
                     /*  0    1    2    3    4    5    6    7    8    9  */

void screen_init( void )
{
    TM1722_WR = 1;            //端口配置初始化
    TM1722_CS = 1;
    TM1722_DATA = 1;
    TM1722_Write_Byte(0x00); //工作模式
    TM1722_WR = 1;
    TM1722_Write_Byte(0x44);   //固定地址模式
    TM1722_WR = 1;
    
    screen_clear();
    
    screen_display();
}
 
void lcd_info_init( void )
{
    lcd_info.channel_num = 7;
    lcd_info.power_level = 50;
    lcd_info.fan_level = 3;
    lcd_info.sync_switch = 0;
    lcd_info.mode_num  = 1;

    lcd_info.sync_icon_ctrl_flag = 1;
    lcd_info.lcd_connect_flag = 0;
	lcd_info.alarm_temp_flick_flag = 1;
    lcd_info.Power_Swtich = 0;

    key.key_read_allow = 0;
}

void led_status_ctrl( uint8_t status ) 
{
    switch (status)
    {
        case LED_OPEN:
        BL = 0;
        LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED_ON;
        break;
    
        case LED_SLEEP:
        BL = 1;
        LED1 = LED_ON;
        LED2 = LED3 = LED4 = LED5 = LED6 = LED_OFF;
        break;

        default:
            break;
    }
}
void screen_clear( void ) //清显示缓存
{
    Celsius_dis(DIS_OFF);
    Percentage_dis(DIS_OFF);
    mode_dis(DIS_OFF);
    sync_dis(DIS_OFF);
    screen_write_val(addr_tab[ADDR_0B],0x00);
    screen_write_val(addr_tab[ADDR_0A],0X00);
    screen_write_val(addr_tab[ADDR_07],0X00);
    screen_write_val(addr_tab[ADDR_06],0X00);
    sun_dis(DIS_OFF);
    channel_dis(0);
    fan_leaf1_dis(DIS_OFF);
    fan_leaf2_dis(DIS_OFF);
    fan_center_dis(DIS_OFF);
    wind_dis(0);
}

void screen_display( void )  //显示开
{
    TM1722_Write_Byte(0x93); 
    TM1722_WR = 1;
}

void screen_write_val(uint8_t addr,uint8_t dat)
{
    TM1722_Write_Byte(addr); 
    TM1722_Write_Byte(dat); 
    TM1722_WR = 1;
}

void TM1722_Write_Byte(uint8_t dat)
{
  uint8_t i;
         
  delay_us(50); //用于片选信号的延时

  TM1722_WR = 0;     //有效的片选信号
  for(i = 0;i < 8;i++)
  {
    TM1722_CS   = 0;
    TM1722_DATA = dat & 0x01;
    TM1722_CS   = 1;    //时钟上升沿，送入一位数据
    dat >>= 1;      
  }

  delay_us(50);   //用于片选信号的延时
}

void num_dis(uint8_t num)
{
    uint8_t hundreds = num / 100;  // 获取百位数
    uint8_t tens = (num / 10) % 10;  // 获取十位数
    uint8_t ones = num % 10;  // 获取个位数
    
    value_0B &= 0x70;
    value_0A &= 0X00;
    value_07 &= 0X00;
    value_06 &= 0X08;

    if((num<10)&&(num>=0))
    {
        value_06 |= num_tab[ones]; 

        screen_write_val(addr_tab[ADDR_0B],0);
        screen_write_val(addr_tab[ADDR_0A],0);
        screen_write_val(addr_tab[ADDR_07],0);

        screen_write_val(addr_tab[ADDR_06],value_06);  
    }
    else if((num<100)&&(num>=10))
    {
        value_07 |= num_tab[tens];
        value_06 |= num_tab[ones]; 

        screen_write_val(addr_tab[ADDR_0B],0);
        screen_write_val(addr_tab[ADDR_0A],0);

        screen_write_val(addr_tab[ADDR_07],value_07);
        screen_write_val(addr_tab[ADDR_06],value_06);   
    }
    else
    {
        value_0B |= (num_tab[hundreds] >> 4);
        value_0A |= (num_tab[hundreds] & 0x0f);
        value_07 |= num_tab[tens];
        value_06 |= num_tab[ones]; 

        screen_write_val(addr_tab[ADDR_0B],value_0B);
        screen_write_val(addr_tab[ADDR_0A],value_0A);
        screen_write_val(addr_tab[ADDR_07],value_07);
        screen_write_val(addr_tab[ADDR_06],value_06);
    }
}


void wind_dis(uint8_t num)
{
    value_0F &= 0x30;
    
    switch(num)
    {
        case 0: value_0F |= 0x00;   break;
        
        case 1: value_0F |= 0x40;   break;
        
        case 2: value_0F |= 0xc0;   break;
        
        case 3: value_0F |= 0xc8;   break;
        
        case 4: value_0F |= 0xcc;   break;

        case 5: value_0F |= 0xce;   break;
        
        case 6: value_0F |= 0xcf;   break;
        
        default:                    break;
    }
    
    screen_write_val(addr_tab[ADDR_0F],value_0F);
}


void channel_dis(uint8_t num)
{
    value_0E &= 0x88;
    value_0B &= 0x0F;
    
    switch(num)
    {
        case 0: value_0E |= 0x00;   value_0B |= 0x00;   break;

        case 1: value_0E |= 0x04;   value_0B |= 0x40;   break;
        
        case 2: value_0E |= 0x02;   value_0B |= 0x20;   break;
        
        case 3: value_0E |= 0x01;   value_0B |= 0x10;   break;
        
        case 4: value_0E |= 0x06;   value_0B |= 0x60;   break;
        
        case 5: value_0E |= 0x05;   value_0B |= 0x50;   break;
        
        case 6: value_0E |= 0x03;   value_0B |= 0x30;   break;
        
        case 7: value_0E |= 0x07;   value_0B |= 0x70;   break;
        
        default:                                        break;
    }
    
    screen_write_val(addr_tab[ADDR_0E],value_0E);
    screen_write_val(addr_tab[ADDR_0B],value_0B);
}


void sun_dis(uint8_t on_off)
{
    value_0E &= 0xf7;

    if( on_off == DIS_ON )
    {
        value_0E |= 0x08;   
    }
    screen_write_val(addr_tab[ADDR_0E],value_0E);
}

void sync_dis(uint8_t on_off)
{
    value_03 &= 0xef;

    if( on_off == DIS_ON )
    {
        value_03 |= 0x10;   
    }
    screen_write_val(addr_tab[ADDR_03],value_03);
}

void alarm_dis(uint8_t on_off)
{
    value_03 &= 0xdf;

    if( on_off == DIS_ON )
    {
        value_03 |= 0x20;   
    }
    screen_write_val(addr_tab[ADDR_03],value_03);
}


void Celsius_dis(uint8_t on_off)
{
    value_03 &= 0xbf;

    if( on_off == DIS_ON )
    {
        value_03 |= 0x40;   
    }
    screen_write_val(addr_tab[ADDR_03],value_03);
}

void mode_dis(uint8_t on_off)
{
    value_03 &= 0x7f;

    if( on_off == DIS_ON )
    {
        value_03 |= 0x80;   
    }
    screen_write_val(addr_tab[ADDR_03],value_03);
}

void Percentage_dis(uint8_t on_off)
{
    value_06 &= 0xf7;

    if( on_off == DIS_ON )
    {
        value_06 |= 0x08;   
    }
    screen_write_val(addr_tab[ADDR_06],value_06);
}


void fan_center_dis(uint8_t on_off)
{
    value_0F &= 0xdf;

    if( on_off == DIS_ON )
    {
        value_0F |= 0x20;   
    }
    screen_write_val(addr_tab[ADDR_0F],value_0F);
}

void fan_leaf1_dis(uint8_t on_off)
{
    value_0E &= 0xef;

    if( on_off == DIS_ON )
    {
        value_0E |= 0x10;   
    }
    screen_write_val(addr_tab[ADDR_0E],value_0E);
}

void fan_leaf2_dis(uint8_t on_off)
{
    value_0F &= 0xef;
    if( on_off == DIS_ON )
    {
        value_0F |= 0x10;   
    }
    screen_write_val(addr_tab[ADDR_0F],value_0F);
}

void fan_rotate_dis( uint8_t on_off )
{
    fan_center_dis(DIS_ON);
    if( on_off == DIS_ON )
    {
        fan_leaf1_dis(lcd_info.sync_icon_ctrl_flag);
        fan_leaf2_dis(1 - lcd_info.sync_icon_ctrl_flag);
    }else
    {
        fan_leaf1_dis(DIS_ON);
        fan_leaf2_dis(DIS_OFF);
    }
}

void sync_icon_ctrl( void )
{  
    if( lcd_info.sync_switch == 1 )
    {
        sync_dis(DIS_ON);
        if( lcd_info.signal_in == 1 )
        {
            fan_rotate_dis(DIS_ON);
            sun_dis(DIS_ON);
        }else
        {
            fan_rotate_dis(DIS_OFF);
            sun_dis(DIS_OFF);
        }
    }else
    {
        sync_dis(DIS_OFF);
        fan_rotate_dis(DIS_ON);
        sun_dis(DIS_ON);
    }
}

void screen_all_dis( void )
{
    num_dis(lcd_info.power_level);
    wind_dis(lcd_info.fan_level);
    channel_dis(lcd_info.channel_num);
    sync_dis(lcd_info.sync_switch);
    sync_icon_ctrl();
    Percentage_dis(DIS_ON);
}