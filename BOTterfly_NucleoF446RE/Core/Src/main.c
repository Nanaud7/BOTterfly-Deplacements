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
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include <BOTterfly-H/config.h>
#include <BOTterfly-H/direction.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SYNC_STACK_SIZE 250
#define CMD_STACK_SIZE 500
#define TOF_STACK_SIZE 1000
#define Ph_STACK_SIZE 500
#define RGB_STACK_SIZE 80
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// TO CALCULATE THE STACK SIZE
UBaseType_t uxHighWaterMark_RGB_1;
UBaseType_t uxHighWaterMark_RGB_2;

UBaseType_t uxHighWaterMark_TOF_1;
UBaseType_t uxHighWaterMark_TOF_2;


// TO COUNT THE TIME
double timeDiff = 0;
uint32_t t1 = 0;
uint32_t t2 = 0;

// SEMAPHORE
SemaphoreHandle_t semBinary_CMD;
SemaphoreHandle_t semBinary_TOF;
SemaphoreHandle_t semBinary_Ph;
SemaphoreHandle_t semBinary_RGB;

SemaphoreHandle_t semMutex;
// TO START THE SYSTEM
uint8_t BTN = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Gère la commande des moteurs en fonction des données des capteurs
 */
void vTaskSync(void* p)
{
	int i =0;

	printf("Wait BTN \r\n");
	while(BTN == 0){
		// Ne rien faire
	}
	printf("Start ! \r\n");

	while(1){

		//printf("TOF=%04d %04d %04d PH=%04ld %04ld %04ld NOISE=%04ld %04ld %04ld NORME=%04lu ANGLE=%03ld RED=%d XY=(%05d;%05d) MOY=%04ld\r\n",TOF_Sensor[0].rangeMillimeter,TOF_Sensor[1].rangeMillimeter,TOF_Sensor[2].rangeMillimeter, Ph_Data[0], Ph_Data[1], Ph_Data[2], Ph_DataNoise[0], Ph_DataNoise[1], Ph_DataNoise[2], Ph_Norma, Ph_Angle, Dir_Red, Dir_X, Dir_Y, Ph_Max_Tr);

		if(i%20 == 0){
			xSemaphoreGive(semBinary_CMD);
		}else if(i%20 == 12){
			xSemaphoreGive(semBinary_TOF);
		}else if(i%20 == 4){
			xSemaphoreGive(semBinary_Ph);
		}else if(i%20 == 8){
			xSemaphoreGive(semBinary_RGB);
		}

		i++;

		vTaskDelay(1);
	}
}


void vTaskControl(void * p)
{
	/* Initialisation Moteur Gauche ---------------------*/
	MOT_InitTimer(&MoteurGauche, &htim1, TIM_CHANNEL_1);
	MOT_InitGPIOs(&MoteurGauche,
			MOTG_IN1_GPIO_Port, MOTG_IN1_Pin,
			MOTG_IN2_GPIO_Port, MOTG_IN2_Pin);
	MOT_SetCoeff(&MoteurGauche, 1.2, 0.1);
	MOT_SetDirection(&MoteurGauche, MOT_FUNCTIONS_FORWARD);
	MOT_SetDutyCycle(&MoteurGauche, 0);

	/* Initialisation Moteur Droite ---------------------*/
	MOT_InitTimer(&MoteurDroite, &htim1, TIM_CHANNEL_2);
	MOT_InitGPIOs(&MoteurDroite,
			MOTD_IN3_GPIO_Port, MOTD_IN3_Pin,
			MOTD_IN4_GPIO_Port, MOTD_IN4_Pin);
	MOT_SetCoeff(&MoteurDroite, 1.2, 0.1);
	MOT_SetDirection(&MoteurDroite, MOT_FUNCTIONS_FORWARD);
	MOT_SetDutyCycle(&MoteurDroite, 0);

	/* Initialisation Encodeur Gauche -------------------*/
	ENC_InitTimer(&CodeurGauche, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_2);
	ENC_SetMillimeterPerTick(&CodeurGauche, 0.08200);

	/* Initialisation Encodeur Droite -------------------*/
	ENC_InitTimer(&CodeurDroite, &htim3, TIM_CHANNEL_1, TIM_CHANNEL_2);
	ENC_SetMillimeterPerTick(&CodeurDroite, 0.08125);

	/* Initialisation Odometrie -------------------------*/
	ODO_Init();

	//CTRL_Pos1_SetTargetX(100);
	//CTRL_Pos1_SetTargetY(100);

	while(1){
		xSemaphoreTake(semBinary_CMD,portMAX_DELAY);

		// Calcul Direction à l'aide des fonctions de direction
		if(BTN){
			CTRL_Pos1_PositionControl();

			// TESTS
			/*
			int16_t ticksG = ENC_GetCnt(&CodeurGauche);
			int16_t ticksD = ENC_GetCnt(&CodeurDroite);
			float vitG = CTRL_SpeedControl(&MoteurGauche, &CodeurGauche, ticksG, 100);
			float vitD = CTRL_SpeedControl(&MoteurDroite, &CodeurDroite, ticksD, 100);
			printf("%d;%d\r\n", (int)ticksG, (int)ticksD);
			*/
		}
	}
}


