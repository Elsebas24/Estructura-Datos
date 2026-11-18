#include <stdio.h>
int main() {
    char nombre[30];
    printf("Ingrese su nombre: ");
    scanf("%s", nombre);
    for(int i = 0; i < 30; i++) {
        printf("Memoria %p, valor: %c\n", (void*)&nombre[i], nombre[i]);
    }
    return 0;
}