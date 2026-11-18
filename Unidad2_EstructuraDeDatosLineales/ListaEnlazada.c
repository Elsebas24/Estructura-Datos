/*
   Sistema de inventario de una biblioteca con lista enlazada simple
   Autor: Geisler Jimenez Torres
   Proposito: gestionar los libros de una biblioteca. Permite agregar,
              buscar, prestar, devolver, eliminar y mostrar el inventario.
*/

//Bibliotecas
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Libro {
    char isbn[20];       // Formato: 978-3-16-148410-0 (20 para que quepa completo)
    char titulo[100];
    char autor[50];
    int anio;
    int estado;          // 0: Disponible, 1: Prestado
    struct Libro *siguiente;
} Libro;

// Recorre la lista comparando ISBN. Devuelve el libro o NULL si no esta.
Libro *buscarLibro(Libro *inicio, char isbn[]) {

    Libro *actual = inicio;

    while (actual != NULL) {

        if (strcmp(actual->isbn, isbn) == 0) {   // strcmp devuelve 0 si son iguales
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;   // No encontrado
}

// Agrega un libro al final de la lista
void insertarLibro(Libro **inicio, char isbn[], char titulo[], char autor[], int anio) {

    if (buscarLibro(*inicio, isbn) != NULL) {   // Validacion: no se repiten ISBN
        printf("Ya existe un libro con ese ISBN.\n");
        return;
    }

    Libro *nuevo = (Libro *)malloc(sizeof(Libro));

    if (nuevo == NULL) {
        printf("No hay memoria disponible.\n");
        return;
    }

    strcpy(nuevo->isbn, isbn);       // strcpy copia el texto (destino, origen)
    strcpy(nuevo->titulo, titulo);
    strcpy(nuevo->autor, autor);
    nuevo->anio = anio;
    nuevo->estado = 0;               // Todo libro nuevo entra disponible
    nuevo->siguiente = NULL;

    if (*inicio == NULL) {           // Lista vacia: el nuevo es el primero
        *inicio = nuevo;

    } else {                         // Si no, se recorre hasta el ultimo
        Libro *actual = *inicio;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }

    printf("Libro agregado al inventario.\n");
}

// Saca un libro de la lista y libera su memoria
void eliminarLibro(Libro **inicio, char isbn[]) {

    Libro *actual = *inicio;
    Libro *anterior = NULL;

    while (actual != NULL) {

        if (strcmp(actual->isbn, isbn) == 0) {

            if (anterior == NULL) {                    // Es el primero de la lista
                *inicio = actual->siguiente;
            } else {                                   // Va en medio o al final
                anterior->siguiente = actual->siguiente;
            }

            printf("Libro eliminado: %s\n", actual->titulo);
            free(actual);                              // Se libera la memoria
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    printf("No se encontro ningun libro con ese ISBN.\n");
}

// Cambia el estado del libro a prestado
void prestarLibro(Libro *inicio, char isbn[]) {

    Libro *libro = buscarLibro(inicio, isbn);

    if (libro == NULL) {
        printf("No se encontro ningun libro con ese ISBN.\n");

    } else if (libro->estado == 1) {
        printf("Ese libro ya esta prestado.\n");

    } else {
        libro->estado = 1;
        printf("Prestado: %s\n", libro->titulo);
    }
}

// Cambia el estado del libro a disponible
void devolverLibro(Libro *inicio, char isbn[]) {

    Libro *libro = buscarLibro(inicio, isbn);

    if (libro == NULL) {
        printf("No se encontro ningun libro con ese ISBN.\n");

    } else if (libro->estado == 0) {
        printf("Ese libro no estaba prestado.\n");

    } else {
        libro->estado = 0;
        printf("Devuelto: %s\n", libro->titulo);
    }
}

// Recorre la lista e imprime todos los libros
void mostrarInventario(Libro *inicio) {

    if (inicio == NULL) {
        printf("El inventario esta vacio.\n");
        return;
    }

    Libro *actual = inicio;

    while (actual != NULL) {

        printf("\nISBN   : %s\n", actual->isbn);
        printf("Titulo : %s\n", actual->titulo);
        printf("Autor  : %s\n", actual->autor);
        printf("Anio   : %d\n", actual->anio);
        printf("Estado : %s\n", (actual->estado == 1) ? "Prestado" : "Disponible");

        actual = actual->siguiente;
    }
}

// Muestra los datos de un solo libro
void mostrarLibro(Libro *libro) {

    printf("\nISBN   : %s\n", libro->isbn);
    printf("Titulo : %s\n", libro->titulo);
    printf("Autor  : %s\n", libro->autor);
    printf("Anio   : %d\n", libro->anio);
    printf("Estado : %s\n", (libro->estado == 1) ? "Prestado" : "Disponible");
}

// Libera toda la lista al cerrar el programa
void liberarLista(Libro **inicio) {

    Libro *actual = *inicio;

    while (actual != NULL) {
        Libro *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    *inicio = NULL;
}

// Lee una linea del teclado y le quita el salto de linea que deja fgets
void leerTexto(char destino[], int tamano) {

    fgets(destino, tamano, stdin);
    destino[strlen(destino) - 1] = '\0';
}

int main() {

    Libro *inicio = NULL;        // Puntero al primer libro de la lista

    char isbn[20], titulo[100], autor[50];
    int anio, opcion;

    do {
        printf("\n===== INVENTARIO DE LA BIBLIOTECA =====\n");
        printf("1. Agregar libro\n");
        printf("2. Buscar libro por ISBN\n");
        printf("3. Prestar libro\n");
        printf("4. Devolver libro\n");
        printf("5. Eliminar libro\n");
        printf("6. Mostrar inventario\n");
        printf("7. Salir\n");
        printf("Opcion: ");

        scanf("%d", &opcion);
        getchar();               // Limpia el ENTER que deja scanf

        if (opcion == 1) {

            printf("ISBN: ");
            leerTexto(isbn, 20);

            printf("Titulo: ");
            leerTexto(titulo, 100);

            printf("Autor: ");
            leerTexto(autor, 50);

            printf("Anio: ");
            scanf("%d", &anio);
            getchar();

            insertarLibro(&inicio, isbn, titulo, autor, anio);

        } else if (opcion == 2) {

            printf("ISBN a buscar: ");
            leerTexto(isbn, 20);

            Libro *libro = buscarLibro(inicio, isbn);

            if (libro == NULL) {
                printf("No se encontro ningun libro con ese ISBN.\n");
            } else {
                mostrarLibro(libro);
            }

        } else if (opcion == 3) {

            printf("ISBN a prestar: ");
            leerTexto(isbn, 20);
            prestarLibro(inicio, isbn);

        } else if (opcion == 4) {

            printf("ISBN a devolver: ");
            leerTexto(isbn, 20);
            devolverLibro(inicio, isbn);

        } else if (opcion == 5) {

            printf("ISBN a eliminar: ");
            leerTexto(isbn, 20);
            eliminarLibro(&inicio, isbn);

        } else if (opcion == 6) {

            mostrarInventario(inicio);

        } else if (opcion != 7) {

            printf("Opcion invalida.\n");
        }

    } while (opcion != 7);

    liberarLista(&inicio);       // Se libera la memoria de los libros restantes
    printf("Sistema cerrado.\n");

    return 0;
}