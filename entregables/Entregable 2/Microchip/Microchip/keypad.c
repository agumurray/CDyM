/**
 * @file keypad.c
 * @brief Implementación del manejo de teclado matricial 4x4.
 *
 * Este módulo permite inicializar el teclado y escanear teclas presionadas,
 * detectando nuevos eventos mediante flancos de subida.
 */

#include "lcd.h"
#include <avr/io.h>
#include <stdint.h>
#include "keypad.h"

// Tabla de correspondencia entre índices de fila/columna y caracteres
const uint8_t keypad_keys[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

/**
 * @brief Inicializa los pines del teclado matricial.
 *
 * Configura las filas como salidas y las columnas como entradas con resistencias pull-up activadas.
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
 * @brief Escanea el teclado matricial en busca de teclas presionadas.
 *
 * Detecta una nueva tecla presionada mediante flanco de subida. Retorna la tecla por referencia.
 *
 * @param key Puntero para almacenar el carácter de la tecla detectada.
 * @return 1 si se detectó una nueva tecla presionada, 0 en caso contrario.
 */
uint8_t KEYPAD_scan(uint8_t *key)
{
  static uint8_t last_key = 0xFF; // Última tecla detectada
  uint8_t current_key = 0xFF;

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

    _delay_us(DEBOUNCE_DELAY_US); // Pequeña espera para rebotes

    // Escanear columnas (LOW activa)
    if (!(PIND & (1 << COLA)))
    {
      current_key = keypad_keys[row * 4];
      break;
    }
    if (!(PIND & (1 << COLB)))
    {
      current_key = keypad_keys[row * 4 + 1];
      break;
    }
    if (!(PIND & (1 << COLC)))
    {
      current_key = keypad_keys[row * 4 + 2];
      break;
    }
    if (!(PIND & (1 << COLD)))
    {
      current_key = keypad_keys[row * 4 + 3];
      break;
    }
  }

  // Detectar flanco de subida (nueva tecla diferente a la anterior)
  if (current_key != 0xFF && current_key != last_key)
  {
    *key = current_key;
    last_key = current_key;
    return 1;
  }

  // Si no se detecta ninguna tecla, reiniciar estado
  if (current_key == 0xFF)
  {
    last_key = 0xFF;
  }

  return 0;
}
