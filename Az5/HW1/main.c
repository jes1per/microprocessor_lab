/* USER CODE BEGIN Header */
/*
 * ***************************************************************************************
 * This code is part of projects created by jes1per.
 * All rights reserved.
 * Unauthorized copying, modification, or distribution of this code is strictly prohibited.
 * For more information, visit https://github.com/jes1per
 * ***************************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void lcd_init(void);
void send_command(uint8_t data);
void send_Idata(uint8_t data);
void send_data(uint8_t data);
void lcd_puts(uint8_t *str);
void lcd_clear(void);
void lcd_gotoxy(uint8_t x, uint8_t y);
void create_custom_character(uint8_t location, uint8_t charmap[]);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void lcd_init(void)
{
  HAL_Delay(2);
  send_command(0x02);
  send_command(0x28);
  send_command(0x0F);
}

void send_command(uint8_t data)
{
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);   // RS = 0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);   // RW = 0


	send_Idata(data >>4);
	send_Idata(data);


}

void send_Idata(uint8_t data)
{
	HAL_Delay(10);
	int temp[4];

	for (int i=0; i<4;i++)
	{

		temp[i]=data%2;
		data=data>>1;

	}



	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,temp[0]);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,temp[1]);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,temp[2]);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,temp[3]);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);     // Enable Pin
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);   // Enable Pin
}

void send_data(uint8_t data)
{
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	  // RS = 1
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);   // RW = 0


	send_Idata(data >>4);

	send_Idata(data);


}

void lcd_puts(uint8_t *str)
{
  HAL_Delay(2);
  while(*str !=0x00)
  {
	  send_data(*str);
    str++;
  }
}

void lcd_clear(void)
{
  HAL_Delay(1);
  send_command(0x01);

}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
  HAL_Delay(1);

    switch(y){
    case 0:
      send_command( 0x80 + x );
    break;
    case 1:
      send_command( 0xC0 + x );
      break;

  }
}

void create_custom_character(uint8_t location, uint8_t charmap[]){
    location &= 0x7;  // Limit location to 0-7 (CG-RAM allows 8 custom characters)

    send_command(0x40 + (location * 8));  // Set CGRAM address for the custom character

    for (uint8_t i = 0; i < 8; i++){
        send_data(charmap[i]);  // Send the character map data
    }
}

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
  /* USER CODE BEGIN 2 */
  lcd_init();

  uint8_t alef[8] = {
		0b00000,
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b00000
  };
  uint8_t mim[8] = {
		0b00000,
		0b00000,
		0b00110,
		0b01001,
		0b11110,
		0b00000,
		0b00000,
		0b00000
  };
  uint8_t ye[8] = {
		0b00000,
		0b00000,
		0b00100,
		0b00100,
		0b11111,
		0b00000,
		0b00110,
		0b00000
  };
  uint8_t non[8] = {
		0b00000,
		0b00000,
		0b01000,
		0b00000,
		0b10011,
		0b10010,
		0b01100,
		0b00000
  };
  uint8_t he[8] = {
		0b00000,
		0b00100,
		0b01010,
		0b00001,
		0b11110,
		0b00000,
		0b00000,
		0b00000
   };
  uint8_t mimv[8] = {
		0b00000,
		0b00000,
		0b01110,
		0b01010,
		0b11111,
		0b00000,
		0b00000,
		0b00000
  };
  uint8_t d[8] = {
		0b00000,
		0b00000,
		0b01000,
		0b00100,
		0b01111,
		0b00000,
		0b00000,
		0b00000
  };


  create_custom_character(0, alef);
  create_custom_character(1, mim);
  create_custom_character(2, ye);
  create_custom_character(3, non);
  create_custom_character(4, he);
  create_custom_character(5, mimv);
  create_custom_character(6, d);

  lcd_gotoxy(8, 0);
  send_data(3);
  send_data(2);
  send_data(1);
  send_data(0);
  send_data(6);
  send_data(5);
  send_data(4);
  send_data(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
