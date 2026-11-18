#include <stdio.h>
#include <stdlib.h>
#include "simdata.h"


/* Crea un arreglo dinamico con n enteros aleatorios entre minimo y maximo */
int *generar_datos(int n, int minimo, int maximo)
{
        int i;
        int *a = (int *) malloc(n * sizeof(int));

        for (i = 0; i < n; i++)
                a[i] = minimo + rand() % (maximo - minimo + 1);

        return a;
}


/* Regresa una copia nueva del arreglo (cada metodo ordena su propia copia) */
int *copiar_datos(int *origen, int n)
{
        int i;
        int *copia = (int *) malloc(n * sizeof(int));

        for (i = 0; i < n; i++)
                copia[i] = origen[i];

        return copia;
}


void imprimir_datos(int *a, int n)
{
        int i;

        for (i = 0; i < n; i++)
                printf("%d ", a[i]);

        printf("\n");
}


/* Regresa 1 si el arreglo quedo en orden ascendente, 0 si no */
int esta_ordenado(int *a, int n)
{
        int i;

        for (i = 0; i < n - 1; i++)
        {
                if (a[i] > a[i + 1])
                        return 0;
        }

        return 1;
}
