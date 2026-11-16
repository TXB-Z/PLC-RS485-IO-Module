#ifndef DIO_H
#define DIO_H

#include "dio.h"
#include "rs485.h"
#include "main.h"
#include "modbus.h"
#include <stdio.h>

extern uint32_t baud_rate;
extern uint32_t t35_ms;
void Baud_Init(void);

uint8_t Handle_Read_Data_DI(uint8_t *frame, uint16_t size);
uint8_t Handle_Read_Data_DO(uint8_t *frame, uint16_t size);


void Handle_Read_DI(uint8_t *rx_buffer, uint16_t rx_size);
void Handle_Read_DO(uint8_t *rx_buffer, uint16_t rx_size);
void Handle_Write_Multiple_DO(uint8_t *frame, uint16_t size);
void Handle_Write_SINGLE_DO(uint8_t *frame, uint16_t size);

uint8_t Read_sigle_di(uint16_t address);// doc le tung di
uint8_t Read_sigle_do(uint16_t address);// doc le tung do
void Write_Multiple_DO(uint16_t address, uint16_t quantity, uint8_t data);// viet nhieu do
void Write_Single(uint16_t address, uint8_t state);

uint8_t DI_ReadRange(uint16_t start_address, uint16_t quantity);// doc di theo fame ben trong goi lai ham doc ke
uint8_t DO_ReadRange(uint16_t start_address, uint16_t quantity);

#endif