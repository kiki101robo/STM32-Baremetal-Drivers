#include "adc_dma.h"

#define GPIOAEN    (1U<<0)
#define ADC1EN     (1U<<8)
#define ADC1ON     (1U<<0)
#define CR1_SCAN   (1U<<8)
#define CR2_DDS    (1U<<9)
#define CR2_DMA    (1U<<8)
#define ADC1SWST   (1U<<30)
#define CR2_CONT   (1U<<1)
#define DMA2_EN    (1U<<22)
#define DMA_SCR_EN (1U<<0)
#define DMA2_CR_MINC (1U<<10)
#define DMA2_CR_PINC (1U<<9)
#define DMA2_CR_TCIE (1U<<4)
#define DMA2_CR_TEIE (1U<<2)
#define DMA2_CR_CIRCEN (1U<<8)

uint16_t adc_raw_data[NUM_OF_CHANNELS];

void adc_dma_init(void)
{
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<0);
	GPIOA->MODER |= (1U<<1);

	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);



	RCC->APB2ENR |= ADC1EN;

	ADC1->SQR3 |= (1U<<20);
	ADC1->SQR3 &= ~(1U<<21);
	ADC1->SQR3 &= ~(1U<<22);
	ADC1->SQR3 &= ~(1U<<23);

	ADC1->SQR1 |= (0U<<0);
	ADC1->SQR1 |= (1U<<0);

	ADC1->CR1 |= CR1_SCAN;

	ADC1->CR2 |= CR2_DDS;
	ADC1->CR2 |= CR2_DMA;
	ADC1->CR2 |= CR2_CONT;

	RCC->AHB1ENR |= DMA2_EN;

	DMA2_Stream0->CR &=~ DMA_SCR_EN;

	while ((DMA2_Stream0->CR & DMA_SCR_EN)){};

	DMA2_Stream0->CR |=  (1U<<13);
	DMA2_Stream0->CR &=~ (1U<<14);

	DMA2_Stream0->CR |= (1U<<11);
	DMA2_Stream0->CR &=~ (1U<<12);

	DMA2_Stream0->CR |= DMA2_CR_MINC;
	DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));
	DMA2_Stream0->M0AR = (uint32_t)(&(adc_raw_data));

	DMA2_Stream0->NDTR = (uint16_t)NUM_OF_CHANNELS;

	DMA2_Stream0->CR &=~ (1U<<7);
	DMA2_Stream0->CR &=~ (1U<<6);

	DMA2_Stream0->CR |= DMA2_CR_CIRCEN;

	DMA2_Stream0->CR |= DMA_SCR_EN;

	ADC1->CR2 |= ADC1ON;
	ADC1->CR2 |= ADC1SWST;
}
