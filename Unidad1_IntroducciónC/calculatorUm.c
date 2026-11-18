/*Program Name: CalculatorUm.c
*Author: [Geisler]
*Descripcion: Este programa es una calculador que realiza
*operaciones basicas de suma, resta, multiplicacion y division.
*/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Prototipos de funciones
int suma();
int resta();
int multiplicacion();
float division();
void menu();
void ASCIArt();

//Funcion principal
int main() {
    setlocale(LC_ALL, "");
    ASCIArt();
    menu();
    return 0;
}

//Funciones
void ASCIArt(){
    int numeroRandom = rand() % 5 + 1;
    printf("\n=== CALCULADORA UM ===\n\n");
    switch (numeroRandom) {
         case 1:
            printf("  ____   ____\n");
            printf(" |  _ \\ |  _ \\\n");
            printf(" | |_) | | |_) |\n");
            printf(" |  __/  |  __/\n");
            printf(" |_|     |_|\n");
            break;
         case 2:
            printf("   ___   ___\n");
            printf("  / _ \\ / _ \\\n");
            printf(" | | | || | | |\n");
            printf(" | |_| || |_| |\n");
            printf("  \\___/  \\___/\n");
            break;
         case 3:
            printf("  _______\n");
            printf(" |  ___  |\n");
            printf(" | |_  | |\n");
            printf(" |  _| | |\n");
            printf(" | |___| |\n");
            printf(" |_______|\n");
            break;
         case 4:
            printf("  ______\n");
            printf(" |  __  |\n");
            printf(" | |  | |\n");
            printf(" | |__| |\n");
            printf(" |______/\n");
            break;
         case 5:
            printf("  ______\n");
            printf(" / ____/\n");
            printf("/ /     \n");
            printf("\\ \\____\n");
            printf(" \\____\\\n");
            break;
         default:
            printf("Error: Opción no válida\n");
            break;
    }
}

void menu() {
    int op = 0;

    printf("/*******************************/\n");
    printf("*      Calculadora Um.          *\n");
    printf("/*******************************/\n");
    printf("* Seleccione una opcion:         *\n");
    printf("*1. Suma                        *\n");
    printf("*2. Resta                       *\n");
    printf("*3. Multiplicacion              *\n");
    printf("*4. Division                    *\n");
    printf("*5. Salir                       *\n");
    printf("/*******************************/\n");

    scanf("%d", &op);
    switch(op){
        case 1:
            suma();
            break;
        case 2:
            resta();
            break;
        case 3:
            multiplicacion();
            break;
        case 4:
            division();
            break;
        case 5:
            printf("Saliendo...\n");
            break;
        default:
            printf("Error: Opcion no valida\n");
            break;
    }
}

int suma(){
    int x,y;
    printf("Ingrese el primer numero: ");
    scanf("%d",&x);
    printf("Ingrese el segundo numero: ");
    scanf("%d",&y);

    printf("El resultado de la suma es: %d\n", x+y);
}

int resta(){
    int x,y;
        printf("Ingrese el primer numero: ");
    scanf("%d",&x);
    printf("Ingrese el segundo numero: ");
    scanf("%d",&y);

    printf("El resultado de la resta es: %d\n", x-y);
}

int multiplicacion(){
    int x,y;
    printf("Ingrese el primer numero: ");
    scanf("%d",&x);
    printf("Ingrese el segundo numero: ");
    scanf("%d",&y);

    printf("El resultado de la multiplicacion es: %d\n", x*y);
}

float division(){
    float x,y;
    printf("Ingrese el primer numero: ");
    scanf("%f",&x);
    printf("Ingrese el segundo numero: ");
    scanf("%f",&y);

    if(y==0){
        printf("Error: No se puede dividir entre cero\n");
        return 0;
    }

    printf("El resultado de la division es: %.2f\n", x/y);
}