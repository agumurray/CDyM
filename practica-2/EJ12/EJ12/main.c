#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// ========================
// Variables Globales
// ========================
volatile uint8_t tick_count = 0;
volatile uint8_t flag_update = 0;
volatile char state = 1;
char seq1 = 0b01010101;
char seq2 = 0b00000001;
char dir = 0;
char pressed = 0;

// ========================
// Timer0 CTC Init - 100ms
// ========================
void Timer0_Init(void) {
	TCCR0A = (1 << WGM01);                  // Modo CTC
	TCCR0B = (1 << CS02) | (1 << CS00);     // Preescaler 1024
	OCR0A = 156;                            // (16MHz / 1024) * 0.1s - 1 = 156
	TIMSK0 |= (1 << OCIE0A);                // Habilita interrupción por OCR0A
}

// ========================
// ISR: Se activa cada 100ms
// ========================
ISR(TIMER0_COMPA_vect) {
	if (++tick_count >= 10) {  // 10 × 10 ms = 100 ms
		tick_count = 0;
		flag_update = 1;
	}
}

// ========================
// Main
// ========================
int main(void) {
	// Configuración de puertos
	DDRD = 0xFF;
	DDRC &= ~(1 << PORTC0 | 1 << PORTC1);
	DDRB |= (1 << PORTB3 | 1 << PORTB4);
	PORTC |= (1 << PORTC0 | 1 << PORTC1);

	// Inicialización
	Timer0_Init();
	sei();  // Habilita interrupciones globales

	while (1) {
		if (flag_update) {
			flag_update = 0;

			// Apagar LEDs PB3 y PB4
			PORTB &= ~(1 << PORTB3 | 1 << PORTB4);

			// Si el botón PC1 está presionado
			if (!(PINC & (1 << PINC1))) {
				PORTB |= (state << 3);  // Enciende PB3 o PB4
			}

			// Botón en PC0 para cambiar estado
			if (!pressed) {
				if (!(PINC & (1 << PINC0))) {
					state = (state % 3) + 1;
					seq1 = 0b01010101;
					seq2 = 0b00000001;
					dir = 0;
					pressed = 1;
				}
				} else {
				if (PINC & (1 << PINC0)) {
					pressed = 0;
				}
			}

			// Lógica de estados
			switch (state) {
				case 1:
				PORTD = seq1;
				seq1 ^= 0xFF;
				break;

				case 2:
				PORTD = seq2;
				seq2 <<= 1;
				if (seq2 == 0) seq2 = 0b00000001;
				break;

				case 3:
				PORTD = seq2;
				if (dir) {
					seq2 >>= 1;
					} else {
					seq2 <<= 1;
				}
				if ((dir && seq2 == (1 << PD0)) || (!dir && seq2 == (1 << PD7))) {
					dir ^= 1;
				}
				break;

				default:
				state = 1;
				seq1 = 0b01010101;
				break;
			}
		}
	}
}
