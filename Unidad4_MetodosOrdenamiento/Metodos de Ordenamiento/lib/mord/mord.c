#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "mord.h"
#include "../simdata/simdata.h"


/* =================== FUNCIONES AUXILIARES =================== */

/* Intercambia el valor de dos posiciones del arreglo */
void Intercambiar(int *a, int *b)
{
        int aux = *a;

        *a = *b;
        *b = aux;
}


/*
   Particion (usada por Quicksort):
   toma como pivote el dato de en medio, deja a la izquierda los
   menores o iguales y a la derecha los mayores, y regresa la
   posicion final donde quedo el pivote.
*/
int Particion(int *a, int bajo, int alto)
{
        int medio = (bajo + alto) / 2;
        int pivote, i, j;

        /* El pivote de en medio se pasa al final para trabajar con el */
        Intercambiar(&a[medio], &a[alto]);
        pivote = a[alto];

        i = bajo - 1;

        for (j = bajo; j < alto; j++)
        {
                if (a[j] <= pivote)
                {
                        i++;
                        Intercambiar(&a[i], &a[j]);
                }
        }

        Intercambiar(&a[i + 1], &a[alto]);

        return i + 1;
}


/*
   Dibuja una linea como:
        Burbuja    [▓▓▓▓▓▓▓▓▓▓▓▓------------]  45.3%
   No imprime salto de linea, para que el main pueda agregar el tiempo.
*/
void mostrar_barra_de_progreso(const char *nombre, int actual, int n)
{
        int ancho = 30;         /* largo de la barra en caracteres */
        int i, llenos;
        double porcentaje;

        porcentaje = (double) actual * 100.0 / n;
        llenos = (int) (porcentaje * ancho / 100.0);

        printf("%-10s [", nombre);

        for (i = 0; i < ancho; i++)
        {
                if (i < llenos)
                        printf("▓");
                else
                        printf("-");
        }

        printf("] %5.1f%%", porcentaje);
}


/* =================== METODOS DE ORDENAMIENTO =================== */


/* ---------------- Burbuja ----------------
   En cada pasada el mayor "sube" al final:
   despues de la pasada i ya hay i+1 datos en su lugar. */

void burbuja(int *a, int n, volatile int *avance)
{
        int i, j;

        for (i = 0; i < n - 1; i++)
        {
                for (j = 0; j < n - 1 - i; j++)
                {
                        if (a[j] > a[j + 1])
                                Intercambiar(&a[j], &a[j + 1]);
                }

                *avance = i + 1;
        }

        *avance = n;
}


/* ---------------- Insercion ----------------
   Toma cada dato y lo inserta en su lugar dentro
   de la parte que ya esta ordenada (a la izquierda). */

void insercion(int *a, int n, volatile int *avance)
{
        int i, j, clave;

        for (i = 1; i < n; i++)
        {
                clave = a[i];
                j = i - 1;

                while (j >= 0 && a[j] > clave)
                {
                        a[j + 1] = a[j];
                        j--;
                }

                a[j + 1] = clave;
                *avance = i;
        }

        *avance = n;
}


/* ---------------- Seleccion ----------------
   Busca el menor de lo que falta y lo pone al inicio. */

void seleccion(int *a, int n, volatile int *avance)
{
        int i, j, menor;

        for (i = 0; i < n - 1; i++)
        {
                menor = i;

                for (j = i + 1; j < n; j++)
                {
                        if (a[j] < a[menor])
                                menor = j;
                }

                Intercambiar(&a[i], &a[menor]);
                *avance = i + 1;
        }

        *avance = n;
}


/* ---------------- Quicksort ----------------
   Parte el arreglo con Particion() y ordena cada lado.
   El avance cuenta cuantos datos ya quedaron en su lugar final:
   cada pivote cuenta 1 y cada pedazo de un solo dato cuenta 1. */

