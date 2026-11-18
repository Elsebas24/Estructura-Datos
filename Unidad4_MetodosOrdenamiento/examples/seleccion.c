#include<stdio.h>
#include<stdlib.h>
#include <time.h>

int* generate_random_array(int n){
    int* arr = (int*)malloc(n * sizeof(int));
    if(arr == NULL){
        return NULL;
    }
    for(int i=0; i<n; i++){
        arr[i] = rand() % 10000; 
    }
    return arr;
}

int* burbuja(int *arr, int n){
int temp;
for(int j=0;j<n;j++){
    for(int i=0; i<n-1; i++){
        if(arr[i]>arr[i+1]){
            temp= arr[i+1];
            arr[i+1]=arr[i];
            arr[i]=temp;
        }
    }
}
return arr;
}


int* Seleccion(int arr[], int n){
    for (int i = 0; i < n - 1; i++) {
        int min = i;                     // suponemos que el mínimo está en i

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min]) {
                min = j;                 // guardamos la posición del menor
            }
        }

        if (min != i) {                  // intercambiamos solo si hace falta
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
    
    return arr;
}
    

//main function
int main(){
    
    int numero;
    srand((unsigned int)time(NULL));

    printf("Dame el numero de elementos del arreglo: ");
    if(scanf("%d", &numero) != 1 || numero <= 0){
        fprintf(stderr, "Error: debes escribir un numero entero mayor que 0.\n");
        return 1;
    }

    printf("Generando arreglo aleatorio de %d elementos...\n", numero);
    int* arrBurbuja = generate_random_array(numero);
    int* arrSeleccion = generate_random_array(numero);
    if(arrBurbuja == NULL){
        fprintf(stderr, "Error: no se pudo reservar memoria para %d elementos.\n", numero);
        return 1;
    }
    if(arrSeleccion == NULL){
        fprintf(stderr, "Error: no se pudo reservar memoria para %d elementos.\n", numero);
        return 1;
    }

    printf("Arreglo generado para burbuja: ");
    /*for(int i=0; i<numero; i++){
        printf("%d ", arrBurbuja[i]);
    }*/
    printf("\n");
    //inicia
    clock_t inicioBurbuja = clock();
    int* arr_ordenadoBurbuja = burbuja(arrBurbuja, numero);
    clock_t finBurbuja = clock();

    clock_t inicioSeleccion = clock();
    int* arr_ordenadoSeleccion = Seleccion(arrSeleccion, numero);
    clock_t finSeleccion = clock();

    //finaliza
    /*printf("Arreglo ordenado: ");
    for(int i=0; i<numero; i++){
        printf("%d ", arr_ordenado[i]);
    }*/
    printf("\n");
    double segundos = (double)(finBurbuja - inicioBurbuja) / CLOCKS_PER_SEC;
    printf("Tiempo de ordenamiento: %.6f segundos\n", segundos);

    double segundosSeleccion = (double)(finSeleccion - inicioSeleccion) / CLOCKS_PER_SEC;
    printf("Tiempo de ordenamiento por seleccion: %.6f segundos\n", segundosSeleccion);
    
    free(arrBurbuja);
    free(arrSeleccion);
    return 0;
}