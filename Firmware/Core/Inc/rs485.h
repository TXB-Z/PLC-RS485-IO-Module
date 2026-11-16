#ifndef RS485_H
#define RS485_H

#include "main.h"


extern UART_HandleTypeDef huart1;

uint16_t Build_Response_FC02(uint8_t *tx_buf, uint8_t slave_id, uint8_t di_data);
uint16_t Build_Response_FC01(uint8_t *tx_buf, uint8_t slave_id, uint8_t di_data);
uint16_t Build_Response_FC0f(uint8_t *tx_buf, uint16_t  address ,uint16_t  quantity, uint8_t slave_id);

void RS485_Send(uint8_t *data, uint16_t size);


#endif