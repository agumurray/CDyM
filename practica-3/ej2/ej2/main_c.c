#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"  // Tu librería de LCD

#define PRESCALER 8
#define TICK_TIME (1.0 / (F_CPU / PRESCALER)) // tiempo de 1 tick del timer

volatile uint16_t last_capture = 0;
volatile uint16_t current_capture = 0;
volatile uint8_t new_capture = 0;

ISR(TIMER1_CAPT_vect)
{
	last_capture = current_capture;
	current_capture = ICR1;
	new_capture = 1;
}

void timer1_input_capture_init()
{
	TCCR1A = 0; // modo normal
	TCCR1B = (1 << ICES1) | (1 << CS11); // flanco subida + prescaler 8
	TIMSK1 = (1 << ICIE1); // habilitar interrupción de input capture
	sei(); // habilitar interrupciones globales
}

int main()
{
	char buffer[16];
	LCD_Init();
	LCDGotoXY(0, 0);
	LCDstring((uint8_t *)"RPM:", 4);

	timer1_input_capture_init();

	while (1)
	{
		if (new_capture)
		{
			new_capture = 0;

			uint16_t ticks = current_capture - last_capture;
			if (ticks == 0) continue;

			float rpm = (60.0 * (F_CPU / PRESCALER)) / (25.0 * ticks);

			LCDGotoXY(0, 5);
			snprintf(buffer, sizeof(buffer), "%4.1f", rpm);
			LCDstring((uint8_t *)buffer, strlen(buffer));
		}
	}
}

