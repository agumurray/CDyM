/*
 * keypad.c
 *
 * Created: 29/4/2025 14:12:31
 *  Author: mater
 */

#include "lcd.h"
#include <avr/io.h>
#include <stdint.h>
#include "keypad.h"

// Definición del teclado matricial
const uint8_t keypad_keys[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

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