#ifndef SIMDATA_H
#define SIMDATA_H

/* Utilerias para simular y manejar la coleccion de datos */

int *generar_datos(int n, int minimo, int maximo);
int *copiar_datos(int *origen, int n);
void imprimir_datos(int *a, int n);
int esta_ordenado(int *a, int n);

#endif
