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
            if( rs485.rcv_buf[0] == MY_ADDR )
            {
                switch ( rs485.rcv_buf[1] )
                {
                    case FUN_03:      Modbus_Fun3();          break;
                    case FUN_04:      Modbus_Fun4();          break;
                    case FUN_06:      Modbus_Fun6();          break;
                    case FUN_16:      Modbus_Fun16();         break;

                    default:                                  break;
                }
            }
        }
        /*4.清空接收完毕标志位  清空接收计数        */    
        rs485.rcv_end_Flag = 0;
        rs485.rcv_cnt = 0; 
    }
}

/**
 * @brief	读输入寄存器  03
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun3( void )
{
    uint16_t i;

    modbus.rcv_addr1_valH  = 3;                //DATA1 H 位置
    modbus.start_reg_03    = rs485.rcv_buf[2] << 8 | rs485.rcv_buf[3];
    modbus.reg_num_03      = rs485.rcv_buf[4] << 8 | rs485.rcv_buf[5];

    rs485.send_buf[0]  = MY_ADDR;                  //Addr
    rs485.send_buf[1]  = FUN_03;                   //Fun
    rs485.send_buf[2]  = modbus.reg_num_03 * 2;    //Byte Count

    for( i = modbus.start_reg_03; i < modbus.start_reg_03 + modbus.reg_num_03; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {   
            /*  40001 通道查询                      */
            case 0x00:
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.channel_num;
                break;

            /*  40002 同步状态查询                  */    
            case 0x01:
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.sync_switch;
                break;

            /*  40003 风速查询                      */
            case 0x02:    
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.fan_level;

                break;

            /*  40004 功率查询                      */
            case 0x03:    
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.power_level;

                break;

            /*  40005 报警温度查询                  */
            case 0x04:   
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.OTP_temp1;

                break;

            /*  40006 模式查询                      */
            case 0x05:   
                modbus.byte_info_H = 0;
                modbus.byte_info_L = slave_06.mode_num;

                break;

            default:
                break;
        }
        rs485.send_buf[modbus.rcv_addr1_valH++] = modbus.byte_info_H;
        rs485.send_buf[modbus.rcv_addr1_valH++] = modbus.byte_info_L;
    }
    slave_to_master(FUN_03,3 + modbus.reg_num_03 * 2);
}

/**
 * @brief	读输出寄存器  04
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun4( void )
{
    uint16_t i;

    modbus.rcv_addr1_valH  = 3;                 //DATA1 H 位置
    modbus.start_reg_04 = rs485.rcv_buf[2] << 8 | rs485.rcv_buf[3];
    modbus.reg_num_04   = rs485.rcv_buf[4] << 8 | rs485.rcv_buf[5];

    rs485.send_buf[0]  = MY_ADDR;                  //Addr
    rs485.send_buf[1]  = FUN_04;                   //Fun
    rs485.send_buf[2]  = modbus.reg_num_04 * 2;    //Byte Count

    for( i = modbus.start_reg_04; i < modbus.start_reg_04 + modbus.reg_num_04; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {
            case 0:
                modbus.byte_info_H = 0x00;
                modbus.byte_info_L = power_ctrl.OTP1_alarm_flag;

                break;

            case 1:
                modbus.byte_info_H = 0x00;
                modbus.byte_info_L = power_ctrl.signal_flag;

                break;
            default:
                break;
        }
        rs485.send_buf[modbus.rcv_addr1_valH++] = modbus.byte_info_H;
        rs485.send_buf[modbus.rcv_addr1_valH++] = modbus.byte_info_L;
    }
    slave_to_master(FUN_04,3 + modbus.reg_num_04 * 2);
}

/**
 * @brief	写单个输出寄存器  06
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun6( void )
{
    Buzzer = 0;
    modbus.reg_addr_06 = rs485.rcv_buf[2] << 8 | rs485.rcv_buf[3];
    modbus.byte_info_H = rs485.rcv_buf[4];
    modbus.byte_info_L = rs485.rcv_buf[5];

    switch (modbus.reg_addr_06)
    {
        /*  40001   加热通道设置                */
        case 0x00:       
            slave_06.channel_num = modbus.byte_info_L;        
            break;

        /*  40002   同步开关设置                */
        case 0x01:
            slave_06.sync_switch = modbus.byte_info_L;                                         
            break;

        /*  40003   风速设置                    */
        case 0x02:                                         
            slave_06.fan_level = modbus.byte_info_L;
            fan_ctrl();
            break;  
            
        /*  40004   功率档位设置                */
        case 0x03:                                         
            slave_06.power_level = modbus.byte_info_L;
            power_level_crl();
            break;

        /*  40005   报警温度设置                */
        case 0x04:
            slave_06.OTP_temp1 = modbus.byte_info_L;                                         
            break;

        /*  40006   模式设置                    */
        case 0x05:                                         
            slave_06.mode_num = rs485.rcv_buf[5];
            mode_select();
            break;

        /*  40007   总开关设置                  */
        case 0x06:  
            power_ctrl.Power_Swtich = rs485.rcv_buf[5];
            break;

        default:
            break;   
    }
    slave_to_master(FUN_06,8);
    
    eeprom_data_write();
    Buzzer = 1;
}

