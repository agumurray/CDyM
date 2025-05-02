#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"

// Variables del reloj
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

// Contador de ticks (61 overflows ? 1s)
volatile uint8_t ticks = 0;
volatile uint8_t flag_update = 0;

// Inicializar Timer2 @ 16MHz con prescaler 1024
void Timer2_Init(void) {
	TCCR2A = 0x00;  // Modo normal
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler 1024
	TCNT2 = 0;
	TIMSK2 |= (1 << TOIE2); // Habilitar interrupción por overflow
}

// ISR: cada ~16.384 ms
ISR(TIMER2_OVF_vect) {
	if (++ticks >= 61) {  // 61 x 16.384ms ? 1s
		ticks = 0;
		flag_update = 1;
	}
}

// Incrementar reloj
void actualizar_reloj(void) {
	seconds++;
	if (seconds >= 60) {
		seconds = 0;
		minutes++;
		if (minutes >= 60) {
			minutes = 0;
			hours++;
			if (hours >= 24) {
				hours = 0;
			}
		}
	}
}

// Mostrar reloj en formato HH:MM:SS
void mostrar_reloj_LCD(void) {
	char buffer[9];
	buffer[0] = '0' + hours / 10;
	buffer[1] = '0' + hours % 10;
	buffer[2] = ':';
	buffer[3] = '0' + minutes / 10;
	buffer[4] = '0' + minutes % 10;
	buffer[5] = ':';
	buffer[6] = '0' + seconds / 10;
	buffer[7] = '0' + seconds % 10;
	buffer[8] = '\0';

	LCDGotoXY(0, 0);
	LCDstring((uint8_t *)buffer, 8);
}

int main(void) {
	LCD_Init();       // Inicializa el LCD
	Timer2_Init();    // Inicializa Timer2
	sei();            // Habilita interrupciones

	while (1) {
		if (flag_update) {
			flag_update = 0;
			actualizar_reloj();
			mostrar_reloj_LCD();
		}
	}
}
