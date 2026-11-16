#ifndef MODBUS_H
#define MODBUS_H

#include "main.h"
#include <stdbool.h>

typedef enum
{
    MODBUS_FC_READ_COILS            = 0x01,  // Doc DO
    MODBUS_FC_READ_DISCRETE_INPUTS  = 0x02,  // Doc DI
    MODBUS_FC_WRITE_SINGLE_COIL     = 0x05,  // Ghi 1 DO
    MODBUS_FC_WRITE_MULTIPLE_COILS  = 0x0F   // Ghi nhieu DO
} Modbus_FunctionCode;

bool Modbus_CheckID(uint8_t *frame, uint16_t size);
bool Modbus_CheckCRC(uint8_t *frame, uint16_t size);
uint8_t Modbus_Check_FuntionCode(uint8_t *frame);
uint16_t Modbus_CALCULATED_CRC16(uint8_t *frame, uint16_t size);

/*void Modbus_BuildResponse_FC02(uint8_t data,
                               uint8_t response[]
                               );
*/

#endif