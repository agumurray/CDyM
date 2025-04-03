/* Inclusión de bibliotecas de código */
#include <avr/io.h>      // Biblioteca para manipular los registros del microcontrolador AVR
#define F_CPU 16000000UL // Definición de la frecuencia del oscilador en 16 MHz
#include <util/delay.h>  // Biblioteca para generar retardos por software

/* Función principal */
int main(void)
{
    /* Configuración inicial */
    char state = 1;         // Variable para almacenar el estado actual
    char seq1 = 0b01010101; // Secuencia inicial para el estado 1
    char seq2 = 0b00000001; // Secuencia inicial para los estados 2 y 3
    char dir = 0;           // Dirección de desplazamiento para el estado 3 (0: izquierda, 1: derecha)

    DDRD = 0xFF;                          // Configuro el Puerto D como salida
    DDRC &= ~(1 << PORTC0 | 1 << PORTC1); // Configuro los pines PC0 y PC1 del Puerto C como entradas
    DDRB |= (1 << PORTB3 | 1 << PORTB4);  // Configuro los pines PB3 y PB4 del Puerto B como salidas
    PORTC |= (1 << PORTC0 | 1 << PORTC1); // Activo resistencias pull-up en los pines PC0 y PC1

    /* Bucle principal */
    while (1)
    {
        // Apago los LEDs conectados a PB3 y PB4
        PORTB &= ~(1 << PORTB3 | 1 << PORTB4);

        // Si el botón conectado a PC1 está presionado
        if (!(PINC & (1 << PINC1)))
        {
            // Enciendo los LEDs correspondientes al estado actual en PB3 o PB4
            PORTB |= (state << 3);
        }

        // Si el botón conectado a PC0 está presionado
        if (!(PINC & (1 << PINC0)))
        {
            // Cambio al siguiente estado (1, 2 o 3)
            state = (state % 3) + 1;
            // Reinicio las secuencias y la dirección
            seq1 = 0b01010101;
            seq2 = 0b00000001;
            dir = 0;
        }

        // Comportamiento según el estado actual
        switch (state)
        {
        case 1:
            // Estado 1: Alternancia de LEDs en el Puerto D
            PORTD = seq1;   // Salida de la secuencia actual
            _delay_ms(100); // Retardo de 100 ms
            seq1 ^= 0xFF;   // Invierto los bits de la secuencia
            break;

        case 2:
            // Estado 2: Desplazamiento de un bit encendido hacia la izquierda
            PORTD = seq2;     // Salida de la secuencia actual
            _delay_ms(100);   // Retardo de 100 ms
            seq2 = seq2 << 1; // Desplazo el bit encendido hacia la izquierda
            if (seq2 == 0)    // Si la secuencia se desborda
            {
                seq2 = 0b00000001; // Reinicio la secuencia
            }
            break;

        case 3:
            // Estado 3: Desplazamiento de un bit encendido con cambio de dirección
            PORTD = seq2;   // Salida de la secuencia actual
            _delay_ms(100); // Retardo de 100 ms
            if (dir)        // Si la dirección es derecha
            {
                seq2 = seq2 >> 1; // Desplazo el bit encendido hacia la derecha
            }
            else // Si la dirección es izquierda
            {
                seq2 = seq2 << 1; // Desplazo el bit encendido hacia la izquierda
            }
            // Cambio de dirección si se alcanza el extremo
            if ((dir && seq2 == 1 << PORTD0) || (~dir && seq2 == 1 << PORTD7))
            {
                dir ^= 1; // Invierto la dirección
            }
            break;

        default:
            // Estado por defecto: Reinicio al estado 1
            state = 1;
            seq1 = 0b01010101;
            break;
        }
    }

    /* Punto de finalización del programa (NO se debe llegar a este lugar) */
    return 0;
}