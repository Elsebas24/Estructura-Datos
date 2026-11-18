#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ---------------------------------------------------------
// Nodo de la lista enlazada: cada nodo representa un disco.
// El campo 'dato' guarda el tamano del disco (entre mas grande el numero, mas grande el disco).
// ---------------------------------------------------------
typedef struct Nodo {
    int dato;
    struct Nodo *next;
} Nodo;

// ---------------------------------------------------------
// Pila dinamica: no usa arreglo de tamano fijo, sino que va creando nodos en memoria (malloc) conforme se necesitan.
// 'top' apunta al nodo que esta hasta arriba de la pila.
// 'nombre' es solo para identificar la torre al imprimir (A, B o C).
// ---------------------------------------------------------
typedef struct {
    Nodo *top;
    int size;
    char nombre;
} StackDinamic;

// Inicializa la pila vacia y le asigna un nombre (para imprimirla despues)
void initStack(StackDinamic *s, char nombre) {
    s->top = NULL;
    s->size = 0;
    s->nombre = nombre;
}

// Una pila dinamica esta vacia cuando su puntero top no apunta a ningun nodo
bool isEmpty(StackDinamic *s) {
    return s->top == NULL;
}

// Inserta un disco en la pila (operacion push).
// Se crea un nuevo nodo en memoria dinamica, se enlaza apuntando
// al nodo que antes era el tope, y ese nuevo nodo pasa a ser el tope.
bool push(StackDinamic *s, int value) {
    Nodo *newNode = (Nodo *)malloc(sizeof(Nodo));
    if (newNode == NULL) {
        return false; // No hay memoria disponible
    }
    newNode->dato = value;
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
    return true;
}

// Retira y devuelve el disco que esta hasta arriba de la pila (operacion pop).
// Si la pila esta vacia, es un error grave del algoritmo (no deberia pasar
// nunca en una ejecucion correcta de Torres de Hanoi), por lo que se detiene
// el programa con un mensaje claro.
int pop(StackDinamic *s) {
    if (isEmpty(s)) {
        fprintf(stderr, "ERROR: se intento retirar un disco de la torre %c, pero esta vacia.\n", s->nombre);
        exit(EXIT_FAILURE);
    }
    Nodo *temp = s->top;
    int value = temp->dato;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return value;
}

// Consulta cual es el disco que esta hasta arriba, SIN retirarlo.
// Se usa para el caso borde de validar movimientos.
int returnTop(StackDinamic *s) {
    if (isEmpty(s)) {
        return -1; // -1 indica "no hay disco" (pila vacia)
    }
    return s->top->dato;
}

// ---------------------------------------------------------
// Imprime el contenido de una torre, de arriba hacia abajo,
// para poder ver el estado completo del juego en cada paso.
// ---------------------------------------------------------
void displayStack(StackDinamic *s) {
    printf("  Torre %c: ", s->nombre);
    if (isEmpty(s)) {
        printf("(vacia)\n");
        return;
    }
    Nodo *actual = s->top;
    while (actual != NULL) {
        printf("%d ", actual->dato);
        actual = actual->next;
    }
    printf("\n");
}

// Imprime el estado de las tres torres siempre en el mismo orden
// (A, B, C), sin importar cuales de ellas son "origen/destino/auxiliar"
// en la llamada recursiva actual. Esto hace mas facil leer el progreso
// del juego en el informe, ya que el orden nunca cambia.
void mostrarEstadoOrdenado(StackDinamic *x, StackDinamic *y, StackDinamic *z) {
    StackDinamic *torres[3] = { x, y, z };
    // ordenamiento simple (3 elementos) por nombre de torre: A, B, C
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            if (torres[j]->nombre > torres[j + 1]->nombre) {
                StackDinamic *temp = torres[j];
                torres[j] = torres[j + 1];
                torres[j + 1] = temp;
            }
        }
    }
    displayStack(torres[0]);
    displayStack(torres[1]);
    displayStack(torres[2]);
}

// ---------------------------------------------------------
// Algoritmo recursivo de las Torres de Hanoi.
//
// Idea general:
//   - Si solo hay 1 disco, se mueve directo de origen a destino.
//   - Si hay N discos:
//       1) Se mueven los N-1 discos de arriba de "origen" hacia
//          "auxiliar" (usando "destino" como apoyo temporal).
//       2) Se mueve el disco mas grande (el que quedo solo en
//          "origen") directamente hacia "destino".
//       3) Se mueven los N-1 discos que estan en "auxiliar"
//          hacia "destino" (usando "origen" como apoyo temporal).
//
// Cada movimiento individual se hace con pop() en la torre
// origen y push() en la torre destino, es decir, el algoritmo
// completo esta construido sobre las operaciones de la pila.
// ---------------------------------------------------------
void hanoi(int n, StackDinamic *origen, StackDinamic *destino, StackDinamic *auxiliar) {
    if (n == 0) {
        return; // Caso base: no hay discos que mover
    }
    hanoi(n - 1, origen, auxiliar, destino);

    int disco = pop(origen);

    // Caso borde: se valida que el movimiento respete la regla del
    // juego (no colocar un disco grande sobre uno chico). En unaZ|
    // ejecucion correcta del algoritmo esto nunca deberia fallar,
    // pero se deja como medida de seguridad ante posibles errores.
    int topeDestino = returnTop(destino);
    if (topeDestino != -1 && disco > topeDestino) {
        fprintf(stderr, "ERROR: movimiento invalido, el disco %d no puede ir sobre el disco %d.\n", disco, topeDestino);
        exit(EXIT_FAILURE);
    }

    push(destino, disco);
    printf("Mover disco %d: Torre %c -> Torre %c\n", disco, origen->nombre, destino->nombre);
    printf("Estado actual:\n");
    mostrarEstadoOrdenado(origen, auxiliar, destino);
    printf("\n");

    hanoi(n - 1, auxiliar, destino, origen);
}

// ---------------------------------------------------------
// Llena la torre de origen con N discos, del mas grande (N)
// en la base hasta el mas chico (1) hasta arriba, que es el
// estado inicial correcto del juego.
// ---------------------------------------------------------
void inicializarTorre(StackDinamic *torre, int n) {
    for (int disco = n; disco >= 1; disco--) {
        push(torre, disco);
    }
}

int main() {
    int n;

    printf("===== TORRES DE HANOI CON PILAS DINAMICAS =====\n");
    printf("Ingresa el numero de discos (N): ");
    scanf("%d", &n);

    // Caso borde: N invalido (0 o negativo)
    if (n <= 0) {
        printf("N debe ser un numero entero positivo. No hay discos que mover.\n");
        return 0;
    }

    StackDinamic torreA, torreB, torreC;
    initStack(&torreA, 'A');
    initStack(&torreB, 'B');
    initStack(&torreC, 'C');

    inicializarTorre(&torreA, n);

    printf("\nEstado inicial:\n");
    mostrarEstadoOrdenado(&torreA, &torreB, &torreC);
    printf("\n");

    hanoi(n, &torreA, &torreC, &torreB);

    printf("Estado final:\n");
    mostrarEstadoOrdenado(&torreA, &torreB, &torreC);

    long totalMovimientos = 1;
    for (int i = 0; i < n; i++) totalMovimientos *= 2;
    totalMovimientos -= 1;
    printf("\nTotal de movimientos realizados: %ld (2^N - 1, con N = %d)\n", totalMovimientos, n);

    return 0;
}