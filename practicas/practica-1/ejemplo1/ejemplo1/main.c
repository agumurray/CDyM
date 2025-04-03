/* Inclusión de bibliotecas de código */
#include <avr/io.h> // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h> // Retardos por software
/* Función main */
int main (void)
{
	/* Setup */
	DDRD = 0xFF; // Configuro Puerto D como salida
	DDRC &= ~(1<<PORTC0); // Configuro bit0 puerto C como entrada
	PORTC |= (1<<PORTC0); // Habilito Pull-Up en bit0 puerto C
	/* Loop */
	while(1)
	{
		if (PINC & (1<<PINC0))
		{
			PORTD = 0b10101010; // Escribo Port D con patrón de bits
			_delay_ms(100); // Delay de 100 ms
			PORTD = 0x00; // Escribo Port D con todos 0
			_delay_ms(100); // Delay de 100 ms
		}
		else
		{
			PORTD = 0b01010101; // Escribo Port D con otro patrón de bits
			_delay_ms(100); // Delay de 100 ms
			PORTD = 0x00; // Escribo Port D con todos 0
			_delay_ms(100); // Delay de 100 ms
		}
	}
	/* Punto de finalización del programa (NO se debe llegar a este lugar) */
	return 0;
}