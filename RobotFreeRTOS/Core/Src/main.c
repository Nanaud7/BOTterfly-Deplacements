/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "BOTterfly-H/shellOS.h"
#include "BOTterfly-H/modules.h"
#include "BOTterfly-H/odometry.h"
#include "BOTterfly-H/strategy.h"
#include "BOTterfly-H/ctrl_speed.h"
//#include "BOTterfly-H/ctrl_positionV1.h"
#include "BOTterfly-H/ctrl_positionV2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STACK_SIZE 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t bPower = 0;
float speedValue = 100;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * pinout : Affiche toutes les broches connectées et leur fonction
 * @param argc Nombre d'éléments de la ligne de commande
 * @param argv Éléments sous la forme d'un tableau de chaînes de caractères
 * @return 0
 */
uint8_t pinout(int argc, char ** argv){

	printf(" +-------------------------------------+ \r\n");
	printf(" |               PINOUT                | \r\n");
	printf(" +-------------------------------------+ \r\n");
	printf(" | STM32                | FONCTION     | \r\n");
	printf(" +-------------------------------------+ \r\n");
	printf(" | TIM1_CH1  | PA8(D7)  | MOTG_PWM     | \r\n");
	printf(" | GPIO_OUT  | PC1(A4)  | MOTG_IN1     | \r\n");
	printf(" | GPIO_OUT  | PC0(A5)  | MOTG_IN2     | \r\n");
	printf(" | TIM2      | PA0(A0)  | ENCG_A       | \r\n");
	printf(" | TIM2      | PA1(A1)  | ENCG_B       | \r\n");
	printf(" | TIM1_CH2  | PA9(D8)  | MOTD_PWM     | \r\n");
	printf(" | GPIO_OUT  | PB8(D15) | MOTD_IN3     | \r\n");
	printf(" | GPIO_OUT  | PB9(D14) | MOTD_IN4     | \r\n");
	printf(" | TIM3      | PA6(D12) | ENCD_A       | \r\n");
	printf(" | TIM3      | PA7(D11) | ENCD_B       | \r\n");
	printf(" +-------------------------------------+ \r\n");

	return 0;
}

void start(int argc, char ** argv){
	printf("Demarrage\r\n");
	bPower = 1;
}

void stop(int argc, char ** argv){
	printf("Arret\r\n");
	bPower = 0;
}

void speed(int argc, char ** argv){
	if(argc == 2){
		speedValue = atoi(argv[1]);
	}
}

void target(int argc, char ** argv){
	if(argc == 3){
		float x = atof(argv[1]);
		float y = atof(argv[2]);
		CTRL_Pos2_SetTargetX((double)x);
		CTRL_Pos2_SetTargetY((double)y);
	}
}

void vTaskShell(void * p) {
	shell_init(&(UART_DEVICE));
	shell_add("start", start, "Demarrage");
	shell_add("stop", stop, "Arret");
	shell_add("speed", speed, "Vitesse");
	shell_add("target", target, "Mise a jour de la consigne");

	while(1){
		shell_char_received();
	}
}

void vTaskAsserv(void * p){
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
	STRAT_strategyInit();

	while(1){

		if(CTRL_Pos2_isArrived()){
			if(indexStrategy >= (int)3){
				//bTypeAsserv = 1;
				//setTargetX(200);
				//setTargetY(100);
			} else {
				CTRL_Pos2_SetTargetX(strategy[indexStrategy].x);
				CTRL_Pos2_SetTargetY(strategy[indexStrategy].y);
				indexStrategy++;
			}

		}

		if(bPower){
			CTRL_Pos2_PositionControl();

			/*
			// Get ticks from encoders
			int32_t ticksG = ENC_GetCnt(&CodeurGauche);
			int32_t ticksD = ENC_GetCnt(&CodeurDroite);

			// Odometry and speed calculations
			//Odo_OdometryUpdate(ticksG, ticksD);

			// Speed Control Update
			float vitG = vit_SpeedControl(&MoteurGauche, &CodeurGauche, ticksG, speedValue);
			float vitD = vit_SpeedControl(&MoteurDroite, &CodeurDroite, ticksD, speedValue);

			// Debug
			printf("%f;%f\r\n", vitG, vitD);
			 */
		} else {
			MOT_SetDutyCycle(&MoteurGauche, 0);
			MOT_SetDutyCycle(&MoteurDroite, 0);
		}

		vTaskDelay(20);
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
	BaseType_t xReturned;
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
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */

	/* Initialisation de la tâche pour le Shell ---------*/
	xReturned = xTaskCreate(
			vTaskShell,      	/* Function that implements the task. */
			"Shell",         	/* Text name for the task. */
			STACK_SIZE,      	/* Stack size in words, not bytes. */
			(void *) NULL,    	/* Parameter passed into the task. */
			1,					/* Priority at which the task is created. */
			&xHandle);      	/* Used to pass out the created task's handle. */
	if( xReturned == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY ) {
		printf("Task Shell creation error: Could not allocate required memory\r\n");
	}

	/* Initialisation de la tâche pour l'asserv ---------*/
	xReturned = xTaskCreate(
			vTaskAsserv,      	/* Function that implements the task. */
			"Asserv",         	/* Text name for the task. */
			STACK_SIZE,      	/* Stack size in words, not bytes. */
			(void *) NULL,    	/* Parameter passed into the task. */
			2,					/* Priority at which the task is created. */
			&xHandle);      	/* Used to pass out the created task's handle. */
	if( xReturned == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY ) {
		printf("Task Asserv creation error: Could not allocate required memory\r\n");
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
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		if(bPower == 0) {
			bPower = 1;
			printf("Demarrage\r\n");
		}
		else {
			bPower = 0;
			printf("Arret\r\n");
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart->Instance == USART2){
		xQueueSendFromISR(qShell, &c, NULL);
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&c, 1);
	}
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
