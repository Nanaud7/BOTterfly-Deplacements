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
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Exported types ------------------------------------------------------------*/

/* End of exported types -----------------------------------------------------*/
#define UART_DEVICE huart2

#define _SHELL_FUNC_LIST_MAX_SIZE 64

extern QueueHandle_t qShell;

uint8_t uart_write(char *s, uint16_t size);
uint8_t shell_init(UART_HandleTypeDef* huart);
void shell_char_received();
int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description);
int shell_exec(char c, char * buf);
int shell_run();

#endif /* INC_SHELL_OS_H_ */
