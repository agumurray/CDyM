#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Declaración de MEF
typedef enum { UP, FALLING, DOWN, RISING } ButtonState;
#define DEBOUNCE_TIME 20  // en milisegundos

static ButtonState btn_state = UP;
static uint8_t debounce_counter = 0;

uint8_t Button_Update(uint8_t pin_level) {
	uint8_t button_pressed = 0;

	switch (btn_state) {
		case UP:
		if (pin_level == 0) {
			btn_state = FALLING;
			debounce_counter = 0;
		}
		break;
		case FALLING:
		if (pin_level == 0) {
			if (++debounce_counter >= DEBOUNCE_TIME) {
				btn_state = DOWN;
				button_pressed = 1;
			}
			} else {
			btn_state = UP;
		}
		break;
		case DOWN:
		if (pin_level == 1) {
			btn_state = RISING;
			debounce_counter = 0;
		}
		break;
		case RISING:
		if (pin_level == 1) {
			if (++debounce_counter >= DEBOUNCE_TIME) {
				btn_state = UP;
			}
			} else {
			btn_state = DOWN;
		}
		break;
	}
	return button_pressed;
}

// Main program
int main(void) {
	// Configurar PD2 como entrada con pull-up
	DDRD &= ~(1 << PD2);   // Entrada
	PORTD |= (1 << PD2);   // Pull-up

	// Configurar PB0 como salida (LED testigo)
	DDRB |= (1 << PB0);

	while (1) {
		uint8_t pin = (PIND & (1 << PD2)) >> PD2;
		if (Button_Update(pin) == 1) {
			// Toggle del LED en cada pulsación detectada
			PORTB ^= (1 << PB0);
		}

		_delay_ms(1); // se llama cada 1 ms (simula timer periódico)
	}
}
