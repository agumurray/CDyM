#define F_CPU 16000000
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

typedef enum {RED, RED_AMBER, GREEN, AMBER} eLight_State;

#define RED_DURATION         20
#define RED_AMBER_DURATION   5
#define GREEN_DURATION       30
#define AMBER_DURATION       5

#define RED_BIT     0b00001000  // PB3 (bit 3)
#define AMBER_BIT   0b00000100  // PB2 (bit 2)
#define GREEN_BIT   0b00000010  // PB1 (bit 1)

static eLight_State Light_state;
static uint8_t Time_in_state = 0;

void Semaforo_Init(void) {
	DDRB |= (RED_BIT | AMBER_BIT | GREEN_BIT); // configurar pines como salida
	Light_state = RED;
	Time_in_state = 0;
}

void Semaforo_Update(void) {
	switch (Light_state) {
		case RED:
		PORTB = RED_BIT;
		if (++Time_in_state >= RED_DURATION) {
			Light_state = RED_AMBER;
			Time_in_state = 0;
		}
		break;
		case RED_AMBER:
		PORTB = RED_BIT | AMBER_BIT;
		if (++Time_in_state >= RED_AMBER_DURATION) {
			Light_state = GREEN;
			Time_in_state = 0;
		}
		break;
		case GREEN:
		PORTB = GREEN_BIT;
		if (++Time_in_state >= GREEN_DURATION) {
			Light_state = AMBER;
			Time_in_state = 0;
		}
		break;
		case AMBER:
		PORTB = AMBER_BIT;
		if (++Time_in_state >= AMBER_DURATION) {
			Light_state = RED;
			Time_in_state = 0;
		}
		break;
	}
}

int main(void) {
	Semaforo_Init();
	while (1) {
		Semaforo_Update();
		_delay_ms(1000); // actualización cada 1 segundo
	}
}
