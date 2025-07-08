#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "sys.h"
#include "communication.h"
#include "delay.h"
#include "tm1722.h"
#include "common.h"

#define MY_ADDR     0xDC

typedef struct 
{
    uint8_t  byte_info_H;                //DATA_H
    uint8_t  byte_info_L;                //DATA_L
    uint8_t  byte_cnt;                   //BYTE NUM*2
    uint8_t  send_value_addr;            //DATA1 H 位置
    uint8_t  rcv_value_addr;             //DATA1 H 位置
    uint16_t start_addr;                 //查询起始位置
    uint8_t  scan_flag_04;
    uint8_t  scan_flag_04_allow;
    uint8_t  modbus_04_rcv_over;
}MODBIS_INFO;


extern MODBIS_INFO modbus;

void Modbus_Event( void );

void Modbus_Fun3( void );
void Modbus_Fun4( void );
void Modbus_Fun6( void );
void Modbus_Fun16( void );

void slave_to_master(uint8_t code_num,uint8_t length);
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);
void send_to_slave_16( void );
void get_slave_params_03( void );
void send_to_slave_06( void );
void get_slave_statu_04( void );
void get_slave_status( void );
void slave_sleep_ctrl( void );
void slave_work_ctrl( void );

#endif