#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// ===== CAMBIAR ESTA FRECUENCIA DESEADA EN HZ =====
#define FREQ_DESEADA 440  // Hz (ej. La4 = 440 Hz)

void timer1_init()
{
	// Configurar pin OC1A (PB1) como salida
	DDRB |= (1 << PB1); // PB1 = pin 9 del Arduino Uno

	// Configurar modo CTC (Clear Timer on Compare Match)
	// WGM12 = 1 => Modo CTC con TOP = OCR1A
	// COM1A0 = 1 => Toggle OC1A en match
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler = 8

	// Calcular y cargar OCR1A
	// f_OC1A = f_clk / (2 * N * (1 + OCR1A))
	// => OCR1A = (f_clk / (2 * N * f_OC1A)) - 1
	OCR1A = (F_CPU / (2UL * 8 * FREQ_DESEADA)) - 1;
}

int main(void)
{
	timer1_init();

	while (1)
	{
		// El TIMER1 genera automáticamente la señal de salida
		// No se necesita hacer nada en el loop
	}
}
