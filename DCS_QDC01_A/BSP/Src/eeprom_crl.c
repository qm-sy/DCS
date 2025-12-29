#include "eeprom_crl.h"

uint8_t mode_info[5];

/**
 * @brief	eeprom状态判定，是否写入过
 *
 * @param   
 *
 * @return  void
**/
void eeprom_statu_judge( void )
{
    uint8_t eeprom_statu_flag;
    uint8_t i;
	
    eeprom_statu_flag = ISP_Read(EEPROM_STATU_JUDGE);

    if( eeprom_statu_flag != 0x58)
    {
        slave_06.channel_num       = 0x07;         // 0000 0111  三路全开
        slave_06.sync_switch       = 0;            // 0000 0001  sync默认关
        slave_06.fan_level         = 3;            // 3  风扇默认三档
        slave_06.power_level       = 50;           // 50         220V输出50%功率
        slave_06.OTP_temp1         = 80;           // 报警温度默认80℃

        for( i = 0; i < 5; i++ )
        {
            Buzzer = 0;

            slave_06.mode_num = i + 1;
            eeprom_data_write();

            Buzzer = 1;
        }

        slave_06.mode_num = 1;
        eeprom_mode_write();
    }else
    {
        eeprom_data_read();
    }

    power_ctrl.OTP_temp2 = 120;

    eeprom_data_init(); 
}

/* 
           扇区0，地址0x0000~0x01FF     存放模式序号

    模式1：扇区1，地址0x0200~0x03FF     存放模式1数据

    模式2：扇区2，地址0x0400~0x05FF     存放模式2数据

    模式3：扇区3，地址0x0600~0x07FF     存放模式3数据

    模式4：扇区4，地址0x0800~0x09FF     存放模式4数据

    模式5：扇区5，地址0x0a00~0x0aFF     存放模式5数据
*/
void eeprom_mode_write( void )
{
    ISP_Earse(MODE_ADDR);
    ISP_Write(MODE_ADDR,slave_06.mode_num);

    ISP_Write(EEPROM_STATU_JUDGE,0x58);
}

void eeprom_data_read( void )
{
    uint8_t i;
    uint16_t addr;

    slave_06.mode_num = ISP_Read(MODE_ADDR);

    addr = (slave_06.mode_num * 2) << 8;

    for( i = 0; i < 5; i++)
    {
        mode_info[i] = ISP_Read(addr);
        addr++;
    }
}

void eeprom_data_write( void )
{
    uint8_t i;
    uint16_t addr;

    mode_info[0] = slave_06.channel_num;
    mode_info[1] = slave_06.sync_switch;
    mode_info[2] = slave_06.fan_level;
    mode_info[3] = slave_06.power_level;
    mode_info[4] = slave_06.OTP_temp1;

    addr = ( slave_06.mode_num * 2 ) << 8;
    ISP_Earse(addr);

    for( i = 0; i < 5; i++ )
    {
        ISP_Write(addr,mode_info[i]);
        addr++;
    }
}

/**
 * @brief	eeprom 数据初始化
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_init( void )
{
    slave_06.channel_num    = mode_info[0];

    slave_06.sync_switch    = mode_info[1];

    slave_06.fan_level      = mode_info[2];
    fan_ctrl();

    slave_06.power_level    = mode_info[3];
    power_level_crl();

    slave_06.OTP_temp1 = mode_info[4];
}
