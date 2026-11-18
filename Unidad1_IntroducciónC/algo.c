#include <stdio.h>
#include <stdlib.h>
int main() {
    int a=2;
    int b=4;
    int c=8;

    //como hago para que eso lo haga en tiempo de ejecucion y no en tiempo de compilacion?
    //Usando malloc para asignar memoria en tiempo de ejecucion
    
    int *DirA, *DirB, *DirC;
    
    DirA = &a;
    DirB = &b;
    DirC = &c;

    printf("Direccion de a: %p\n", DirA);
    printf("Direccion de b: %p\n", DirB);
    printf("Direccion de c: %p\n", DirC);
    
    return 0;
}