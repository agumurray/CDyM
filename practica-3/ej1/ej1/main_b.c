#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#define BUFFER_SIZE 64

// Buffers y estados
volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile bool nueva_cadena = false;

volatile char tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_index = 0;
volatile uint8_t enviando = 0;

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

void UART_SendString_IT(char *str) {
	strncpy((char *)tx_buffer, str, BUFFER_SIZE - 3);
	tx_buffer[BUFFER_SIZE - 3] = '\0';

	tx_index = 0;
	enviando = 1;

	UCSR0B |= (1 << TXCIE0);
	UART_SendChar(tx_buffer[tx_index++]);
}


// ISR de recepción
ISR(USART_RX_vect) {
	char c = UDR0;
	if (rx_index < BUFFER_SIZE - 1) {
		if (c == '\r') {
			rx_buffer[rx_index] = '\0';
			rx_index = 0;
			nueva_cadena = true;
			} else {
			rx_buffer[rx_index++] = c;
		}
		} else {
		rx_index = 0; // Overflow de buffer
	}
}

// ISR de transmisión completa
ISR(USART_TX_vect) {
	if (enviando == 1) {
		char c = tx_buffer[tx_index];
		if (c != '\0') {
			UART_SendChar(c);
			tx_index++;
			} else {
			UART_SendChar('\r');
			enviando = 2;
		}
		} else if (enviando == 2) {
		UART_SendChar('\n');
		enviando = 0;
		UCSR0B &= ~(1 << TXCIE0); // Deshabilita interrupción TX
	}
}

int main(void) {
	UART_Init();
	sei();

	while (1) {
		if (nueva_cadena) {
			nueva_cadena = false;
			UART_SendString_IT((char *)rx_buffer);
		}
	}
}
