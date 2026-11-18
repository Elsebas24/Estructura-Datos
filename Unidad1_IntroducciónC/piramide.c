#include <stdio.h>

int main() {
    int i,j,k,n;
    printf("Ingrese el número de filas de la pirámide: ");
    scanf("%d", &n);

    //Crear espacios y asteriscos para formar la pirámide

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n - i; j++) {
            printf(" ");
        }
        for (k = 1; k <= 2 * i - 1; k++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}