#include "main.h"

/******************** 主函数 **************************/
void main(void)
{
    P_SW2 |= 0x80;     //扩展寄存器(XFR)访问使能

    GPIO_Init();

	/*  调试使用 printf  */
	Uart1_Init();
	Timer3_Init();

	/*  485通讯  */
	Uart4_Init();
	Uart4_Send_Statu_Init();    //接收发送初始化
    Timer0_Init();              //不定长数据接收
	modbus_params_init();

    EA = 1;     //打开总中断

	lcd_info_init();
	led_status_ctrl(LED_SLEEP);

	delay_ms(1000);

	while(lcd_info.Power_Swtich == 0)
	{
		key_scan();
		delay_ms(10);
	}
	screen_init();

	while ( lcd_info.lcd_connect_flag == 0 ) 
	{
		read_slave_03();
		Modbus_Event();
		delay_ms(50);
	}

	printf("====== code start ====== \r\n");

    while(1)
    {
		Modbus_Event();
		key_scan();
		sync_icon_ctrl();
		get_slave_status();
	}
}