void quick_recursivo(int *a, int inicio, int fin, volatile int *avance)
{
        int p;

        if (inicio > fin)
                return;

        if (inicio == fin)
        {
                *avance = *avance + 1;
                return;
        }

        p = Particion(a, inicio, fin);
        *avance = *avance + 1;          /* el pivote ya quedo en su lugar */

        quick_recursivo(a, inicio, p - 1, avance);
        quick_recursivo(a, p + 1, fin, avance);
}

void quicksort(int *a, int n, volatile int *avance)
{
        *avance = 0;
        quick_recursivo(a, 0, n - 1, avance);
        *avance = n;
}


/* ---------------- ShellSort ----------------
   Como insercion, pero comparando datos separados por un "salto"
   que se va haciendo mas chico hasta llegar a 1. */

void shellsort(int *a, int n, volatile int *avance)
{
        int salto, i, j, aux;
        int total_saltos = 0, paso = 0;

        for (salto = n / 2; salto > 0; salto = salto / 2)
                total_saltos++;

        for (salto = n / 2; salto > 0; salto = salto / 2)
        {
                for (i = salto; i < n; i++)
                {
                        aux = a[i];
                        j = i;

                        while (j >= salto && a[j - salto] > aux)
                        {
                                a[j] = a[j - salto];
                                j = j - salto;
                        }

                        a[j] = aux;

                        *avance = (int) (((long long) paso * n + i) / total_saltos);
                }

                paso++;
        }

        *avance = n;
}


/* ---------------- Radix (base 10) ----------------
   Ordena digito por digito (unidades, decenas, centenas...).
   Para aceptar negativos, primero se le resta el minimo a todos. */

void radix(int *a, int n, volatile int *avance)
{
        int i, minimo, maximo, exp, digito;
        int pasadas = 0, pasada = 0;
        int cuenta[10];
        int *salida;

        if (n <= 1)
        {
                *avance = n;
                return;
        }

        minimo = a[0];
        for (i = 1; i < n; i++)
        {
                if (a[i] < minimo)
                        minimo = a[i];
        }

        for (i = 0; i < n; i++)
                a[i] = a[i] - minimo;

        maximo = a[0];
        for (i = 1; i < n; i++)
        {
                if (a[i] > maximo)
                        maximo = a[i];
        }

        /* Cuantos digitos tiene el maximo = cuantas pasadas habra */
        for (i = maximo; i > 0; i = i / 10)
                pasadas++;

        salida = (int *) malloc(n * sizeof(int));
        exp = 1;

        for (pasada = 0; pasada < pasadas; pasada++)
        {
                for (i = 0; i < 10; i++)
                        cuenta[i] = 0;

                for (i = 0; i < n; i++)
                {
                        digito = (a[i] / exp) % 10;
                        cuenta[digito]++;
                }

                for (i = 1; i < 10; i++)
                        cuenta[i] = cuenta[i] + cuenta[i - 1];

                for (i = n - 1; i >= 0; i--)
                {
                        digito = (a[i] / exp) % 10;
                        cuenta[digito]--;
                        salida[cuenta[digito]] = a[i];
                }

                for (i = 0; i < n; i++)
                        a[i] = salida[i];

                *avance = (int) ((long long) n * (pasada + 1) / pasadas);

                if (pasada < pasadas - 1)
                        exp = exp * 10;
        }

        for (i = 0; i < n; i++)
                a[i] = a[i] + minimo;

        free(salida);
        *avance = n;
}


/* =================== ORDENAMIENTO SIMULTANEO (HILOS) =================== */

/* Informacion que recibe cada hilo */
typedef struct
{
        char nombre[20];
        int metodo;               /* 1..6, ver funcion hilo_ordenar() */
        int *datos;
        int n;
        volatile int avance;      /* de 0 a n, lo sube el metodo */
        volatile int terminado;   /* 1 cuando acaba */
        double segundos;
} Tarea;


