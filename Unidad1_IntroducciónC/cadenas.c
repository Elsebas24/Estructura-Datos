#include<stdio.h>
#include<string.h>

int strlen2(char *cadena) {
    int i = 0;
    while(cadena[i] != '\0') {
        i++;
    }
    return i;
}

int main() {
    char cadena[]="Hola Mundo";
    int longitud = strlen2(cadena);
    int longitud_lib = strlen(cadena);
    printf("La longitud de la cadena es: %d\n", longitud);
    printf("La longitud de la cadena es: %d\n", longitud_lib);
    return 0;
}