/**
 * @brief	写多个输出寄存器  16
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun16( void )
{
    uint16_t i;

    Buzzer = 0;
    modbus.rcv_addr1_valH = 7;                  //DATA1 H位置
    modbus.start_reg_16 = rs485.rcv_buf[2] << 8 | rs485.rcv_buf[3];
    modbus.reg_num_16   = rs485.rcv_buf[5] << 8 | rs485.rcv_buf[6];

    for( i = modbus.rcv_addr1_valH; i < modbus.rcv_addr1_valH + modbus.reg_num_16; i++)
    {
        modbus.byte_info_H = rs485.rcv_buf[modbus.rcv_addr1_valH];
        modbus.byte_info_L = rs485.rcv_buf[modbus.rcv_addr1_valH + 1];
        switch (i)
        {
            /*  40001  通道设置                 */
            case 0x00:
                slave_06.channel_num = modbus.byte_info_L;
                break;
            
            /*  40002   同步状态设置                          */
            case 0x01:
                slave_06.sync_switch = modbus.byte_info_L;
                break;

            /*  40003 风速设置                          */
            case 0x02:
                slave_06.fan_level = modbus.byte_info_L;
                fan_ctrl();
                break;

            /*  40004  功率设置                   */
            case 0x03:
                slave_06.power_level = modbus.byte_info_L;
                power_level_crl();
                break;

            /*  40005  报警温度设置                   */
            case 0x04:                                         
                slave_06.OTP_temp1 = modbus.byte_info_L;
                break;

            /*  40006  模式设置                   */
            case 0x05:                                         
                slave_06.mode_num = modbus.byte_info_L;
                mode_select();
                break;
                
            default:
                break;
        }
        modbus.rcv_addr1_valH += 2;         //从Value1_H →→ 从Value2_H
    }

    slave_to_master(FUN_16,8);

    eeprom_data_write();                      //记录更改后的值
    Buzzer = 1;
}


/**
 * @brief	从机回复主机
 *  
 * @param   code_num:功能码       
 * @param   length:数据长度        
 * 
  @return  crc16:crc校验的值 2byte
 */
void slave_to_master(uint8_t code_num,uint8_t length)
{
    uint16_t crc;

    switch (code_num)
    {
        case 0x03:
            crc = MODBUS_CRC16(rs485.send_buf,length);

            rs485.send_buf[length + 1] = crc;               //CRC H
            rs485.send_buf[length]     = crc >> 8;          //CRC L

            rs485.send_bytelength = length + 2;
            
            break;
        case 0x04:
            crc = MODBUS_CRC16(rs485.send_buf,length);

            rs485.send_buf[length + 1] = crc;               //CRC H
            rs485.send_buf[length]     = crc >> 8;          //CRC L

            rs485.send_bytelength = length + 2;
            
            break;    

        case 0x06:
            memcpy(rs485.send_buf,rs485.rcv_buf,8);

            rs485.send_bytelength = length;
            
            break;   

        case 0x10:
            memcpy(rs485.send_buf,rs485.rcv_buf,6);
        
            crc = MODBUS_CRC16(rs485.send_buf,6);

            rs485.send_buf[7] = crc;                 //CRC H
            rs485.send_buf[6] = crc >> 8;              //CRC L
        
            rs485.send_bytelength = length;
            
            break;         

        default:
            break;
    }

    DR_485 = 1;                                 //485可以发送
    delay_ms(2);
    S4CON |= S4TI;                              //开始发送
    delay_ms(1);
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
