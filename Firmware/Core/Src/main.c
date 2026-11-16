
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include <stdbool.h>
/* USER CODE BEGIN Includes */
#include <string.h>
#include "modbus.h"
#include "dio.h"
#include "rs485.h"

uint8_t crate_data;
uint8_t response_data[6];
uint16_t response_size_data;
uint8_t SLAVE_ID;
extern uint32_t baud_rate;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void LED_Boot_OK(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void Slave_ID_Init();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t rx_buffer[64];
uint16_t rx_size = 8;
volatile bool frame_received_flash = false;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	


int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	HAL_Delay(5);   
	Slave_ID_Init();
	Baud_Init();
	MX_USART1_UART_Init();
	huart1.Init.BaudRate = baud_rate;      // dat baud theo DIP
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
			Error_Handler();
	}
	LED_Boot_OK();
  
	
  /* USER CODE BEGIN 2 */

	HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, sizeof(rx_buffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		if(frame_received_flash)
			{
					if (Modbus_CheckID(rx_buffer, rx_size) && Modbus_CheckCRC(rx_buffer,rx_size))
					{
							switch (Modbus_Check_FuntionCode(rx_buffer))
							{
									case MODBUS_FC_READ_DISCRETE_INPUTS:
											 Handle_Read_DI(rx_buffer,rx_size);
											 break;

									case MODBUS_FC_READ_COILS:
											 Handle_Read_DO(rx_buffer,rx_size);
											 break;

									case MODBUS_FC_WRITE_SINGLE_COIL:
											Handle_Write_SINGLE_DO(rx_buffer,rx_size);
											break;

									case MODBUS_FC_WRITE_MULTIPLE_COILS:
											Handle_Write_Multiple_DO(rx_buffer,rx_size);
											break;
							}
					}
				frame_received_flash = false;
			}
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3|
												GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* OUTPUT: PA3 = DE/RE cua MAX485 (mac dinh muc thap = che do nhan) */
GPIO_InitStruct.Pin   = GPIO_PIN_3;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure PC13 as output */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
// c
	__HAL_RCC_GPIOB_CLK_ENABLE();


/* Configure PB12..PB15 as DO (output push-pull) */
GPIO_InitStruct.Pin   = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15| GPIO_PIN_10;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15| GPIO_PIN_10, GPIO_PIN_RESET);

/* ===== INPUT: PB0, PB1 (DIP switch) ===== */
GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;      // <-- INPUT
GPIO_InitStruct.Pull = GPIO_PULLUP;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  /* Initial state: LED OFF */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/*
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                                uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        // Chuy?n MAX485 sang phát
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

        // Phát l?i nguyên frame v?a nh?n
        HAL_UART_Transmit(&huart1, rx_buffer, Size, 100);

        // Chuy?n MAX485 v? nh?n
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

        // Ti?p t?c ch? frame ti?p theo
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,
                                    rx_buffer,
                                    sizeof(rx_buffer));
    }
}
*/ //code phan hoi fame
// nhan khung truyen

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                                uint16_t Size)
{
    if (huart->Instance == USART1)
    {
				frame_received_flash = true;
				rx_size = Size;
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,
                                    rx_buffer,
                                    sizeof(rx_buffer));
    }
}

void Slave_ID_Init()
	{
	/*
 * ======================= CAU HINH DIA CHI THIET BI (SLAVE ID) =======================
 *
 * Dia chi Modbus cua module duoc dat bang DIP switch 4 bit.
 * Chuong trinh chi doc DIP switch MOT LAN luc khoi dong (Slave_ID_Init),
 * ket qua luu vao bien slave_id. Doi cong tac xong phai reset hoac cap nguon lai.
 *
 * Phan cong chan (input, pull-up noi):
 *
 *   Cong tac | Chan STM32 | Bit    | Trong so
 *   ---------+------------+--------+---------
 *   SW1      | PB0        | bit 0  | 1   (LSB)
 *   SW2      | PA7        | bit 1  | 2
 *   SW3      | PA6        | bit 2  | 4
 *   SW4      | PA5        | bit 3  | 8   (MSB)
 *
 * Cach dau: moi cong tac noi giua chan STM32 va GND.
 *   - Cong tac ON  (dong) -> chan ve GND -> doc ra muc THAP -> bit = 1
 *   - Cong tac OFF (ho)   -> pull-up keo len 3.3V -> doc ra muc CAO -> bit = 0
 *   Vi doc ra nguoc muc nen trong code kiem tra "== GPIO_PIN_RESET" roi moi set bit.
 *
 * Bang tra dia chi (ON = 1, OFF = 0):
 *
 *   SW4 SW3 SW2 SW1 | Slave ID
 *   OFF OFF OFF OFF | 1   (ID 0 la broadcast nen ep ve 1)
 *   OFF OFF OFF ON  | 1
 *   OFF OFF ON  OFF | 2
 *   OFF OFF ON  ON  | 3
 *   OFF ON  OFF OFF | 4
 *   ...             | ...
 *   ON  ON  ON  ON  | 15
 *
 * Luu y: cac chan nay khong trung voi DI (PA0, PA1, PA4, PA5), DO (PB12..PB15),
 * UART (PA9, PA10), DE/RE (PB6), SWD (PA13, PA14).
 * =====================================================================================
 */
     uint8_t id = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) id |= 0x01;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET) id |= 0x02;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET) id |= 0x04;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET) id |= 0x08;

    if (id == 0) id = 1;   // ID 0 la broadcast, khong dung lam dia chi thiet bi
    SLAVE_ID = id;
	}
void LED_Boot_OK(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);

        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);
    }
}

// bd 1 PA3
//BD 2 PA4