#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

// Estructura para compartir información y estado de cada hilo
typedef struct {
    int* arr;
    int n;
    volatile int progress;  // Progreso de 0 a 100%
    volatile int finished;  // Estado de finalización
    double execution_time;  // Tiempo transcurrido en segundos
} ThreadData;

int* generate_random_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) return NULL;
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Hilo de ordenamiento por Burbuja
void* worker_burbuja(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    int* arr = data->arr;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
        // Actualiza el progreso en el bucle exterior
        data->progress = (int)(((double)(j + 1) / n) * 100);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    data->execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    data->progress = 100;
    data->finished = 1;
    return NULL;
}

// Hilo de ordenamiento por Selección
void* worker_seleccion(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    int* arr = data->arr;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        if (min != i) {
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
        // Actualiza el progreso en el bucle exterior
        data->progress = (int)(((double)(i + 1) / (n - 1)) * 100);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    data->execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    data->progress = 100;
    data->finished = 1;
    return NULL;
}

// Función para imprimir barra de progreso utilizando ASCII 178
void print_progress_bar(const char* label, int progress, int width) {
    printf("%-12s [", label);
    int filled = (progress * width) / 100;
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            putchar((unsigned char)178); // Carácter ASCII 178
        } else {
            putchar(' ');
        }
    }
    printf("] %3d%%\n", progress);
}

int main() {
    int numero;
    srand((unsigned int)time(NULL));

    printf("Dame el numero de elementos del arreglo: ");
    if (scanf("%d", &numero) != 1 || numero <= 0) {
        fprintf(stderr, "Error: debes escribir un numero entero mayor que 0.\n");
        return 1;
    }

    int* arrBurbuja = generate_random_array(numero);
    int* arrSeleccion = generate_random_array(numero);

    if (arrBurbuja == NULL || arrSeleccion == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria.\n");
        free(arrBurbuja);
        free(arrSeleccion);
        return 1;
    }

    // Estructuras de datos para los hilos
    ThreadData dataBurbuja = {arrBurbuja, numero, 0, 0, 0.0};
    ThreadData dataSeleccion = {arrSeleccion, numero, 0, 0, 0.0};

    pthread_t thread1, thread2;

    printf("\nIniciando ordenamiento simultaneo con hilos...\n\n");

    // Dibuja las barras vacías por primera vez
    print_progress_bar("Burbuja", 0, 40);
    print_progress_bar("Selección", 0, 40);
    fflush(stdout);

    // Lanzar ambos algoritmos en hilos independientes
    pthread_create(&thread1, NULL, worker_burbuja, &dataBurbuja);
    pthread_create(&thread2, NULL, worker_seleccion, &dataSeleccion);

    // Bucle principal de renderizado de interfaz en tiempo real
    while (!dataBurbuja.finished || !dataSeleccion.finished) {
        usleep(30000); // Frecuencia de refresco: ~30ms

        // Reubica el cursor 2 líneas arriba e imprime de nuevo las barras
        printf("\033[2A\r");
        print_progress_bar("Burbuja", dataBurbuja.progress, 40);
        print_progress_bar("Selección", dataSeleccion.progress, 40);
        fflush(stdout);
    }

    // Esperar a la finalización de los hilos
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Render final al 100%
    printf("\033[2A\r");
    print_progress_bar("Burbuja", 100, 40);
    print_progress_bar("Selección", 100, 40);
    fflush(stdout);

    printf("\n--- Tiempos de ejecución ---\n");
    printf("Tiempo Burbuja:    %.6f segundos\n", dataBurbuja.execution_time);
    printf("Tiempo Selección:  %.6f segundos\n", dataSeleccion.execution_time);

    free(arrBurbuja);
    free(arrSeleccion);

    return 0;
}