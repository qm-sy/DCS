#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "communication.h"
#include "ntc.h"
#include "gpio.h"
#include "power_crl.h"
#include "eeprom_crl.h"

#define MASTER_ADDR    0x01
#define SLAVE_ADDR     0x02

#define FUN_03      0X03
#define FUN_04      0X04
#define FUN_06      0X06
#define FUN_16      0X10

typedef struct 
{
    uint8_t  byte_info_H;                //DATA_H
    uint8_t  byte_info_L;                //DATA_L
    uint8_t  rcv_addr1_valH;             //DATA1 H 位置

    uint16_t start_reg_03;                 //查询起始位置
    uint16_t reg_num_03;                 //查询起始位置

    uint16_t start_reg_04;                 //查询起始位置
    uint16_t reg_num_04;                 //查询起始位置

    uint16_t start_reg_16;                 //查询起始位置
    uint16_t reg_num_16;                 //查询起始位置

    uint16_t reg_addr_06;
}MODBIS_INFO;

extern MODBIS_INFO modbus;

void Modbus_Event( void );

void Modbus_Fun3( void );
void Modbus_Fun4( void );
void Modbus_Fun6( void );
void Modbus_Fun16( void );

void slave_to_master(uint8_t code_num,uint8_t length);
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

#endif