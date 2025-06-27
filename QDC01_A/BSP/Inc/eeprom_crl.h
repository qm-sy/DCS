#ifndef __EEPROM_CRL_H_
#define __EEPROM_CRL_H_

#include "eeprom.h"
#include "sys.h"
#include "power_crl.h"
#include "ntc.h"
#include "stdio.h"

#define MODE_ADDR              0x0000

#define EEPROM_STATU_JUDGE    0X0010         

extern uint8_t mode_info[5];

void eeprom_statu_judge( void );
void eeprom_data_record( void );
void eeprom_mode_record( void );
void eeprom_data_init( void );

#endif