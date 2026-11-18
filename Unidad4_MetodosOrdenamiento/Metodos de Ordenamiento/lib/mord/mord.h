#ifndef MORD_H
#define MORD_H

/*
   mord.h - Libreria de Metodos de Ordenamiento (orden ascendente)

   Los metodos reciben:
        a      -> arreglo a ordenar
        n      -> cantidad de datos
        avance -> contador que el metodo va subiendo de 0 hasta n
                  para poder dibujar la barra de progreso.

   "volatile" le indica al compilador que esa variable la lee otro
   hilo (el main), asi que siempre debe escribirla en memoria.
*/


/* ---------- Funciones auxiliares ---------- */

void Intercambiar(int *a, int *b);
int  Particion(int *a, int bajo, int alto);
void mostrar_barra_de_progreso(const char *nombre, int actual, int n);


/* ---------- Metodos de ordenamiento ---------- */

void burbuja(int *a, int n, volatile int *avance);
void insercion(int *a, int n, volatile int *avance);
void seleccion(int *a, int n, volatile int *avance);
void quicksort(int *a, int n, volatile int *avance);
void quick_recursivo(int *a, int inicio, int fin, volatile int *avance);
void shellsort(int *a, int n, volatile int *avance);
void radix(int *a, int n, volatile int *avance);



/* ---------- Los 6 metodos al mismo tiempo (hilos) ---------- */

void ordenar_simultaneo(int *original, int n);

#endif
