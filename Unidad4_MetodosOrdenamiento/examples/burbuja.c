#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* generate_random_array(int n){
    int *arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) return NULL;

    for(int i = 0; i < n; i++){
        arr[i] = rand() % 10000; 
    }
    return arr;
}

int* burbuja(int *arr, int n){
    int temp;
    for(int j = 0; j < n; j++){
        for(int i = 0; i < n - 1; i++){
            if(arr[i] > arr[i + 1]){
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
    return arr;
}

int main(){
    srand(time(NULL));
    int numero;

    printf("Dame el numero de elementos del arreglo: ");
    scanf("%d", &numero);

    int* arr = generate_random_array(numero);
    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    printf("Arreglo generado: ");
    
    for(int i = 0; i < numero; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    clock_t inicio = clock();
    int* arr_ordenado = burbuja(arr, numero);
    clock_t fin = clock();
    printf("Arreglo ordenado: ");
    for(int i = 0; i < numero; i++){
        printf("%d ", arr_ordenado[i]);
    }
    printf("\n");
    double segundos = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ordenamiento: %.6f segundos\n", segundos);
    printf("Tiempo de ejecucion: %f segundos\n", ((double)(fin - inicio)) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}