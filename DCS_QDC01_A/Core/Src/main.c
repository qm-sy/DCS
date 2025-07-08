#include "main.h"

void main( void )
{
    P_SW2 |= 0x80;
    
    GPIO_Init();

    /*  调试使用 printf  */
    Uart1_Init();
    
    /*  温度控制  */
    ADC_Init();

    /*  485控制  */
    Uart4_Init();               //串口4用作485
    Uart4_Send_Statu_Init();    //接收发送初始化
    Timer0_Init();              //不定长数据接收

    /*  PWM控制  */
    PWM_Init();

    /*  3路220输出控制  */
    Power_Statu_Init();
    INT0_Init();
    Timer1_Init();

    /*  定时事件  */
    Timer3_Init();

    EA = 1;     //中断总开关

    eeprom_statu_judge();       //EEPROM初始化
    PWMB_BKR = 0x00; 
    EX0 = 0;
    FAN_TMEP = 0;
    Buzzer = 1;
    while ( ac_dc.connect_flag == 0 )
    {
        Modbus_Event();
    }
    
    printf("========== code start ========== \r\n");

    while (1)
    {
        if( ac_dc.all_ctrl_flag == 1 )
        {
            if( ac_dc.connect_flag == 1)
            {  
                sync_ctrl();
            }
            if( ac_dc.connect_flag == 0)
            {
                PWMB_BKR = 0x00; 
                EX0 = 0;
            }
            Modbus_Event();
            temp_scan();
        }

        if( ac_dc.all_ctrl_flag == 0)
        {
            ac_dc.buzzer_call_flag2 = 1;
            ac_dc.buzzer_call_flag1 = 0;
        }else
        {
            ac_dc.buzzer_call_flag2 = 0;
            if( ac_dc.alarm_flag == 1 )
            {
                ac_dc.buzzer_call_flag1 = 1;
            }else
            {
                ac_dc.buzzer_call_flag1 = 0;
                Buzzer = 1;
            }
        }
    }  
}