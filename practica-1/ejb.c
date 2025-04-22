#include <stdio.h>

void invertirArray(int *array, size_t array_size_bytes) {
    size_t array_size = array_size_bytes / sizeof(int);

    for (size_t i = 0; i<array_size/2; i++){
        int aux=array[i];
        array[i]=array[array_size-1-i];
        array[array_size-1-i]=aux;
    }
}

int main(){
    int array[] = {1,2,3,4,5,6,7,8,9,10};
    size_t array_size_bytes = sizeof(array);

    printf("Original array: ");
    for (size_t i = 0; i<array_size_bytes/sizeof(int); i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    invertirArray(array, array_size_bytes);

    printf("Inverted array: ");
    for (size_t i = 0; i<array_size_bytes/sizeof(int); i++){
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}