/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
volatile uint8_t ret = 0;

uint8_t rx_data1;

uint8_t  rx_index2, rx_data2;
char rx_buffer2[250];
char ResponseRX[250];

uint8_t ConfigAT = 0;
long last = 0;

uint8_t ErrorCode = 0;
uint8_t CheckConnect = 0;

uint32_t TTTB1 = 0;
uint32_t TB1 = 0;
uint32_t TTTB2 = 0;
uint32_t TB2 = 0;

uint8_t Second = 0;
uint8_t Minute = 0;
uint8_t Hour = 0;
uint8_t Date = 0;
uint8_t Day = 0;
uint8_t Month = 0;
uint8_t Year = 0;



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
/* USER CODE BEGIN PFP */
void Received_AT_Commands_ESP(void);
void Reset_ESP(void);
void clearbuffer_UART_ESP(void);
void SettingESP(void);
void Button(void);
void Send_AT_Commands_Setting(char *AT_Commands, char *DataResponse, uint32_t timesend , uint32_t setting, uint32_t count);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
     #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
     #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	// cong USB
	HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,0xFFFF);  //A 
	// Cong giao tiep ESP
	//HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};
RTC_TimeTypeDef currTime = {0};

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_data2, 1);//ESP 
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data1, 1);//USB PC
	
	printf("start\r\n");
	//SettingESP();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    /* USER CODE BEGIN 3 */
//		if(ErrorCode == 1)
//		{
//			while(1)
//			{
//			printf("ESP reset!!!\r\n");
//			SettingESP();
//				if (ErrorCode == 0)
//				{
//					printf("ESP reset Suceessfully!!!\r\n");
//					break;
//				}
//			}
//		} 
//		else if(ErrorCode == 0)
//		{
//			// get and update data
//			while(1)
//			{
//				Button();
//				if(HAL_GetTick() - last >= 500)
//				{
//				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
//				last = HAL_GetTick();
//				}
//				if(ErrorCode == 1)
//				{
//					break;
//				}
//			}
//		}
		
		
//		HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
//		HAL_RTC_GetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);
//		Second = sTime.Seconds;
//		Minute = sTime.Minutes;
//		Hour   = sTime.Hours;
//		Day = DateToUpdate.Date;
//		Month = DateToUpdate.Month;
//		Year = DateToUpdate.Year;
//		printf("Gio: %d - Phut: %d -  Giay: %d\n", Hour, Minute, Second);
		HAL_Delay(200);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Button()
{
	// PB14 OUTPUT , PB15 OUTPUT 
	//PB12 INPUT , PB13 INPUT
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
	{
		HAL_Delay(300);
		printf("Nut 1 start\r\n");
		while(1)
		{
			last = HAL_GetTick();
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1)
			{
				if(TTTB1 == 0)
				{
					TTTB1 = 1;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
				}
				else if(TTTB1 == 1)
				{
					TTTB1 = 0;
					 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
				}
				HAL_Delay(300);
				break;
			}
		}
	}
	
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 0)
	{
		printf("Nut 2 start\r\n");
		HAL_Delay(300);
		while(1)
		{
			last = HAL_GetTick();
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1)
			{
				if(TTTB2 == 0)
				{
					TTTB2 = 1;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				}
				else if(TTTB2 == 1)
				{
					TTTB2 = 0;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
				}
				HAL_Delay(300);
				
				break;
				
			}
		}
	}
	
	//CheckUpdateDK();
	
}

