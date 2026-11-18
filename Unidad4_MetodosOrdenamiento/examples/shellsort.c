#include <stdio.h>
#include <stdlib.h>

int* shell_sort(int* arr, int n) {
    if (arr == NULL || n <= 1) {
        return arr;
    }

    // Comienza con una brecha (gap) grande y la reduce a la mitad en cada iteración
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Realiza un ordenamiento por inserción para el tamaño de brecha actual
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            // Desplaza los elementos que son mayores que temp hacia adelante
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            // Coloca el valor en su posición correspondiente
            arr[j] = temp;
        }
    }

    return arr;
}
int main() {
    int arr[] = {12, 34, 54, 2, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    shell_sort(arr, n);

    printf("Array ordenado: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}