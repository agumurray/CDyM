#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define PRESCALER 8

// Lista de frecuencias musicales (Do4, Re4, Mi4, Fa4...)
const uint16_t notas[] = {261, 294, 329, 349, 392, 440, 493, 523};
#define CANT_NOTAS (sizeof(notas)/sizeof(notas[0]))

void timer1_init_ctc12(uint16_t freq)
{
	// Configurar pines PB1 (OC1A) y PB2 (OC1B) como salidas
	DDRB |= (1 << PB1) | (1 << PB2);

	// Deshabilitar el timer mientras lo configuramos
	TCCR1A = 0;
	TCCR1B = 0;

	// Modo 12 CTC: WGM13:0 = 1100
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << COM1A0) | (1 << COM1B0); // Toggle OC1A y OC1B

	// Prescaler = 8
	TCCR1B |= (1 << CS11);

	// Calcular el valor del TOP (ICR1)
	// f_OC1x = f_clk / (2 * N * (1 + OCR1x)) con OCR1x en toggle
	// Usamos ICR1 como TOP: ICR1 = (f_clk / (f * N)) - 1
	uint16_t top = (F_CPU / (freq * PRESCALER)) - 1;
	ICR1 = top;

	// Generar salida en OC1A y OC1B
	OCR1A = 0;             // Toggle al inicio del ciclo
	OCR1B = top / 4;       // Toggle después de T/4
}

int main(void)
{
	while (1)
	{
		for (uint8_t i = 0; i < CANT_NOTAS; i++)
		{
			timer1_init_ctc12(notas[i]);
			_delay_ms(2000); // Esperar 2 segundos por nota
		}
	}
}
