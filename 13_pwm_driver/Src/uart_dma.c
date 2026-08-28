#include "uart_dma.h"

#define UART2EN			(1U<<17)
#define GPIOAEN			(1U<<0)



#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)
#define DMATEN          (1U<<7)
#define DMAREN          (1U<<6)
#define SR_TC           (1U<<6)
#define CR1_TCIE        (1U<<6)
#define DMA1EN          (1U<<21)

#define DMASxEN                  (1U<<0)

#define DMA_MINC_EN              (1U<<10)
#define DMA_TCIE_EN              (1U<<4)
#define DMA_CIRC                 (1U<<8)

#define DMA1_HISR_TCIF6			 (1U<<21)
#define DMA1_HISR_TCIF5			 (1U<<11)




#define UART_BAUDRATE	115200
#define CLK				16000000

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
char uart_data_buffer[DATABUFFER];
extern uint8_t g_rx_cmplt;
extern uint8_t g_tx_cmplt;
uint8_t g_uart_cmplt;

void uart2_rx_tx_init(void)
{

	/*************Configure UART GPIO pin********************/
	/*1.Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*2.Set PA2 mode to alternate function mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |=	 (1U<<5);
	/*Set PA3 to AF mode*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |=	 (1U<<7);

	/*3.Set PA2 alternate function function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);
	/*Set PA3 to af mode 7*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/*************Configure UART Module********************/
	/*4. Enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*DMA Enabler*/
	USART2->CR3 = DMATEN | DMAREN;

	/*5. Set baudrate*/
	uart_set_baudrate(CLK,UART_BAUDRATE);

	/*6. Set transfer direction*/
	 USART2->CR1 = CR1_TE | CR1_RE;

	/*7. Update Status register (Transmission Complete)*/
	 USART2->SR &= SR_TC;
	/*8.Transfer Complete interrupt*/
	 USART2->CR1 |= CR1_TCIE;

	/*9. Enable uart module*/
	 USART2->CR1 |= CR1_UE;

	 NVIC_EnableIRQ(USART2_IRQn);

}

void dma1_init(void)
{
	RCC->AHB1ENR |= DMA1EN;

	/*Enable DMA Stream6 Interrupt*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

void dma1_stream5_uart2_rx(void)
{
	DMA1_Stream5->CR &=~ DMASxEN;

	while((DMA1_Stream5->CR & DMASxEN)){}

	DMA1->HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;

	DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

	DMA1_Stream5->M0AR= (uint32_t)(&(uart_data_buffer));

	DMA1_Stream5->NDTR= DATABUFFER;

	DMA1_Stream5->CR |= (1U<<27);
	DMA1_Stream5->CR &=~(1U<<26);
	DMA1_Stream5->CR &=~(1U<<25);

	DMA1_Stream5->CR |= DMA_MINC_EN;

	DMA1_Stream5->CR |= DMA_TCIE_EN;

	DMA1_Stream5->CR|= DMA_CIRC;

	DMA1_Stream5->CR &=~(1U<<6);
	DMA1_Stream5->CR &=~(1U<<7);

	DMA1_Stream5->CR |= DMASxEN;

	NVIC_EnableIRQ(DMA1_Stream5_IRQn);


}

void dma1_stream6_uart2_tx(uint32_t msg, uint32_t len_msg)
{
	DMA1_Stream6->CR &=~ DMASxEN;

	while((DMA1_Stream6->CR & DMASxEN)){}

	DMA1->HIFCR |= DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6;

	DMA1_Stream6->PAR = (uint32_t)(&(USART2->DR));

	DMA1_Stream6->M0AR= msg;

	DMA1_Stream6->NDTR= len_msg;

	DMA1_Stream6->CR |= (1U<<27);
	DMA1_Stream6->CR &=~(1U<<26);
	DMA1_Stream6->CR &=~(1U<<25);

	DMA1_Stream6->CR |= DMA_MINC_EN;

	DMA1_Stream6->CR |= DMA_TCIE_EN;

	//DMA1_Stream6->CR|= DMA_CIRC;

	DMA1_Stream6->CR |= (1U<<6);
	DMA1_Stream6->CR &=~(1U<<7);

	DMA1_Stream6->CR |= DMASxEN;

	NVIC_EnableIRQ(DMA1_Stream6_IRQn);


}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk +( baudrate/2U ))/baudrate);
}


static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR  = compute_uart_bd(periph_clk,baudrate);
}

/*
void DMA1_Stream6_IRQHandler(void)
{
	if((DMA1->HISR) & DMA1_HISR_TCIF6)
	{
		Do something
		g_tx_cmplt = 1;
		clear the flag
		DMA1->HIFCR |=DMA_HIFCR_CTCIF6;
	}
}
*/

void USART2_IRQHandler(void)
{
	g_uart_cmplt = 1;

	USART2->SR &=~ SR_TC;
}
