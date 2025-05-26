/*
Realice una implementación en pseudocódigo de un driver para el periférico UART aplicando el modelo
productor-consumidor.

Transmisión y recepción por interrupción (Foreground) y procesamiento en superloop (Background)
*/


// Buffers circulares
char RX_Buffer[64];
char TX_Buffer[64];
volatile int RX_Head = 0, RX_Tail = 0;
volatile int TX_Head = 0, TX_Tail = 0;
volatile bool TX_Ready = true;

// ISR: Se ejecuta al recibir un byte
ISR(USART_RX_vect) {
    char data = UDR0;
    int next_head = (RX_Head + 1) % 64;
    if (next_head != RX_Tail) {
        RX_Buffer[RX_Head] = data;
        RX_Head = next_head;
    }
}

// ISR: Se ejecuta cuando se vacía el registro de transmisión
ISR(USART_UDRE_vect) {
    if (TX_Head != TX_Tail) {
        UDR0 = TX_Buffer[TX_Tail];
        TX_Tail = (TX_Tail + 1) % 64;
    } else {
        // Fin de transmisión
        TX_Ready = true;
        UCSR0B &= ~(1 << UDRIE0); // Deshabilitar interrupción UDRE
    }
}

// Función pública para enviar datos
void UART_Send(char c) {
    int next_head = (TX_Head + 1) % 64;
    while (next_head == TX_Tail); // Espera si buffer lleno (bloqueante simple)

    TX_Buffer[TX_Head] = c;
    TX_Head = next_head;

    // Habilitar interrupción de transmisión si estaba deshabilitada
    if (TX_Ready) {
        TX_Ready = false;
        UCSR0B |= (1 << UDRIE0);
    }
}

// Función para leer datos disponibles en RX buffer
bool UART_Available() {
    return RX_Head != RX_Tail;
}

char UART_Read() {
    if (UART_Available()) {
        char c = RX_Buffer[RX_Tail];
        RX_Tail = (RX_Tail + 1) % 64;
        return c;
    }
    return 0; // o error
}

// En el main (Background):
void main() {
    UART_Init(); // configura UART, interrupciones
    sei(); // habilita interrupciones globales

    while (1) {
        if (UART_Available()) {
            char c = UART_Read();
            ProcesarDato(c);
        }
        // otras tareas en background
    }
}
