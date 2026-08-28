#ifndef __ADC_DMA_H__
#define __ADC_DMA_H__
#include <stdint.h>
#include "stm32f4xx.h"
void adc_dma_init(void);
void adc_tim_dma_init(void);


#define LISR_TCIF0		    (1U<<5)
#define LIFCR_CTCIF0		(1U<<5)

#define LISR_TEIF0			(1U<<3)
#define LIFCR_CTEIF0		(1U<<3)
#define NUM_OF_CHANNELS 2
#define NUM_OF_SAMPLES 10

#endif
