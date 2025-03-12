#include <stdint.h>
#include <stdio.h>

uint8_t sumar(uint8_t N);

int main(void)

{
    uint8_t N = 10; 
    printf("El resultado de la suma es: %d\n", sumar(N));
    return 0;
}

uint8_t sumar(uint8_t N){
    uint8_t res = 0;
    for (uint8_t i=1; i<=N; i++){
        res+=i;
    }
    return res;
}