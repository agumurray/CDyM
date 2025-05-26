/**
 * @file game_mef.c
 * @brief Implementación de la máquina de estados del juego de ingreso de contraseñas.
 *
 * Este módulo contiene la lógica de control principal del juego, implementada como una
 * máquina de estados finitos. El juego consiste en mostrar una contraseña aleatoria,
 * permitir que el usuario intente ingresarla desde el teclado y verificarla carácter
 * por carácter. Los estados posibles incluyen inicio, muestra de contraseña, juego,
 * acierto, error, victoria y derrota.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "keypad.h"
#include "game_mef.h"
#include "timer.h"

volatile GameState gameState = START;

uint8_t keyPressed = -1;
uint8_t *password = NULL;
uint8_t xPosition = 0;
uint8_t lifes = 3;
uint8_t pressedKeys[4] = {0, 0, 0, 0};
uint8_t currentKey = 0;
uint8_t firstTime = 1;

const uint8_t *dicc[] = {
    "Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas",
    "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES",
    "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud",
    "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};

/**
 * @brief Ejecuta un paso de la máquina de estados del juego.
 *
 * Según el estado actual del juego, delega el control a la función correspondiente.
 * Esta función debe ser llamada periódicamente, idealmente desde el lazo principal.
 */
void game_mef(void)
{
    switch (gameState)
    {
    case START:
        handle_start_state();
        break;

    case SHOW_PASSWORD:
        handle_show_password_state();
        break;

    case GAME:
        handle_game_state();
        break;

    case HIT:
        handle_hit_state();
        break;

    case MISS:
        handle_miss_state();
        break;

    case WIN:
        handle_win_state();
        break;

    case LOSE:
        handle_lose_state();
        break;
    }
}

/**
 * @brief Maneja el estado de inicio del juego.
 *
 * Espera que el usuario presione '*' para iniciar una nueva partida. Selecciona
 * una contraseña aleatoria del diccionario y la muestra por pantalla.
 */
void handle_start_state(void)
{
    keyPressed = -1;
    KEYPAD_scan(&keyPressed);
    if (keyPressed == '*')
    {
        gameState = SHOW_PASSWORD;
        LCDclr();
        _delay_ms(5);
        if (firstTime)
        {
            srand(tick + 100 * second);
            firstTime = 0;
        }
        password = dicc[rand() % 25];
        LCDGotoXY(0, 0);
        LCDstring("Password: ", 10);
        LCDstring(password, 5);
        tick = 0;
        second = 0;
    }
}

/**
 * @brief Maneja el estado de visualización de la contraseña.
 *
 * Espera un tiempo determinado para mostrar la contraseña y luego
 * inicializa las variables del juego y cambia al estado de juego.
 */
void handle_show_password_state(void)
{
    if (second >= 2)
    {
        second = 0;
        gameTick = 0;
        gameSecond = 0;
        xPosition = 0;
        currentKey = 0;
        lifes = 3;
        LCDclr();
        _delay_ms(5);
        LCDGotoXY(0, 0);
        LCDstring("Password: ", 10);
        LCDGotoXY(13, 1);
        LCDsendChar(lifes + '0');
        LCDstring("/3", 2);
        gameState = GAME;
    }
}

/**
 * @brief Maneja el estado principal del juego.
 *
 * Escanea el teclado para recibir la entrada del usuario. Si se presiona '#',
 * se compara la entrada con el carácter correspondiente de la contraseña.
 * Si es correcto se cambia al estado HIT, si es incorrecto, al estado MISS.
 */
void handle_game_state(void)
{
    if (gameState == GAME)
    {
        pressedKeys[currentKey] = 0;
        KEYPAD_scan(&pressedKeys[currentKey]);
        switch (pressedKeys[currentKey])
        {
        case '#':
            pressedKeys[currentKey] = '\0';
            gameState = (atoi(pressedKeys) == password[xPosition]) ? HIT : MISS;
            break;
        case 0:
            break;
        default:
            LCDGotoXY(currentKey, 1);
            LCDsendChar(pressedKeys[currentKey]);
            currentKey++;
            break;
        }
        if (currentKey >= 4)
        {
            gameState = MISS;
        }
    }
}

/**
 * @brief Maneja el estado de acierto (HIT).
 *
 * Actualiza la pantalla y las variables para pasar al siguiente carácter
 * de la contraseña. Si se completa la contraseña, cambia al estado WIN.
 */
void handle_hit_state(void)
{
    xPosition++;
    currentKey = 0;
    LCDGotoXY(10, 0);
    LCDstring(password, xPosition);
    LCDGotoXY(0, 1);
    LCDstring("    ", 4);
    if (xPosition >= 5)
    {
        tick = 0;
        second = 0;
        LCDclr();
        _delay_ms(5);
        LCDGotoXY(0, 0);
        LCDstring("Ganaste!", 8);

        char message[16];
        sprintf(message, "Tiempo: %d seg.", gameSecond);
        uint8_t length = strlen(message);
        uint8_t start_pos = (length < 16) ? (16 - length) / 2 : 0;

        LCDGotoXY(start_pos, 1);
        LCDstring(message, length);

        gameState = WIN;
    }
    else
    {
        gameState = GAME;
    }
}

/**
 * @brief Maneja el estado de error (MISS).
 *
 * Resta una vida, actualiza el display y verifica si se terminó el juego.
 * Si no quedan vidas, cambia al estado LOSE.
 */
void handle_miss_state(void)
{
    currentKey = 0;
    LCDGotoXY(0, 1);
    LCDstring("    ", 4);
    lifes--;
    LCDGotoXY(13, 1);
    LCDsendChar(lifes + '0');
    if (lifes <= 0)
    {
        tick = 0;
        second = 0;
        LCDclr();
        _delay_ms(5);
        LCDGotoXY(0, 0);
        LCDstring("Perdiste!", 9);
        gameState = LOSE;
    }
    else
    {
        gameState = GAME;
    }
}

/**
 * @brief Maneja el estado de victoria (WIN).
 *
 * Espera unos segundos y vuelve al estado de inicio.
 */
void handle_win_state(void)
{
    if (second >= 5)
    {
        LCDclr();
        _delay_ms(5);
        LCDstring(" --Bienvenido--", 15);
        LCDGotoXY(0, 1);
        LCDstring("Presiona * init", 15);
        gameState = START;
    }
}

/**
 * @brief Maneja el estado de derrota (LOSE).
 *
 * Espera unos segundos y vuelve al estado de inicio.
 */
void handle_lose_state(void)
{
    if (second >= 5)
    {
        LCDclr();
        _delay_ms(5);
        LCDstring(" --Bienvenido--", 15);
        LCDGotoXY(0, 1);
        LCDstring("Presiona * init", 15);
        gameState = START;
    }
}
