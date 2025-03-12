#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bin_to_ascii(const char *binary, char *buffer){
    unsigned int num = strtol(binary, NULL, 2);
    sprintf(buffer, "%u", num);
}

int main(){
    char binary[9];
    char buffer[4];
    
    printf("Ingrese un numero binario de 8 bits: ");
    scanf("%8s", binary);

    bin_to_ascii(binary, buffer);
    printf("Numero en ASCII: %s\n", buffer);
    
    return 0;
}