/*
 * keypad.h
 *
 * Created: 29/4/2025 14:12:10
 *  Author: mater
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

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

uint8_t KEYPAD_scan(uint8_t *key); // Escanea el teclado matricial
void KEYPAD_init(void);         // Inicializa el teclado matricial

#endif /* KEYPAD_H_ */