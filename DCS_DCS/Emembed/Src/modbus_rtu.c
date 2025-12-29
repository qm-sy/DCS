#include "modbus_rtu.h"

MODBIS_INFO modbus;

/**
 * @brief	modbus_rtu  无奇偶校验
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Event( void )
{
    uint16_t crc,rccrc;
    
    /*1.接收完毕                                           */
    if( rs485.rcv_end_Flag == 1 )
    {
        /*2.CRC校验                                         */                                     
        crc = MODBUS_CRC16(rs485.rcv_buf, rs485.rcv_cnt-2);
        rccrc = (rs485.rcv_buf[rs485.rcv_cnt - 1]) | (rs485.rcv_buf[rs485.rcv_cnt -2 ] << 8);

        /*3.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*3-1.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485.rcv_buf[0] == MASTER_ADDR )
            {
                switch ( rs485.rcv_buf[1] )
                {
                    case FUN_03:      Modbus_Fun3();          break;
                    case FUN_04:      Modbus_Fun4();          break;
                    case FUN_06:      Modbus_Fun6();          break;

                    default:                                  break;
                }
            }
        }
        /*4.清空接收完毕标志位  清空接收计数        */    
        rs485.rcv_end_Flag = 0;
        rs485.rcv_cnt = 0; 
    }
}

void modbus_params_init( void )
{
    modbus.modbus_04_rcv_over = 1;
    modbus.scan_flag_04 = 0;
    modbus.scan_flag_04_allow = 0;
}

void Modbus_Fun3()
{
    uint16_t i;

    modbus.rcv_addr1_valH = 3;                  //DATA1_H 位置 rcvbuf[3]

    for( i = modbus.start_reg_03; i < modbus.start_reg_03 + modbus.reg_num_03; i++)
    {
        modbus.byte_info_H = rs485.rcv_buf[modbus.rcv_addr1_valH];
        modbus.byte_info_L = rs485.rcv_buf[modbus.rcv_addr1_valH + 1];

        switch (i)
        {
            /*  40001 通道查询                      */
            case 0x00:
                lcd_info.channel_num = modbus.byte_info_L;
                break;

            /*  40002 同步状态查询                  */        
            case 0x01:
                lcd_info.sync_switch = modbus.byte_info_L;
                break;
    
            /*  40003 风速查询                      */    
            case 0x02:
                lcd_info.fan_level = modbus.byte_info_L;

                break; 

            /*  40004 功率查询                      */
            case 0x03:
                lcd_info.power_level = modbus.byte_info_L;

                break;
    
            /*  40005 报警温度查询                  */    
            case 0x04:
                lcd_info.OTP_temp1 = modbus.byte_info_L;
                break;  

            /*  40006 模式查询                      */
            case 0x05:
                lcd_info.mode_num = modbus.byte_info_L;
                break;   
    
            default:
                break;
        }
        modbus.rcv_addr1_valH += 2;
    }
    lcd_info.lcd_connect_flag = 1;

	screen_all_dis();
}


