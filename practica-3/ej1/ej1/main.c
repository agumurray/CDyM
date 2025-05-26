#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#define BUFFER_SIZE 64

volatile char buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile bool nueva_cadena = false;

void UART_Init(void) {
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)(UBRR_VALUE);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_SendChar(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void SerialPort_Send_String(char *str) {
	while (*str) {
		UART_SendChar(*str++);
	}
}

ISR(USART_RX_vect) {
	char c = UDR0;
	if (rx_index < BUFFER_SIZE - 1) {
		if (c == '\r') {
			buffer[rx_index] = '\0';
			rx_index = 0;
			nueva_cadena = true;
		} 
		else {
			buffer[rx_index++] = c;
		}
	}
	else {
		rx_index = 0;
	}
}

int main(void) {
	UART_Init();
	sei();

	while (1) {
		if (nueva_cadena) {
			nueva_cadena = false;
			SerialPort_Send_String((char *)buffer);
			SerialPort_Send_String("\r\n");
		}
	}
}