/*
 * Réalise les mesures de distance
 * Indique les distance à l'aide des variables globales :
 * TOF_Sensor[0].rangeMillimeter
 * TOF_Sensor[1].rangeMillimeter
 * TOF_Sensor[2].rangeMillimeter
 */
void vTaskToF(void * p)
{
	uxHighWaterMark_TOF_1 = uxTaskGetStackHighWaterMark(NULL);

	for(int i=0; i<TOF_nbOfSensor; i++){
		HAL_NVIC_EnableIRQ(TOF_Sensor[i].EXTI_IRQn);
	}

	while(1){
		xSemaphoreTake(semBinary_TOF,portMAX_DELAY);

		//printf("vTaskTOF \r\n");

		// Durée max observée : 2.319950ms
		// Durée min observée : 1.225000us
		//		for(int i=0; i<TOF_nbOfSensor; i++){
		//			if(TOF_Sensor[i].it.flag){
		//				TOF_SetDistance_mm(&TOF_Sensor[i]);
		//				TOF_Sensor[i].it.flag = 0;
		//			}
		//		}
		uxHighWaterMark_TOF_2 = uxTaskGetStackHighWaterMark(NULL);
	}
}


void vTaskPhotodiodes(void * p)
{
	// Réalise les trois mesures
	// Calcule l'emplacement le plus lumineux
	// Remplie une variable avec un angle compris entre -100 et 100 pour orienter le robot :
	// Orientation
	//vTaskDelay(5);
	//Ph_Init();

	while(1){
		xSemaphoreTake(semBinary_Ph,portMAX_DELAY);

		Ph_GetMeasure(Ph_Data);
		//printf("--PH_DataNoise : %04lu, %04lu, %04lu\r\n", Ph_Data[0], Ph_Data[1], Ph_Data[2]);
		Ph_GetBestAngle(Ph_Data);
		Ph_GetNorma(Ph_Data);

		//printf("vTask_Ph \r\n");
		//xSemaphoreGive(semBinary_RGB);
	}
}


/*
 * Réalise les mesures de couleur
 * Indique si le sol est rouge avec la variable globale :
 * RGB_Sensor.isFloorRed
 */
