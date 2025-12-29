#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "sys.h"
#include "communication.h"
#include "delay.h"
#include "tm1722.h"
#include "common.h"

#define SLAVE_ADDR     0xDC

#define FUN_03      0X03
#define FUN_04      0X04
#define FUN_06      0X06
#define FUN_16      0X10

#define START_REG_03        0
#define REG_NUM_03          6

#define START_REG_04        0
#define REG_NUM_04          2

typedef struct 
{
    uint8_t  byte_info_H;                //DATA_H
    uint8_t  byte_info_L;                //DATA_L
    uint8_t  rcv_addr1_valH;             //DATA1 H 位置

    uint16_t start_reg_03;                  //查询起始位置
    uint16_t reg_num_03;                 //查询起始位置

    uint16_t start_reg_04;                 //查询起始位置
    uint16_t reg_num_04;                 //查询起始位置

    uint16_t start_reg_16;                 //查询起始位置
    uint16_t reg_num_16;                 //查询起始位置

    uint16_t reg_addr_06;

    uint8_t scan_flag_04;
    uint8_t scan_flag_04_allow;
    uint8_t modbus_04_rcv_over;
}MODBIS_INFO;


extern MODBIS_INFO modbus;

void Modbus_Event( void );

void Modbus_Fun3( void );
void Modbus_Fun4( void );
void Modbus_Fun6( void );

void slave_to_master(uint8_t code_num,uint8_t length);
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);
void send_to_slave_16( void );
void read_slave_03( void );
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L);
void read_slave_04( void );
void get_slave_status( void );
void modbus_params_init( void );

#endif