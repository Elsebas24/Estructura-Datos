#include <stdio.h>

int main() {
    int a = 3;
    printf("El valor contenido en el espacio de memoria %p es: %d\n", (void*)&a, a);
    return 0;
}