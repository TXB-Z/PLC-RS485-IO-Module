#include "dio.h"
#include "rs485.h"
//* chon toc do giaot iep////////////////////////////////////////////////
uint32_t baud_rate = 4800;
uint32_t t35_ms    = 5;

void Baud_Init(void)
{
    static const uint32_t baud_table[4] = {4800, 9600, 19200, 115200};
    uint8_t sel = 0;

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) sel |= 0x01;   // SW1
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) sel |= 0x02;   // SW2

    baud_rate = baud_table[sel];

    // T3.5 = 3.5 ky tu x 11 bit / baud, lam tron len; tren 19200 chuan co dinh 1.75 ms
    if (baud_rate > 19200) t35_ms = 2;
    else                   t35_ms = (38500UL / baud_rate) + 1;
}

/*
 * Phan cong chan dau vao so (DI):
 *
 * PB5 -> DI1
 * PB6 -> DI2
 * PB8 -> DI3
 * PB9 -> DI4
 *
 * Cac chan duoc lua chon nham dam bao 4 kenh DI hoat dong doc lap,
 * khong xung dot voi cac ngoai vi chinh cua he thong nhu USART1
 * dung cho truyen thong RS485, cac chan dieu khien DO, DE/RE,
 * cau hinh dia chi thiet bi va giao tiep SWD.
 */
 
 /*
 * Phan cong chan dau ra so (DO):
 *
 * PB12 -> DO1
 * PB13 -> DO2
 * PB14 -> DO3
 * PB15 -> DO4
 *
 * Cac chan duoc lua chon nham dam bao 4 kenh Do hoat dong doc lap,
 * khong xung dot voi cac ngoai vi chinh cua he thong nhu USART1
 * dung cho truyen thong RS485, cac chan dieu khien DO, DE/RE,
 * cau hinh dia chi thiet bi va giao tiep SWD.
 */
 
/*******************************************************************************************************************/	
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
																			/***** mã code FC02********/
	// hàm doc 1 di rieng le
uint8_t Read_sigle_di(uint16_t address)
	{
			switch (address)
			{
					case 0:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
					case 1:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
					case 2:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
					case 3:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
					default:
							return 0;
			}
	}
	
	// doc di theo vi tri tu frame
uint8_t DI_ReadRange(uint16_t start_address, uint16_t quantity)
	{
			uint8_t data = 0;
			for (uint16_t i = 0; i < quantity; i++)
			{
					if (Read_sigle_di(start_address + i))
					{
							data |= (1 << i);
					}
			}
			return data;
	}
	
uint8_t Handle_Read_Data_DI(uint8_t *frame, uint16_t size)// ham xu ly doc ra trang thai các di
	{
		uint16_t address;
    uint16_t quantity;
    uint8_t data;

    address = ((uint16_t)frame[2] << 8) | frame[3];
    quantity = ((uint16_t)frame[4] << 8) | frame[5];
		return DI_ReadRange(address,quantity);
	}

	
void Handle_Read_DI(uint8_t *rx_buffer, uint16_t rx_size)// tao data repons va gui len
	{
		uint8_t  di_data = Handle_Read_Data_DI(rx_buffer, rx_size);
		uint8_t  tx_buf[8];
		uint16_t tx_len  = Build_Response_FC02(tx_buf, rx_buffer[0], di_data);
		RS485_Send(tx_buf, tx_len);
	}

/*******************************************************************************************************************/	
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
																			/***** mã code FC01********/
	uint8_t Read_sigle_do(uint16_t address)// doc tung do
	{
			switch (address)
			{
					case 0:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
					case 1:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
					case 2:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
					case 3:
							return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
					default:
							return 0;
			}
	}
	
uint8_t DO_ReadRange(uint16_t start_address, uint16_t quantity)// gom toan bo data do
	{
			uint8_t data = 0;
			for (uint16_t i = 0; i < quantity; i++)
			{
					if (Read_sigle_do(start_address + i))
					{
							data |= (1 << i);
					}
			}
			return data;
	}
	
	uint8_t Handle_Read_Data_DO(uint8_t *frame, uint16_t size)// ham xu ly doc ra trang thai các di
	{
		uint16_t address;
    uint16_t quantity;
    uint8_t data;

    address = ((uint16_t)frame[2] << 8) | frame[3];
    quantity = ((uint16_t)frame[4] << 8) | frame[5];
		return DO_ReadRange(address,quantity);
	}
	
void Handle_Read_DO(uint8_t *rx_buffer, uint16_t rx_size)// tao data repons va gui len
	{
		uint8_t  do_data = Handle_Read_Data_DO(rx_buffer, rx_size);
		uint8_t  tx_buf[8];
		uint16_t tx_len  = Build_Response_FC01(tx_buf, rx_buffer[0], do_data);
		RS485_Send(tx_buf, tx_len);
	}
/*******************************************************************************************************************/	
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
																			/***** mã code FC05********/
																					/* dk le DO*/
	void Handle_Write_SINGLE_DO(uint8_t *frame, uint16_t size)// ham xu ly dieu khien 1 di
	{
		//if (size != 8) return;
		uint16_t address;
    uint16_t state_of_DO;//

    address = ((uint16_t)frame[2] << 8) | frame[3];
    state_of_DO = ((uint16_t)frame[4] << 8) | frame[5];
		
			switch (address)
				{
				case 0x00:
						if(state_of_DO == 0xff00) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
						else if (state_of_DO == 0x0000) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
						break;
				case 0x01:
						if(state_of_DO == 0xff00) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
						else if (state_of_DO == 0x0000) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
						break;
				case 0x02:
						if(state_of_DO == 0xff00) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
						else if (state_of_DO == 0x0000) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
						break;
				case 0x03:
						if(state_of_DO == 0xff00) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
						else if (state_of_DO == 0x0000) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
						break;
				}
			RS485_Send(frame,size);
		
	}
	
/*******************************************************************************************************************/	
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
/*******************************************************************************************************************/			
																			/***** mã code FC0F********/
																					/* dk nhieu DO*/
	void Handle_Write_Multiple_DO(uint8_t *frame, uint16_t size)// ham xu ly dieu khien nhieu di
	{   
		uint16_t address;
    uint16_t quantity;
		uint8_t byte_count;
		uint8_t data;
		address = ((uint16_t)frame[2] << 8) | frame[3];
    quantity = ((uint16_t)frame[4] << 8) | frame[5];
		byte_count = frame[6];
		data = frame[7];
		if(address >= 4)return;
		if(quantity == 0) return;
		if((address + quantity) > 4) return;
		Write_Multiple_DO(address,quantity,data);
		uint8_t  tx_buf[8];
		uint16_t tx_len  = Build_Response_FC0f(tx_buf, address ,quantity,frame[0]);
		RS485_Send(tx_buf, tx_len);
	}
void Write_Multiple_DO(uint16_t address, uint16_t quantity, uint8_t data)
{
    for (uint16_t i = 0; i < quantity; i++)
    {
        uint8_t state = (data >> i) & 0x01;   // bit th? i c?a data
        Write_Single(address + i, state);     // chân th? address + i
    }
}

void Write_Single(uint16_t address, uint8_t state)
{
    GPIO_PinState pin_state;

    pin_state = state ? GPIO_PIN_SET : GPIO_PIN_RESET;

    switch (address)
    {
        case 0:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, pin_state);
            break;

        case 1:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, pin_state);
            break;

        case 2:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, pin_state);
            break;

        case 3:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, pin_state);
            break;
    }
}
