#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void send_serial(unsigned char num){
    for (int i=0; i<8; i++){
        printf("%d\n", (num>>i)&1);
        fflush(stdout);
        sleep(1);
    }
}

int main() {
    unsigned char num;

    printf("Ingrese un numero (0-255): ");
    scanf("%hhu", &num);

    printf("Enviando bits en serie:\n");
    send_serial(num);

    return 0;
}