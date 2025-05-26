/**
 * @file game_utils.c
 * @brief Implementación de funciones auxiliares para el manejo del temporizador del juego.
 *
 * Este archivo contiene la configuración del temporizador y la rutina de interrupción
 * que controla la temporización en los diferentes estados del juego.
 */

#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "timer.h"

volatile uint8_t tick = 0;
volatile uint8_t second = 0;
volatile uint8_t gameTick = 0;
volatile uint8_t gameSecond = 0;
volatile uint8_t temporization_flag = 0;

/**
 * @brief Inicializa el temporizador Timer0 en modo CTC.
 *
 * Configura el Timer0 con un prescaler de 1024 y un valor de comparación de 155,
 * lo que genera interrupciones aproximadamente cada 10 ms.
 */
void timer_init(void)
{
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 155;                        // Comparación para 10 ms (con F_CPU = 16 MHz)
    TIMSK0 = (1 << OCIE0A);             // Habilitar interrupción por comparación
}

/**
 * @brief Rutina de interrupción del Timer0.
 *
 * Esta ISR se ejecuta aproximadamente cada 10 ms y actualiza variables globales
 * relacionadas con el tiempo transcurrido, dependiendo del estado actual del juego.
 */
ISR(TIMER0_COMPA_vect)
{
    tick++;
    gameTick++;
    temporization_flag ^= 1;

    if (gameState == SHOW_PASSWORD || gameState == WIN || gameState == LOSE)
    {
        if (tick >= 100)
        {
            second++;
            tick = 0;
        }
    }
    else if (gameState == GAME || gameState == HIT || gameState == MISS)
    {
        if (gameTick >= 100)
        {
            gameSecond++;
            gameTick = 0;
        }
    }
}
