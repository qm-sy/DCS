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
    uint8_t i,j;
    uint16_t addr;

    eeprom_statu_flag = ISP_Read(EEPROM_STATU_JUDGE);

    if( eeprom_statu_flag == 0xFF)
    {
        ac_dc.channel_num       = 0x07;          // 0000 0111  三路全开
        channel_close();

        ac_dc.sync_flag         = 0x00;          // 0000 0001  sync默认关
        sync_ctrl();

        ac_dc.fan_level         = 0x03;          // 3  风扇默认三档
        fan_ctrl(ac_dc.fan_level);

        ac_dc.power_level       = 0x32;          // 50         220V输出50%功率
        ac_220v_crl(ac_dc.power_level);
        
        ac_dc.alarm_temp_val    = 0x50;          // 报警温度默认80℃

        mode_info[0] = ac_dc.channel_num;
        mode_info[1] = ac_dc.sync_flag;
        mode_info[2] = ac_dc.fan_level;
        mode_info[3] = ac_dc.power_level;
        mode_info[4] = ac_dc.alarm_temp_val;

        for( j = 0; j < 5; j++)
        {
            Buzzer = 0;
            addr = ((j + 1) * 2) << 8;
            ISP_Earse(addr);
            for( i = 0; i < 5; i++)
            {
                ISP_Write(addr,mode_info[i]);
                addr++;
                Buzzer = 1;
            }
        }
        ac_dc.mode_num = 1;
        eeprom_mode_record();
    }else
    {
        eeprom_data_init(); 
    }
}

void eeprom_mode_record( void )
{
    ISP_Earse(MODE_ADDR);
    ISP_Write(MODE_ADDR,ac_dc.mode_num);
    ISP_Write(EEPROM_STATU_JUDGE,0x58);
}

/**
 * @brief	eeprom 数据写入
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_record( void )
{
    uint8_t i;
    uint16_t addr;

    mode_info[0] = ac_dc.channel_num;
    mode_info[1] = ac_dc.sync_flag;
    mode_info[2] = ac_dc.fan_level;
    mode_info[3] = ac_dc.power_level;
    mode_info[4] = ac_dc.alarm_temp_val;

    addr = (ac_dc.mode_num * 2) << 8;
    ISP_Earse(addr);

    for( i = 0; i < 5; i++)
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
    uint8_t i;
    uint16_t addr;

    ac_dc.mode_num = ISP_Read(MODE_ADDR);

    addr = (ac_dc.mode_num * 2) << 8;
    for( i = 0; i < 5; i++)
    {
        mode_info[i] = ISP_Read(addr);
        addr++;
    }
    ac_dc.channel_num = mode_info[0];
    channel_close();

    ac_dc.sync_flag   = mode_info[1];
    sync_ctrl();

    ac_dc.fan_level   = mode_info[2];
    fan_ctrl(ac_dc.fan_level);

    ac_dc.power_level = mode_info[3];
    ac_220v_crl(ac_dc.power_level);

    ac_dc.alarm_temp_val    = mode_info[4];
}
