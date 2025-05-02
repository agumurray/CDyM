#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL

void Timer0_Init_CTC(void)
{
	TCCR0A = (1 << WGM01); // Modo CTC (Clear Timer on Compare Match)
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = 249; // 1 ms con prescaler 64
	TCNT0 = 0;
}

void retardo_ms(uint8_t ms)
{
	for (uint8_t i = 0; i < ms; i++)
	{
		TCNT0 = 0; // Reiniciar el contador
		TIFR0 |= (1 << OCF0A); // Limpiar la bandera de comparación previa

		// Esperar hasta que el timer llegue a OCR0A
		while (!(TIFR0 & (1 << OCF0A)));
	}
}

int main(void)
{
	DDRB |= (1 << PB0); // PB0 como salida

	Timer0_Init_CTC();

	while (1)
	{
		PORTB ^= (1 << PB0); // Toggle LED
		retardo_ms(200); // Esperar 200 ms
	}
}
