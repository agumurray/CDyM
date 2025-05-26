/**
 * @file game_utils.h
 * @brief Declaraciones y variables globales para el manejo de temporización en el juego.
 *
 * Contiene variables de temporización
 * y la declaración de la función de inicialización del temporizador.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TIMER_H
#define TIMER_H
#include "game_mef.h"

/// Estado actual del juego
extern volatile GameState gameState;

/**
 * @brief Inicializa el temporizador Timer0 en modo CTC.
 *
 * Configura el Timer0 con un prescaler de 1024 y un valor de comparación de 155,
 * lo que genera interrupciones aproximadamente cada 10 ms.
 */
void timer_init(void);

#endif
