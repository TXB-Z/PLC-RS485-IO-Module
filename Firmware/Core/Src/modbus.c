#include "modbus.h"
#include <stdio.h>
extern uint8_t SLAVE_ID;
// kiem tra id
// bien kieu Mobus_FunctionCode xac dinh ma code hien tai muon gi dc dinh nghia tai file modbus.h
extern UART_HandleTypeDef huart1;
bool Modbus_CheckID(uint8_t *frame, uint16_t size) // kiem tra id thiet bi
{
		if(size < 1) return false;
    if(frame[0] == SLAVE_ID) return true;
		return false;
}

uint8_t Modbus_Check_FuntionCode(uint8_t *frame)// kiem tra ma code
{
	    switch (frame[1])
    {
        case MODBUS_FC_READ_COILS:
            return MODBUS_FC_READ_COILS;

        case MODBUS_FC_READ_DISCRETE_INPUTS:
            return MODBUS_FC_READ_DISCRETE_INPUTS;

        case MODBUS_FC_WRITE_SINGLE_COIL:
            return MODBUS_FC_WRITE_SINGLE_COIL;

        case MODBUS_FC_WRITE_MULTIPLE_COILS:
            return MODBUS_FC_WRITE_MULTIPLE_COILS;

        default:
            return 0;
    }
}

bool Modbus_CheckCRC(uint8_t *frame, uint16_t size)
{
    uint16_t crc_received;
    uint16_t crc_calculated;

    if (size < 3)
        return false;
    crc_received = frame[size - 2] | ((uint16_t)frame[size - 1] << 8);

    crc_calculated = Modbus_CALCULATED_CRC16(frame, size - 2);
    if (crc_received == crc_calculated)
        return true;
    return false;
}
	uint16_t Modbus_CALCULATED_CRC16(uint8_t *frame, uint16_t size)// ham tu tinh ma crc tu frame nhan dc
	{
		uint16_t crc = 0xFFFF;
		
    for (uint16_t i = 0; i < size; i++)
    {
        crc ^= frame[i];// xor voi tung byte
        for (uint8_t j = 0; j < 8; j++)// xu ly theo thuat toan
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
	}
	
/*void Modbus_BuildResponse_FC02(uint8_t data,
                               uint8_t response[]
                               )
{
    uint16_t crc;

    response[0] = SLAVE_ID;
    response[1] = MODBUS_FC_READ_DISCRETE_INPUTS;
    response[2] = 1;        // Byte Count
    response[3] = data;
		response[4]= 0xff;
		response[5]=0xff;
    crc = Modbus_CALCULATED_CRC16(response, 6);

    response[6] = crc & 0xFF;          // CRC Low
    response[7] = (crc >> 8) & 0xFF;  // CRC High


}
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	