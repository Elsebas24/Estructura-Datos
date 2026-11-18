/*
   mo.c - Programa principal
   Ordena la misma coleccion de enteros al mismo tiempo con
   Burbuja, Insercion, Seleccion, Quicksort, ShellSort y Radix.
   Todo el trabajo esta en las librerias lib/mord y lib/simdata.

   Compilar:
        gcc mo.c lib/mord/mord.c lib/simdata/simdata.c -o ordenar -lpthread
   Ejecutar:
        .\ordenar.exe    (PowerShell)
        ./ordenar        (w64devkit / Linux)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "lib/mord/mord.h"
#include "lib/simdata/simdata.h"

#define RESET    "\033[0m"
#define ROJO     "\033[31m"
#define VERDE    "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL     "\033[34m"
#define MAGENTA  "\033[35m"
#define CIAN     "\033[36m"
#define BLANCO   "\033[37m"


/* Prepara la consola de Windows para colores, subir el cursor y el caracter ▓ */
void preparar_consola()
{
#ifdef _WIN32
        HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD modo;

        GetConsoleMode(consola, &modo);
        SetConsoleMode(consola, modo | 0x0004);
        SetConsoleOutputCP(65001);
#endif
}


void begin()
{
        system("cls");
        printf(ROJO"╔══════════════════════════════════════════════════════════════════════════╗\n");
        printf(ROJO"║              MÉTODOS DE ORDENAMIENTO EN C                                ║\n");
        printf(ROJO"║                                                                          ║\n");
        printf(ROJO"╠══════════════════════════════════════════════════════════════════════════╣\n");
        printf(ROJO"║                    ..▄███▄███▄                                           ║\n");
        printf(ROJO"║                    ..█████████                                           ║\n");
        printf(ROJO"║                    ..::▀█████▀.                                          ║\n");
        printf(ROJO"║                    ..:..▀█▀                                              ║\n");
        printf(ROJO"║                    ..:..█.                                               ║\n");
        printf(ROJO"║                    ..:.█                                                 ║\n");
        printf(ROJO"║                    ...█                                                  ║\n");
        printf(ROJO"║                    ..█....███████                                        ║\n");
        printf(ROJO"║                    .██...██##███#██:                                     ║\n");
        printf(ROJO"║                    ██...█######█#████                                    ║\n");
        printf(ROJO"║                    ██..██###(o)##█#█#█                                   ║\n");
        printf(ROJO"║                    ███###█#####█#█####█                                  ║\n");
        printf(ROJO"║                    ▀██##█.██####██#####█                                 ║\n");
        printf(ROJO"║                    .▀██▀..█#############█                                ║\n");
        printf(ROJO"║                    ....:...█#####█######█                                ║\n");
        printf(ROJO"║                    ....:...█####█#█#####█                                ║\n");
        printf(ROJO"║                    .:..:...█###█###█####█                                ║\n");
        printf(ROJO"║                    .:..:...█###█...█###█                                 ║\n");
        printf(ROJO"║                    .:..:..██#██...██##██                                 ║\n");
        printf(ROJO"║                    █████████████████████                                 ║\n");
        printf(ROJO"║                                                                          ║\n");
        printf(VERDE"║   ___          _                                 _        _              ║\n");
        printf(VERDE"║  / _ \\ _ _  __| |___ _ _  __ _ _ __  _  ___ _ _ | |_ ___ | |             ║\n");
        printf(VERDE"║ | (_) | '_|/ _` / -_) ' \\/ _` | '  \\| || -_) ' \\|  _/ _ \\|_|             ║\n");
        printf(VERDE"║  \\___/|_|  \\__,_\\___|_||_\\__,_|_|_|_|\\_,_\\___|_||_|\\__\\___/(_)             ║\n");
        printf(ROJO"║                                                                          ║\n");
        printf(ROJO"╚══════════════════════════════════════════════════════════════════════════╝\n");

}


int main()
{
        int n;
        int *datos;

        preparar_consola();
        srand(time(NULL));

        begin();

        printf(BLANCO "Cuantos datos desea ordenar? ");
        scanf("%d", &n);

        if (n <= 0)
        {
                printf("La cantidad debe ser mayor que 0\n");
                return 1;
        }

        datos = generar_datos(n, -1000, 1000);

        if (n <= 30)
        {
                printf("\nDatos originales:\n");
                imprimir_datos(datos, n);
        }

        ordenar_simultaneo(datos, n);

        free(datos);
        printf(RESET);

        return 0;
}
