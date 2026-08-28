#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"
#include "uart_dma.h"


extern char uart_data_buffer[DATABUFFER];
extern uint8_t g_rx_cmplt;
extern uint8_t g_tx_cmplt;
extern uint8_t g_uart_cmplt;
char init_msg[120] = {'\0'};

int main(void)
{
	uart2_rx_tx_init();
	dma1_init();
	dma1_stream5_uart2_rx();

	sprintf(init_msg, "Initialization Complete!\n\r");
	dma1_stream6_uart2_tx((uint32_t)init_msg, strlen(init_msg));

	while(!g_tx_cmplt){}


	while(1)
	{
		if (g_rx_cmplt)
		{
			sprintf(init_msg, "Message Received : %s \r\n",uart_data_buffer);
			g_rx_cmplt = 0;
			g_uart_cmplt = 0;
			g_tx_cmplt = 0;
			dma1_stream6_uart2_tx((uint32_t)init_msg, strlen(init_msg));
			while(!g_tx_cmplt){}

		}
	}

}


