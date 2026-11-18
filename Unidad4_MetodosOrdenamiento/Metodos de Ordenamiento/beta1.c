#include <stdio.h>

void mostrar_barra_progreso(int actual, int n){
    const int ancho = 40; // Ancho de la barra de progreso
    float porcentaje = (float)actual / n;
    int llenos = (int)(porcentaje * ancho);


    printf("\r[");
    for (int i = 0; i < ancho; i++) {
        if (i < llenos) {
            printf("▓");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%", (int)(porcentaje * 100.0f));

    fflush(stdout);
}