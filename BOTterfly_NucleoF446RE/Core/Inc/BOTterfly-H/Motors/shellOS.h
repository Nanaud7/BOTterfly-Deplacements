/**
 ******************************************************************************
 * @file	shellOS.h
 * @author 	Arnaud CHOBERT (based on codes of Laurent FIACK)
 * @brief	Shell STM32 adapted for FreeRTOS
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_SHELL_OS_H_
#define INC_SHELL_OS_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Exported types ------------------------------------------------------------*/
/* End of exported types -----------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
#define UART_DEVICE huart2
/* End of exported macros ----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern char c;
extern QueueHandle_t qShell;
/* End of external variables -------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
uint8_t uart_write(char *s, uint16_t size);
uint8_t shell_init(UART_HandleTypeDef* huart);
uint8_t shell_add(char * cmd, int (* pfunc)(int argc, char ** argv), char * description);
uint8_t shell_char_received();
uint8_t shell_exec(char * cmd);

#endif /* INC_SHELL_OS_H_ */
