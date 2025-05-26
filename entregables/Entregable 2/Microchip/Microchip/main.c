/**
 * @file main.c
 * @brief Programa principal del juego de contraseñas con teclado y LCD.
 *
 * Este sistema utiliza un teclado matricial 4x4, una pantalla LCD, temporización por interrupciones
 * y una máquina de estados finitos (MEF) para controlar el flujo del juego de adivinar la contraseña.
 * El juego inicia al presionar el botón '*'.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <time.h>
#include "lcd.h"
#include "keypad.h"
#include "game_mef.h"
#include "timer.h"

extern volatile uint8_t temporization_flag; ///< Bandera de temporización para el juego, cambia cada interrupcion

/**
 * @brief Función principal del programa.
 *
 * Inicializa los periféricos, muestra un mensaje de bienvenida y ejecuta
 * la máquina de estados del juego en un bucle infinito.
 *
 * @return Esta función nunca retorna.
 */

int main(void)
{
    LCD_Init();    ///< Inicializa pantalla LCD
    KEYPAD_init(); ///< Inicializa teclado matricial
    timer_init();  ///< Inicializa temporizador (interrupciones)
    _delay_ms(10); ///< Pequeña demora de estabilización

    LCDstring(" --Bienvenido--", 15); ///< Mensaje de bienvenida
    LCDGotoXY(0, 1);
    LCDstring("Presiona * init", 15); ///< Instrucción inicial

    sei(); ///< Habilitar interrupciones globales

    while (1)
    {
        if (temporization_flag)
        {
            game_mef(); ///< Ejecuta la máquina de estados del juego
            temporization_flag = 0;
        }
    }

    return 0; ///< Esta línea nunca se alcanza, pero es buena práctica incluirla.
}
