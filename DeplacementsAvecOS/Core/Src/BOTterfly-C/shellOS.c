/**
 ******************************************************************************
 * @file	shellOS.c
 * @author 	Arnaud CHOBERT (based on codes of Laurent FIACK)
 * @brief	Shell STM32 adapted for FreeRTOS
 ******************************************************************************
 */

#include "BOTterfly-H/shellOS.h"

/* Types ---------------------------------------------------------------------*/

// Structure pour une commande du Shell
typedef struct{
	char * cmd;
	int (* func)(int argc, char ** argv);
	char * description;
} shell_func_t;

/* End of types --------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/
#define SHELL_FUNC_LIST_MAX_SIZE 16
#define SHELL_CMD_MAX_SIZE 16
#define SHELL_ARGC_MAX 8
#define SHELL_BUFFER_SIZE 40
/* End of macros -------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
UART_HandleTypeDef* shell_huart = NULL;
char starting[] = "\r\n\r\n===== Shell =====\r\n";
char prompt[] = "@Nucleo-G431 >> ";

char c = 0; // Caractère reçu
uint8_t pos = 0; // Position dans le buffer
static char buf[SHELL_BUFFER_SIZE];
static char backspace[] = "\b \b";
uint8_t it_uart_rx_ready = 0;

static int shell_func_list_size = 0;
static shell_func_t shell_func_list[SHELL_FUNC_LIST_MAX_SIZE];

QueueHandle_t qShell;
/* End of variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * __io_putchar : Indispensable pour printf() sur la liaison uart
 * @param ch Caractère à écrire sur la liaison uart
 * @return 0 si HAL_OK
 */
uint8_t __io_putchar(int ch) {
	if(HAL_OK != HAL_UART_Transmit(shell_huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY)){
		return 1;
	}
	return 0;
}

/**
 * uart_write : Ecrire sur la liaison uart
 * @param s Chaîne de caractères à écrire sur la liaison uart
 * @param size Longueur de la chaîne de caractère
 * @return 0 si HAL_OK
 */
uint8_t uart_write(char *s, uint16_t size) {
	if(HAL_OK != HAL_UART_Transmit(shell_huart, (uint8_t*)s, size, 0xFFFF)){
		return 1;
	}
	return 0;
}

/**
 * sh_help : Menu d'aide du Shell
 * @param argc Nombre d'éléments de la ligne de commande
 * @param argv Eléments sous la forme d'un tableau de chaînes de caractères
 * @return 0
 */
uint8_t sh_help(int argc, char ** argv) {
	int i;
	for(i = 0 ; i < shell_func_list_size ; i++) {
		printf("%s : %s\r\n", shell_func_list[i].cmd, shell_func_list[i].description);
	}
	return 0;
}

/**
 * sh_example : Fonction d'exemple pour le Shell
 * @param argc Nombre d'éléments de la ligne de commande
 * @param argv Eléments sous la forme d'un tableau de chaînes de caractères
 * @return 0
 */
uint8_t sh_example(int argc, char ** argv) {
	printf("argc = %d\r\n", argc);
	for (int i = 0 ; i < argc ; i++) {
		printf("arg numero %d = %s\r\n", i, argv[i]);
	}
	return 0;
}

/**
 *shell_init : Initialisation du Shell
 * @param huart UART Handle à utiliser
 * @return 0 si HAL_OK
 */
uint8_t shell_init(UART_HandleTypeDef* huart) {
	shell_huart = huart;

	uart_write(starting,sizeof(starting));
	uart_write(prompt,sizeof(prompt));

	shell_add("help", sh_help, (char *)"help");
	shell_add("f", sh_example, "example command");

	qShell = xQueueCreate(1,sizeof(char));

	if(HAL_OK != HAL_UART_Receive_IT(shell_huart, (uint8_t*)&c, 1)){
		return 1;
	}
	return 0;
}

int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description) {
	if (shell_func_list_size < _SHELL_FUNC_LIST_MAX_SIZE) {
		shell_func_list[shell_func_list_size].c = c;
		shell_func_list[shell_func_list_size].func = pfunc;
		shell_func_list[shell_func_list_size].description = description;
		shell_func_list_size++;
		return 0;
	}

	return -1;
}

int shell_exec(char c, char * buf) {
	int i;

	int argc;
	char * argv[ARGC_MAX];
	char *p;

	for(i = 0 ; i < shell_func_list_size ; i++) {
		if (shell_func_list[i].c == c) {
			argc = 1;
			argv[0] = buf;

			for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++){
				if(*p == ' ') {
					*p = '\0';
					argv[argc++] = p+1;
				}
			}

			return shell_func_list[i].func(argc, argv);
		}
	}
	printf("%c: no such command\r\n", c);
	return -1;
}

static char buf[BUFFER_SIZE];
static char backspace[] = "\b \b";
static char prompt[] = "> ";

int shell_run() {
	int reading = 0;
	int pos = 0;

	while (1) {
		uart_write(prompt, 2);
		reading = 1;

		while(reading) {
			//char c = uart_read();
			char c = 0;
			xQueueReceive(qShell, &c, portMAX_DELAY);

			switch (c) {
			//process RETURN key
			case '\r':
				//case '\n':
				printf("\r\n");    //finish line
				buf[pos++] = 0;     //to use cprintf...
				printf(":%s\r\n", buf);
				reading = 0;        //exit read loop
				pos = 0;            //reset buffer
				break;
				//backspace
			case '\b':
				if (pos > 0) {      //is there a char to delete?
					pos--;          //remove it in buffer

					uart_write(backspace, 3);
				}
				break;
				//other characters
			default:
				//only store characters if buffer has space
				if (pos < BUFFER_SIZE) {
					uart_write(&c, 1);
					buf[pos++] = c; //store
				}
			}
		}
		shell_exec(buf[0], buf);
	}
	return 0;
}
