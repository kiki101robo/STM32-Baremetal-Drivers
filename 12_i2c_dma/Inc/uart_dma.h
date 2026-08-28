#ifndef __UART_DMA_H__
#define __UART_DMA_H__
#include <stdint.h>
#include "stm32f4xx.h"
#define DATABUFFER 5
void uart2_rx_tx_init(void);
void dma1_init(void);
void dma1_stream5_uart2_rx(void);
void dma1_stream6_uart2_tx(uint32_t msg, uint32_t len_msg);

#endif