/* Tiempo actual en segundos (reloj real, sirve con hilos) */
double ahora()
{
        struct timespec t;

        clock_gettime(CLOCK_MONOTONIC, &t);
        return t.tv_sec + t.tv_nsec / 1000000000.0;
}


/* Funcion que ejecuta cada hilo */
void *hilo_ordenar(void *arg)
{
        Tarea *t = (Tarea *) arg;
        double inicio = ahora();

        if (t->metodo == 1)
                burbuja(t->datos, t->n, &t->avance);
        else if (t->metodo == 2)
                insercion(t->datos, t->n, &t->avance);
        else if (t->metodo == 3)
                seleccion(t->datos, t->n, &t->avance);
        else if (t->metodo == 4)
                quicksort(t->datos, t->n, &t->avance);
        else if (t->metodo == 5)
                shellsort(t->datos, t->n, &t->avance);
        else
                radix(t->datos, t->n, &t->avance);

        t->segundos = ahora() - inicio;
        t->terminado = 1;

        return NULL;
}


/* Dibuja la barra de un metodo y le agrega el tiempo cuando ya termino */
void dibujar_barra(Tarea *t)
{
        mostrar_barra_de_progreso(t->nombre, t->avance, t->n);

        if (t->terminado)
                printf("  %.4f s", t->segundos);
        else
                printf("           ");

        printf("\n");
}


/*
   Ordena la misma coleccion con los 6 metodos al mismo tiempo
   (un hilo por metodo), dibuja las barras de progreso y al final
   muestra la tabla de tiempos. El arreglo "original" no se modifica.
*/
void ordenar_simultaneo(int *original, int n)
{
        int i, todos_terminaron;
        int metodos = 6;
        pthread_t *hilos;
        Tarea tareas[] = {
                { "Burbuja",   1 },
                { "Insercion", 2 },
                { "Seleccion", 3 },
                { "Quicksort", 4 },
                { "ShellSort", 5 },
                { "Radix",     6 }
        };

        hilos = (pthread_t *) malloc(metodos * sizeof(pthread_t));

        /* Cada metodo recibe su propia copia de los mismos datos */
        for (i = 0; i < metodos; i++)
        {
                tareas[i].datos = copiar_datos(original, n);
                tareas[i].n = n;
                tareas[i].avance = 0;
                tareas[i].terminado = 0;
        }

        printf("\nOrdenando de manera simultanea...\n\n");

        for (i = 0; i < metodos; i++)
                pthread_create(&hilos[i], NULL, hilo_ordenar, &tareas[i]);

        printf("\033[?25l");      /* esconde el cursor mientras se dibujan las barras */

        /* Mientras los hilos trabajan, se redibujan las barras */
        do
        {
                todos_terminaron = 1;

                for (i = 0; i < metodos; i++)
                {
                        dibujar_barra(&tareas[i]);

                        if (!tareas[i].terminado)
                                todos_terminaron = 0;
                }

                if (!todos_terminaron)
                {
                        usleep(100000);                 /* 0.1 segundos */
                        printf("\033[%dA", metodos);    /* sube el cursor para redibujar */
                }

        } while (!todos_terminaron);

        printf("\033[?25h");      /* vuelve a mostrar el cursor */

        for (i = 0; i < metodos; i++)
                pthread_join(hilos[i], NULL);

        printf("\n%-12s %-14s %s\n", "Metodo", "Tiempo (s)", "Ordenado");
        printf("-----------------------------------------\n");

        for (i = 0; i < metodos; i++)
        {
                printf("%-12s %-14.6f %s\n",
                       tareas[i].nombre,
                       tareas[i].segundos,
                       esta_ordenado(tareas[i].datos, n) ? "Si" : "No");
        }

        if (n <= 30)
        {
                printf("\nDatos ordenados:\n");
                imprimir_datos(tareas[0].datos, n);
        }

        for (i = 0; i < metodos; i++)
                free(tareas[i].datos);

        free(hilos);
}
