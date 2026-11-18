#include <stdio.h>

int main() {
    int a,b;
    printf("Dame el primer numero para sumar:\n ");
    scanf("%d", &a);
    printf("Dame el segundo numero para sumar:\n ");
    scanf("%d", &b);
    //printf("El resultado de la suma es: %d\n", a + b);
    printf("%p\n",&a);
    printf("%p\n",&b);
    return 0;
}