#ifndef _USART_H_INCLUDED
#define _USART_H_INCLUDED

#define UART_ISR_ON		1
#define UART_ISR_OFF	0

#define FRAME_ERROR	 (1<<4)
#define DATA_OVERRUN (1<<3)
#define PARITY_ERROR (1<<2)


#define UART0_BAUD 57600UL
#define UART0_UBRR (F_CPU/(16*UART0_BAUD)-1) // za kvarc od 11059200 Hz dodati jos -1

#define UART1_BAUD 9600UL
#define UBRR1_VALUE (F_CPU/(16*UART1_BAUD)-1)

void UART0_Init(unsigned long baud, char useISR);
void initUart1(unsigned long, char);
void UART0_Write(unsigned char);
void UART1_Write(unsigned char);
unsigned char UART0_Read(void);
unsigned char UART1_Read(void);
void altUart1Write(unsigned char);
void altUart0Write(unsigned char);

#endif
