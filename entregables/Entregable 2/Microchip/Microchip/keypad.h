/**
 * @file keypad.h
 * @brief Declaraciones para el manejo de teclado matricial 4x4.
 *
 * Define los pines utilizados, constantes, y las funciones para inicialización y escaneo del teclado.
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

// Pines asignados a filas
#define ROWA PORTB4 ///< Fila A - PB4
#define ROWB PORTB3 ///< Fila B - PB3
#define ROWC PORTB0 ///< Fila C - PB0
#define ROWD PORTD7 ///< Fila D - PD7

// Pines asignados a columnas
#define COLA PORTD3 ///< Columna A - PD3
#define COLB PORTD5 ///< Columna B - PD5
#define COLC PORTD4 ///< Columna C - PD4
#define COLD PORTD2 ///< Columna D - PD2

// Constantes de configuración
#define DEBOUNCE_DELAY_US 5 ///< Tiempo de espera para rebote de teclas (en microsegundos)
#define MAX_CHARS_LINE_1 16 ///< Máximo de caracteres en línea 1 del LCD
#define MAX_CHARS_LINE_2 32 ///< Máximo de caracteres en línea 2 del LCD

/**
 * @brief Inicializa los pines del teclado matricial.
 *
 * Configura las filas como salidas y las columnas como entradas con resistencias pull-up activadas.
 */
void KEYPAD_init(void);

/**
 * @brief Escanea el teclado matricial en busca de teclas presionadas.
 *
 * Detecta una nueva tecla presionada mediante flanco de bajada. Retorna la tecla por referencia.
 *
 * @param key Puntero para almacenar el carácter de la tecla detectada.
 * @return 1 si se detectó una nueva tecla presionada, 0 en caso contrario.
 */
uint8_t KEYPAD_scan(uint8_t *key);

#endif /* KEYPAD_H_ */
