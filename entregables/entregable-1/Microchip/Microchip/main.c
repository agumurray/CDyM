/* Inclusion de bibliotecas de codigo */
#include <avr/io.h>			 // Biblioteca para manipular los registros del microcontrolador AVR
#define F_CPU 16000000UL // Definicion de la frecuencia del oscilador en 16 MHz
#include <util/delay.h>	 // Biblioteca para generar retardos por software

/* Funcion principal */
int main(void)
{
	/* Configuracion inicial */
	char state = 1;					// Variable para almacenar el estado actual
	char seq1 = 0b01010101; // Secuencia inicial para el estado 1
	char seq2 = 0b00000001; // Secuencia inicial para los estados 2 y 3
	char dir = 0;						// Direccion de desplazamiento para el estado 3 (0: izquierda, 1: derecha)
	char pressed = 0;				// Variable para detectar si el boton fue presionado

	DDRD = 0xFF;													// Configuro el Puerto D como salida
	DDRC &= ~(1 << PORTC0 | 1 << PORTC1); // Configuro los pines PC0 y PC1 del Puerto C como entradas
	DDRB |= (1 << PORTB3 | 1 << PORTB4);	// Configuro los pines PB3 y PB4 del Puerto B como salidas
	PORTC |= (1 << PORTC0 | 1 << PORTC1); // Activo resistencias pull-up en los pines PC0 y PC1

	/* Bucle principal */
	while (1)
	{
		// Apago los LEDs conectados a PB3 y PB4
		PORTB &= ~(1 << PORTB3 | 1 << PORTB4);

		// Si el boton conectado a PC1 esta presionado
		if (!(PINC & (1 << PINC1)))
		{
			// Enciendo los LEDs correspondientes al estado actual en PB3 o PB4
			PORTB |= (state << 3);
		}

		// Si el boton conectado a PC0 esta presionado
		if (!pressed) // Verifico si el boton no estaba previamente presionado
		{
			if (!(PINC & (1 << PINC0))) // Si el boton esta presionado
			{
				// Cambio al siguiente estado (1, 2 o 3)
				state = (state % 3) + 1;
				// Reinicio las secuencias y la direccion
				seq1 = 0b01010101;
				seq2 = 0b00000001;
				dir = 0;
				pressed = 1; // Indico que el boton fue presionado
			}
		}
		else // Si el boton estaba presionado
		{
			if (PINC & (1 << PINC0)) // Si el boton ya no esta presionado
			{
				pressed = 0; // Indico que el boton fue liberado
			}
		}

		// Comportamiento segun el estado actual
		switch (state)
		{
		case 1:
			// Estado 1: Alternancia de LEDs en el Puerto D
			PORTD = seq1; // Salida de la secuencia actual
			seq1 ^= 0xFF; // Invierto los bits de la secuencia
			break;

		case 2:
			// Estado 2: Desplazamiento de un bit encendido hacia la izquierda
			PORTD = seq2;			// Salida de la secuencia actual
			seq2 = seq2 << 1; // Desplazo el bit encendido hacia la izquierda
			if (seq2 == 0)		// Si la secuencia se desborda
			{
				seq2 = 0b00000001; // Reinicio la secuencia
			}
			break;

		case 3:
			// Estado 3: Desplazamiento de un bit encendido con cambio de direccion
			PORTD = seq2; // Salida de la secuencia actual
			if (dir)			// Si la direccion es derecha
			{
				seq2 = seq2 >> 1; // Desplazo el bit encendido hacia la derecha
			}
			else // Si la direccion es izquierda
			{
				seq2 = seq2 << 1; // Desplazo el bit encendido hacia la izquierda
			}
			// Cambio de direccion si se alcanza el extremo
			if ((dir && seq2 == 1 << PORTD0) || (~dir && seq2 == 1 << PORTD7))
			{
				dir ^= 1; // Invierto la direccion
			}
			break;

		default:
			// Estado por defecto: Reinicio al estado 1
			state = 1;
			seq1 = 0b01010101;
			break;
		}
		_delay_ms(100); // Retardo de 100 ms
	}

	/* Punto de finalizacion del programa (NO se debe llegar a este lugar) */
	return 0;
}