void Modbus_Fun4()
{
    uint16_t i;

    modbus.rcv_addr1_valH = 3;              //DATA1_H 位置 rcvbuf[3]

    for(i = modbus.start_reg_04; i < modbus.start_reg_04 + modbus.reg_num_04; i++)
    {
        modbus.byte_info_H = rs485.rcv_buf[modbus.rcv_addr1_valH];
        modbus.byte_info_L = rs485.rcv_buf[modbus.rcv_addr1_valH + 1];
        switch (i)
        {
            case 0X00:
                lcd_info.OTP1_alarm_flag = modbus.byte_info_L;
                alarm_dis(lcd_info.OTP1_alarm_flag);

                break;

            case 0X01:
                lcd_info.signal_in = modbus.byte_info_L;
                break;

            /*  40001 通道查询                      */
            case 0x02:
                lcd_info.channel_num = modbus.byte_info_L;
                break;

            /*  40002 同步状态查询                  */        
            case 0x03:
                lcd_info.sync_switch = modbus.byte_info_L;
                break;
    
            /*  40003 风速查询                      */    
            case 0x04:
                lcd_info.fan_level = modbus.byte_info_L;

                break; 

            /*  40004 功率查询                      */
            case 0x05:
                lcd_info.power_level = modbus.byte_info_L;

                break;
    
            /*  40005 报警温度查询                  */    
            case 0x06:
                lcd_info.OTP_temp1 = modbus.byte_info_L;
                break;  

            /*  40006 模式查询                      */
            case 0x07:
                lcd_info.mode_num = modbus.byte_info_L;
                break; 

            case 0x08:
                lcd_info.Power_Swtich = modbus.byte_info_L;
                break; 

            default:
                break;
        }
        modbus.rcv_addr1_valH += 2;
    }
    modbus.modbus_04_rcv_over = 0;
    screen_all_dis();
}


void Modbus_Fun6( void )
{
    modbus.rcv_addr1_valH = 4;
    modbus.byte_info_H = rs485.rcv_buf[modbus.rcv_addr1_valH];
    modbus.byte_info_L = rs485.rcv_buf[modbus.rcv_addr1_valH + 1];

    switch (modbus.reg_addr_06)
    {
        case 0X05:
        delay_ms(200); 
        read_slave_03();
        break;
    }
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}

void get_slave_status( void )
{
    if(( modbus.scan_flag_04 == 1) && ( modbus.scan_flag_04_allow == 1 ))
    {
        read_slave_04();
        modbus.scan_flag_04 = 0;
    }
}

/**
 * @brief	读输出寄存器-03
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_03( void )
{
    uint8_t send_buf[8];
    uint16_t crc;

    modbus.start_reg_03 = START_REG_03;
    modbus.reg_num_03   = REG_NUM_03;


    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_03;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = modbus.start_reg_03 >> 8;
    send_buf[3] = modbus.start_reg_03;
    send_buf[4] = modbus.reg_num_03 >> 8;
    send_buf[5] = modbus.reg_num_03;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(rs485.send_buf,send_buf,8);
    
    rs485.send_bytelength = 8;
    DR_485 = 1;                                 //485可以发送
    delay_ms(2);
    S4CON |= S4TI;                              //开始发送
    delay_ms(1);
}

/**
 * @brief	读输入寄存器-04
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_04( void )
{
    uint8_t send_buf[8];
    uint16_t crc;

    modbus.start_reg_04 = START_REG_04;
    modbus.reg_num_04   = REG_NUM_04;

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_04;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = modbus.start_reg_04 >> 8;
    send_buf[3] = modbus.start_reg_04;
    send_buf[4] = modbus.reg_num_04 >> 8;
    send_buf[5] = modbus.reg_num_04;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    
    /*   发送，后使能接收    */
    memcpy(rs485.send_buf,send_buf,8);
    
    rs485.send_bytelength = 8;
    DR_485 = 1;                                 //485可以发送
    delay_ms(2);
    S4CON |= S4TI;                              //开始发送
    delay_ms(1);
}

/**
 * @brief	写单个输出寄存器-06
 *
 * @param   reg_addr：要写的寄存器地址
 *          reg_val： 要写的值
 *
 * @return  void
 */
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L)
{
    uint8_t send_buf[8];
    uint16_t crc;

    while ( modbus.modbus_04_rcv_over == 0 );

    modbus.reg_addr_06 = reg_addr;

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_06;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = modbus.reg_addr_06 >> 8;
    send_buf[3] = modbus.reg_addr_06;
    send_buf[4] = reg_val_H;
    send_buf[5] = reg_val_L;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(rs485.send_buf,send_buf,8);

    rs485.send_bytelength = 8;
    DR_485 = 1;                                 //485可以发送
    delay_ms(2);
    S4CON |= S4TI;                              //开始发送
}
