/*
 * Microchip.c
 *
 * Created: 29/4/2025 14:11:11
 * Author : mater
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "keypad.h"
#include "lcd.h"

const uint8_t *dicc[] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};

#define F_CPU 16000000UL
typedef enum
{
    START,
    SHOW_PASSWORD,
    GAME,
    HIT,
    MISS,
    WIN,
    LOSE
} GameState;

volatile GameState gameState = START;
volatile uint8_t keyPressed = -1;
volatile uint8_t *password = NULL;
volatile uint8_t xPosition = 0;
volatile uint8_t lifes = 3;
volatile uint8_t tick = 0;
volatile uint8_t second = 0;
volatile uint8_t gameTick = 0;
volatile uint8_t gameSecond = 0;

void timer_init(void)
{
    // Configurar el temporizador 0 en modo CTC
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 155;                        // Valor de comparación para 1 segundo
    TIMSK0 = (1 << OCIE0A);             // Habilitar interrupción por comparación
}

ISR(TIMER0_COMPA_vect)
{
    static uint8_t pressedKeys[4] = {0, 0, 0, 0};
    static uint8_t currentKey = 0;
    tick++;
    gameTick++;
    switch (gameState)
    {
    case START:
        keyPressed = -1;
        KEYPAD_scan(&keyPressed);
        if (keyPressed == '*')
        {
            gameState = SHOW_PASSWORD;
            LCDclr();
            password = dicc[rand() % 25];
            LCDGotoXY(0, 0);
            LCDstring("Password: ", 10);
            LCDstring(password, 5);
            tick = 0;
            second = 0;
        }
        break;

    case SHOW_PASSWORD:
        if (tick >= 100)
        {
            second++;
            tick = 0;
        }
        if (second >= 2)
        {
            second = 0;
            gameTick = 0;
            gameSecond = 0;
            xPosition = 0;
            currentKey = 0;
            lifes = 3;
            LCDclr();
            LCDGotoXY(0, 0);
            LCDstring("Password: ", 10);
            LCDGotoXY(13, 1);
            LCDsendChar(lifes + '0');
            LCDstring("/3", 2);
            gameState = GAME;
        }
        break;

    case GAME:
        if (gameTick >= 100)
        {
            gameSecond++;
            gameTick = 0;
        }
        pressedKeys[currentKey] = 0;
        KEYPAD_scan(&pressedKeys[currentKey]);
        switch (pressedKeys[currentKey])
        {
        case '#':
            pressedKeys[currentKey] = '\0';
            gameState = atoi(pressedKeys) == password[xPosition] ? HIT : MISS;
            break;
        case 0:
            break;
        default:
            LCDGotoXY(currentKey, 1);
            LCDsendChar(pressedKeys[currentKey]);
            currentKey++;
            break;
        }
        _delay_ms(200);
        if (currentKey >= 4)
        {
            gameState = MISS;
        }

        break;

    case HIT:
        if (gameTick >= 100)
        {
            gameSecond++;
            gameTick = 0;
        }
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
            LCDGotoXY(0, 0);
            LCDstring("Ganaste!", 8);

            char message[16];
            sprintf(message, "Tiempo: %d seg.", gameSecond);
            uint8_t length = strlen(message);
            uint8_t start_pos = 0;

            if (length < 16)
                start_pos = (16 - length) / 2;

            LCDGotoXY(start_pos, 1);
            LCDstring(message, length);

            gameState = WIN;
        }
        else
        {
            gameState = GAME;
        }
        break;

    case MISS:
        if (gameTick >= 100)
        {
            gameSecond++;
            gameTick = 0;
        }
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
            LCDGotoXY(0, 0);
            LCDstring("Perdiste!", 9);
            LCDGotoXY(0, 1);
            gameState = LOSE;
        }
        else
        {
            gameState = GAME;
        }
        break;

    case WIN:
        if (tick >= 100)
        {
            second++;
            tick = 0;
        }
        if (second >= 5)
        {
            LCDclr();
            LCDstring("--Bienvenido--", 15);
            LCDGotoXY(0, 1);
            LCDstring("Presiona * init", 15);
            gameState = START;
        }
        break;

    case LOSE:
        if (tick >= 100)
        {
            second++;
            tick = 0;
        }
        if (second >= 5)
        {
            LCDclr();
            LCDstring("--Bienvenido--", 15);
            LCDGotoXY(0, 1);
            LCDstring("Presiona * init", 15);
            gameState = START;
        }
        break;
    }
}

int main(void)
{
    LCD_Init();
    KEYPAD_init();
    timer_init();
    _delay_ms(10);
    srand(time(NULL)); // Inicializar la semilla para la función rand()
    LCDstring("--Bienvenido--", 15);
    LCDGotoXY(0, 1);
    LCDstring("Presiona * init", 15);
    sei(); // Habilitar interrupciones globales
    while (1)
    {
    }
}