void vTaskCouleur(void * p)
{
	uxHighWaterMark_RGB_1 = uxTaskGetStackHighWaterMark(NULL);
	HAL_TIM_IC_Start_IT(RGB_Sensor.Timer_Handle, RGB_Sensor.Timer_Channel);

	//HAL_GPIO_WritePin(RGB_Sensor.LED_GPIOx, RGB_Sensor.LED_GPIO_Pin, RESET);

	while(1){
		xSemaphoreTake(semBinary_RGB,portMAX_DELAY);

		//printf("vTask_RGB \r\n");

		if(RGB_Sensor.it.flag){
			// Bloc traitement IT : 10.15 us
			RGB_IsTheFloorRed(&RGB_Sensor);
			RGB_Sensor.it.flag = 0;
			//
		}
		uxHighWaterMark_RGB_2 = uxTaskGetStackHighWaterMark(NULL);
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
	TaskHandle_t xHandle = NULL;
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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */

	printf("\r\n --------------- BOTterfly --------------- \r\n");

	// INITIALISATION DES CAPTEURS ET DU MOTEUR :
	printf("Initialization \r\n");

	// Bloc TOF_Init : 1735.410150 ms
	TOF_Init_SetI2C(&TOF_Sensor[0], &hi2c1, 0x55);
	TOF_Init_SetGPIOs(&TOF_Sensor[0], TOF_XSHUT0_GPIO_Port, TOF_XSHUT0_Pin, TOF_GPIOI0_GPIO_Port, TOF_GPIOI0_Pin);
	TOF_Init_SetEXTI(&TOF_Sensor[0], TOF_GPIOI0_EXTI_IRQn);

	TOF_Init_SetI2C(&TOF_Sensor[1], &hi2c1, 0x58);
	TOF_Init_SetGPIOs(&TOF_Sensor[1], TOF_XSHUT1_GPIO_Port, TOF_XSHUT1_Pin, TOF_GPIOI1_GPIO_Port, TOF_GPIOI1_Pin);
	TOF_Init_SetEXTI(&TOF_Sensor[1], TOF_GPIOI1_EXTI_IRQn);

	TOF_Init_SetI2C(&TOF_Sensor[2], &hi2c1, 0x5b);
	TOF_Init_SetGPIOs(&TOF_Sensor[2], TOF_XSHUT2_GPIO_Port, TOF_XSHUT2_Pin, TOF_GPIOI2_GPIO_Port, TOF_GPIOI2_Pin);
	TOF_Init_SetEXTI(&TOF_Sensor[2], TOF_GPIOI2_EXTI_IRQn);

	TOF_Init(TOF_Sensor);
	//

	// Bloc RGB_Init : 401.954350 ms
	RGB_Init_SetTimer(&RGB_Sensor, &htim3, TIM_CHANNEL_1);

	RGB_Init_SetParamGPIOs(&RGB_Sensor, RGB_OE_GPIO_Port, RGB_OE_Pin, RGB_LED_GPIO_Port, RGB_LED_Pin);
	RGB_Init_SetOutFreqGPIOs(&RGB_Sensor, RGB_S0_GPIO_Port, RGB_S0_Pin, RGB_S1_GPIO_Port, RGB_S1_Pin);
	RGB_Init_SetColorFilterGPIOs(&RGB_Sensor, RGB_S2_GPIO_Port, RGB_S2_Pin, RGB_S3_GPIO_Port, RGB_S3_Pin);

	RGB_Init(&RGB_Sensor);

	Ph_Init();

	Dir_Init();

	// Init Motor

	// CRÉATION DES SÉMAPHORES :
	printf("Creation Semaphore \r\n");

	semBinary_CMD = xSemaphoreCreateBinary();
	semBinary_TOF = xSemaphoreCreateBinary();
	semBinary_Ph = xSemaphoreCreateBinary();
	semBinary_RGB = xSemaphoreCreateBinary();

	semMutex = xSemaphoreCreateMutex();

	// CRÉATION DES BOÎTES AUX LETTRES :


	// CRÉATION DES TÂCHES :
	printf("Creation Task \r\n");

	if (xTaskCreate(vTaskSync, "Sync", SYNC_STACK_SIZE, (void *)NULL, 100, &xHandle) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
	{
		printf("Task Sync Creation error : Could not allocate required memory\r\n");
	}
	if (xTaskCreate(vTaskControl, "Control", CMD_STACK_SIZE, (void *)NULL, 80, &xHandle) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
	{
		printf("Task Control Creation error : Could not allocate required memory\r\n");
	}
	if (xTaskCreate(vTaskToF, "ToF", TOF_STACK_SIZE, (void *)NULL, 60, &xHandle) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
	{
		printf("Task ToF Creation error : Could not allocate required memory\r\n");
	}
	if (xTaskCreate(vTaskPhotodiodes, "Photodiodes", Ph_STACK_SIZE, (void *)NULL, 40, &xHandle) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
	{
		printf("Task Photodiode Creation error : Could not allocate required memory\r\n");
	}
	if (xTaskCreate(vTaskCouleur, "Couleur", RGB_STACK_SIZE, (void *)NULL, 20, &xHandle) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
	{
		printf("Task Couleur Creation error : Could not allocate required memory\r\n");
	}


  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
}

/* USER CODE BEGIN 4 */
uint16_t icVal01 = 0;

uint8_t countTime = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// Durée de l'IT : 1.550000us ou 1.325000us
	if(htim->Instance == RGB_Sensor.Timer_Handle->Instance)
	{
		if(RGB_Sensor.it.isFirstCaptured == 0){
			icVal01 = HAL_TIM_ReadCapturedValue(htim, RGB_Sensor.Timer_Channel);
			RGB_Sensor.it.isFirstCaptured = 1;
		}else{
			RGB_Sensor.it.icVal1 = icVal01;
			RGB_Sensor.it.icVal2 = HAL_TIM_ReadCapturedValue(htim, RGB_Sensor.Timer_Channel);
			RGB_Sensor.it.isFirstCaptured = 0;

			__HAL_TIM_SET_COUNTER(htim, 0);

			RGB_Sensor.it.flag = 1;
		}
	}
}


// VL53L0X_RangingMeasurementData_t VL53L0X_RangingMeasurementData;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Durée de l'IT : 0.500000us
	if(GPIO_Pin == TOF_Sensor[0].EXTI_GPIO_Pin){
		// IT toutes les 33ms
		TOF_Sensor[0].it.flag = 1;
		TOF_SetDistance_mm(&TOF_Sensor[0]);

	}
	else if(GPIO_Pin == TOF_Sensor[1].EXTI_GPIO_Pin){
		// IT toutes les 33ms
		TOF_Sensor[1].it.flag = 1;
		TOF_SetDistance_mm(&TOF_Sensor[1]);
	}
	else if(GPIO_Pin == TOF_Sensor[2].EXTI_GPIO_Pin){
		// IT toutes les 33ms
		TOF_Sensor[2].it.flag = 1;
		TOF_SetDistance_mm(&TOF_Sensor[2]);
	}

	else if(GPIO_Pin == BTN_Pin){
		BTN = 1;
	}
}


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
