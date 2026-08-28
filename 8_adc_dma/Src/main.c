#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

#define BUFFER_SIZE		5

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];


int main(void)
{

	uart2_tx_init();
	adc_dma_init();

	while(1)
	{

	}

}

