#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL

volatile uint8_t overflow_count = 0;

void Timer0_Init(void)
{
	TCCR0A = 0x00; // Modo normal
	//TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	TCCR0B |= (1<<CS02) | (1<<CS00); //Prescaler 1024
	TCCR0B &= ~(1<<CS01); //Prescaler 1024
	TIMSK0 = (1 << TOIE0); // Habilitar interrupción por overflow
	TCNT0 = 0; // Inicializar contador
}

ISR(TIMER0_OVF_vect)
{
	overflow_count++;
	if (overflow_count >= 3) // 3 overflows (con prescaler 1024) - 49 overflows (con prescaler 64) = ~50ms
	{
		PORTB ^= (1 << PB0); // Toggle LED
		overflow_count = 0;
	}
}

int main(void)
{
	DDRB |= (1 << PB0); // PB0 salida
	PORTB &= ~(1 << PB0); // LED apagado inicialmente

	Timer0_Init();
	sei(); // Habilitar interrupciones globales

	while (1)
	{
		// Loop vacío
	}
}
