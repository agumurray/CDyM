#include "lcd.h"
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#include <util/delay.h>

// Definición del teclado matricial
const uint8_t keypad_keys[16] = {
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'*', '0', '#', 'D'};

// Definición de pines para filas y columnas
#define ROWA PORTB4 // Fila A en PB4
#define ROWB PORTB3 // Fila B en PB3
#define ROWC PORTB0 // Fila C en PB0
#define ROWD PORTD7 // Fila D en PD7
#define COLA PORTD3 // Columna A en PD3
#define COLB PORTD5 // Columna B en PD5
#define COLC PORTD4 // Columna C en PD4
#define COLD PORTD2 // Columna D en PD2

// Constantes
#define DEBOUNCE_DELAY_US 5
#define SCAN_DELAY_MS 100
#define MAX_CHARS_LINE_1 16
#define MAX_CHARS_LINE_2 32

/**
 * @brief Inicializa el teclado matricial
 */
void KEYPAD_init(void)
{
	// Configurar filas como salidas
	DDRB |= (1 << ROWA) | (1 << ROWB) | (1 << ROWC);
	DDRD |= (1 << ROWD);

	// Configurar columnas como entradas con pull-up
	DDRD &= ~((1 << COLA) | (1 << COLB) | (1 << COLC) | (1 << COLD));
	PORTD |= (1 << COLA) | (1 << COLB) | (1 << COLC) | (1 << COLD);
}

/**
 * @brief Escanea el teclado matricial en busca de teclas presionadas
 * @param key Puntero para almacenar la tecla detectada
 * @return 1 si se detectó una tecla, 0 en caso contrario
 */
uint8_t KEYPAD_scan(uint8_t *key)
{
	for (uint8_t row = 0; row < 4; row++)
	{
		// Activar todas las filas (HIGH)
		PORTB |= (1 << ROWA) | (1 << ROWB) | (1 << ROWC);
		PORTD |= (1 << ROWD);

		// Desactivar la fila actual (LOW)
		switch (row)
		{
		case 0:
			PORTB &= ~(1 << ROWA);
			break;
		case 1:
			PORTB &= ~(1 << ROWB);
			break;
		case 2:
			PORTB &= ~(1 << ROWC);
			break;
		case 3:
			PORTD &= ~(1 << ROWD);
			break;
		}

		_delay_us(DEBOUNCE_DELAY_US); // Espera para estabilización

		// Escanear columnas (activas en LOW)
		if (!(PIND & (1 << COLA)))
		{
			*key = keypad_keys[row * 4];
			return 1;
		}
		if (!(PIND & (1 << COLB)))
		{
			*key = keypad_keys[row * 4 + 1];
			return 1;
		}
		if (!(PIND & (1 << COLC)))
		{
			*key = keypad_keys[row * 4 + 2];
			return 1;
		}
		if (!(PIND & (1 << COLD)))
		{
			*key = keypad_keys[row * 4 + 3];
			return 1;
		}
	}

	return 0; // No se detectó tecla presionada
}

int main()
{
	LCD_Init();
	KEYPAD_init();

	uint8_t key, count = 0, state = 0, pressed = 0;

	while (1)
	{
		state = KEYPAD_scan(&key);

		if (!pressed && state)
		{
			// Manejo del cursor en el LCD
			if (count == MAX_CHARS_LINE_1)
			{
				LCDGotoXY(0, 1); // Mover a la segunda línea
			}
			else if (count == MAX_CHARS_LINE_2)
			{
				LCDclr();				 // Limpiar pantalla
				LCDGotoXY(0, 0); // Mover al inicio
				count = 0;			 // Resetear contador
			}

			count++;
			LCDstring(&key, 1); // Mostrar tecla
			pressed = 1;				// Marcar como presionada
		}

		if (!state)
		{
			pressed = 0; // Resetear estado cuando se suelta la tecla
		}

		_delay_ms(SCAN_DELAY_MS);
	}
}