void SettingESP()
{
	Send_AT_Commands_Setting("AT+RST\r\n", "OK", 1000 , 0,10);
	
	HAL_Delay(100);

	Send_AT_Commands_Setting("AT\r\n", "OK", 2500, 0, 10);
	
	//HAL_Delay(100);
	
	Send_AT_Commands_Setting("ATE0\r\n", "OK" , 1000, 0, 10);
	
	//HAL_Delay(3000);
	
	Send_AT_Commands_Setting("AT+CWMODE=1,1\r\n", "OK", 1000, 0 ,10);
	
	//HAL_Delay(3000);
	
	Send_AT_Commands_Setting("AT+CWJAP=\"Le Hoang 1\",\"hoang1801\"\r\n", "WIFI CONNECTED", 1500, 0 ,10);
	
	//HAL_Delay(3000);
	
	Send_AT_Commands_Setting("AT+CIPMUX=0\r\n", "OK", 1000 , 0, 10);
	
	//HAL_Delay(3000);
	
	Send_AT_Commands_Setting("AT+CIPSTART=\"TCP\",\"188.166.206.43\",80\r\n", "CONNECT\r\n\r\nOK", 1000, 0, 10);
	
	//HAL_Delay(3000);
	
	ErrorCode = 0;
}

void Send_AT_Commands_Setting(char *AT_Commands, char *DataResponse, uint32_t timesend , uint32_t setting, uint32_t count)
{
	uint32_t Count = 0;
	
	last = HAL_GetTick();
	ConfigAT = setting;
	uint8_t Size_AT = 100;
	char DataSendAT[Size_AT];
	for(int i = 0 ; i < Size_AT; i++)
	{
		DataSendAT[i] = 0;
	}
	// dua data lenh AT_Commands vao mang  DataSendAT
	//snprintf(DataSendAT, sizeof(DataSendAT),"%s\r\n", AT_Commands);
	sprintf(DataSendAT,"%s\r\n",AT_Commands);
	// send ESP qua cong UART2
	//clearbuffer_UART_ESP();
	HAL_UART_Transmit(&huart1,(uint8_t *)&DataSendAT,strlen(DataSendAT),1000);
	printf("Send AT-Commands Setting: %s\r\n", DataSendAT);
	// dung doi phan hoi + doi lau qua thi goi tiep ( phan hoi tu ham ngat uart2) 
	last = HAL_GetTick();
	while(1)
	{
		// qua 5s thi gui lai lenh cu ( gui khi nao ok thi thoi)
		
		
		if(HAL_GetTick() - last >= timesend)
		{
			Count ++;
			HAL_UART_Transmit(&huart1,(uint8_t *)&DataSendAT,strlen(DataSendAT),1000);
			printf("Send AT-Commands Setting TimeSend: %s\r\n", DataSendAT);
			last = HAL_GetTick();
		}
		
		if(strstr(rx_buffer2,DataResponse) != NULL)
		{
			//printf("Data Buffer2: %s\r\n",rx_buffer2);
			printf("Reponse Setting: %s\r\n",DataResponse);
			clearbuffer_UART_ESP();
			last = HAL_GetTick();
			Count = 0;
			break;
		}
		if (Count >= count)
		{
			Count = 0;
			Reset_ESP();
			HAL_Delay(500);
			//break;
		}
	}
}

void Reset_ESP()
{
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
}

void Received_AT_Commands_ESP()
{
	rx_buffer2[rx_index2++] = rx_data2;
}

void clearbuffer_UART_ESP()
{
	for(int i = 0 ; i < 250 ; i++)
	{
		rx_buffer2[i] = 0;
	}
	rx_index2 = 0;	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// che do setting ConfigAT = 0
	if(ConfigAT == 0)
	{
		if(huart -> Instance == USART1)
		{
			Received_AT_Commands_ESP();		
			HAL_UART_Receive_IT(&huart1,&rx_data2,1);
		}
	}
	else if(ConfigAT == 1)
	{
		if(huart -> Instance == USART1)
		{
			//Received_AT_Commands_ESP_SendBlynk();
			HAL_UART_Receive_IT(&huart1,&rx_data2,1);
		}
	}
	else if(ConfigAT == 2)
	{
		if(huart -> Instance == USART1)
		{
			//Received_AT_Commands_ESP_GetBlynk();
			HAL_UART_Receive_IT(&huart1,&rx_data2,1);
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  if (GPIO_Pin == GPIO_PIN_10)
			{
				ret ++;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
			}
			else
				{
				ret = 0;
				}

}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
