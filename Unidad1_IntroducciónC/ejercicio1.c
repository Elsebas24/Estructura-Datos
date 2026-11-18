#include <stdio.h>
#include <stdbool.h>

int main() {
    char nombre[12];
    int a = 5,b = 10;
    float c = 3.2584;
    double e = 3.2584;
    bool f = true;

    printf("Dame tu nombre:\n ");
    scanf("%s", nombre);
    printf("Hola %s\n", nombre);
    return 0;

    if(a<b){
        printf("a es menor que b\n");
    } else{
        printf("a es mayor que b\n");
    }

    for (int i = 0; i < 10; i++){
        printf("El valor de i es: %d\n", i);
    }

    while(a<b){
        printf("el valor de a es: %d\n", a);
        a++;
    }

    do{
        printf("el valor de a es: %d\n", a);
        a++;
    } while(a<b);

    switch(a){
        case 5:
            printf("a es igual a 5\n");
            break;
        case 6:
            printf("a es igual a 6\n");
            break;
        default:
            printf("El valor de a es otro\n");
    }

    int suma(int a, int b); {
        return a + b;
    }
}