#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Nodo{
    int Id;
    char *Nombre;
    char *ApellidoP;
    char *ApellidoM;
    int Edad;
    char *Email;
    char *Telefono;
    struct Nodo *siguiente;
}Nodo;

typedef struct Lista{
    Nodo *primero;
    Nodo *ultimo;
    int size;
}MiLista;

void iniLista(MiLista *l){
    l->primero = NULL;
    l->ultimo = NULL;
    l->size = 0;
}

bool empty(MiLista *l){
    return l->primero == NULL;
}

/* Muestra el mensaje, lee el texto de cualquier largo y lo regresa, esta funcion ya fue implementada
por medio de busqueda de soluciones ya que a nivel de compilacion estaba teniendo los problemas de memoria
por el hecho de que se estaba usando malloc pero al momento de guardar se comia unos caracteres */
char *pedirTexto(char *mensaje){
    char *texto = NULL;
    char *nuevo;
    size_t longitud = 0;
    size_t capacidad = 0;
    int c;

    printf("%s", mensaje);

    /* Consume el Enter pendiente de scanf, si existe */
    c = getchar();
    if(c == '\n'){
        c = getchar();
    }

    while(c != '\n' && c != EOF){
        if(longitud + 1 >= capacidad){
            capacidad = capacidad == 0 ? 16 : capacidad * 2;
            nuevo = realloc(texto, capacidad);
            if(nuevo == NULL){
                free(texto);
                printf("No hay memoria disponible\n");
                exit(1);
            }
            texto = nuevo;
        }
        texto[longitud++] = (char)c;
        c = getchar();
    }

    nuevo = realloc(texto, longitud + 1);
    if(nuevo == NULL){
        free(texto);
        printf("No hay memoria disponible\n");
        exit(1);
    }
    texto = nuevo;
    texto[longitud] = '\0';
    return texto;
}

/* Libera los textos de un nodo y luego el nodo */
void liberarNodo(Nodo *n){
    free(n->Nombre);
    free(n->ApellidoP);
    free(n->ApellidoM);
    free(n->Email);
    free(n->Telefono);
    free(n);
}

void agregar(MiLista *l){
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));

    if(nuevo == NULL){
        printf("No hay memoria disponible\n");
        exit(1);
    }

    printf("Id: ");
    scanf("%d", &nuevo->Id);

    nuevo->Nombre = pedirTexto("Nombre: ");
    nuevo->ApellidoP = pedirTexto("Apellido paterno: ");
    nuevo->ApellidoM = pedirTexto("Apellido materno: ");
    printf("Edad: ");
    scanf("%d", &nuevo->Edad);
    nuevo->Email = pedirTexto("Email: ");
    nuevo->Telefono = pedirTexto("Telefono: ");
    nuevo->siguiente = NULL;

    if(empty(l)){
        l->primero = nuevo;
    }else{
        l->ultimo->siguiente = nuevo;
    }
    l->ultimo = nuevo;
    l->size++;

    printf("Registro agregado con Id %d\n", nuevo->Id);
}

/* Regresa el primer nodo con ese Id o NULL si no existe */
Nodo *buscar(MiLista *l, int id){
    Nodo *actual = l->primero;

    while(actual != NULL){
        if(actual->Id == id){
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void imprimirNodo(Nodo *n){
    printf("\n");
    printf("    Id:         %d\n", n->Id);
    printf("    Nombre:     %s\n", n->Nombre);
    printf("    Apellido P: %s\n", n->ApellidoP);
    printf("    Apellido M: %s\n", n->ApellidoM);
    printf("    Edad:       %d\n", n->Edad);
    printf("    Email:      %s\n", n->Email);
    printf("    Telefono:   %s\n", n->Telefono);
}

void imprimirLista(MiLista *l){
    Nodo *actual = l->primero;

    if(empty(l)){
        printf("No hay registros\n");
    }
    while(actual != NULL){
        imprimirNodo(actual);
        actual = actual->siguiente;
    }
}

/* Solo se libera el texto anterior y se sobreescribe el dato */
void editar(MiLista *l){
    int id;
    Nodo *n;

    printf("Id a editar: ");
    scanf("%d", &id);
    n = buscar(l, id);

    if(n == NULL){
        printf("No existe ese registro\n");
        return;
    }

    free(n->Nombre);
    n->Nombre = pedirTexto("Nombre nuevo: ");
    free(n->ApellidoP);
    n->ApellidoP = pedirTexto("Apellido paterno nuevo: ");
    free(n->ApellidoM);
    n->ApellidoM = pedirTexto("Apellido materno nuevo: ");
    printf("Edad nueva: ");
    scanf("%d", &n->Edad);
    free(n->Email);
    n->Email = pedirTexto("Email nuevo: ");
    free(n->Telefono);
    n->Telefono = pedirTexto("Telefono nuevo: ");

    printf("Registro actualizado\n");
}

/* Elimina el primero con ese Id: regresa 1 si lo borro, -1 si no existe */
int buscarYEliminar(MiLista *l, int id){
    Nodo *actual = l->primero;
    Nodo *anterior = NULL;

    while(actual != NULL){
        if(actual->Id == id){
            if(anterior == NULL){
                l->primero = actual->siguiente;
            }else{
                anterior->siguiente = actual->siguiente;
            }
            if(actual == l->ultimo){
                l->ultimo = anterior;
            }
            liberarNodo(actual);
            l->size--;
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return -1;
}

void liberar(MiLista *l){
    Nodo *actual = l->primero;
    Nodo *siguiente;

    while(actual != NULL){
        siguiente = actual->siguiente;
        liberarNodo(actual);
        actual = siguiente;
    }
    l->primero = NULL;
    l->ultimo = NULL;
    l->size = 0;
}

int main(){
    MiLista lista;
    int opcion;
    int id;
    Nodo *n;

    iniLista(&lista);

    do{
        printf("\n==================\n");
        printf("       AGENDA         ");
        printf("\n==================\n");
        printf("1. Agregar registro\n");
        printf("2. Editar registro\n");
        printf("3. Eliminar registro\n");
        printf("4. Mostrar registros\n");
        printf("5. Buscar registro\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                agregar(&lista);
                break;
            case 2:
                editar(&lista);
                break;
            case 3:
                printf("Id a eliminar: ");
                scanf("%d", &id);
                if(buscarYEliminar(&lista, id) == 1){
                    printf("Registro eliminado\n");
                }else{
                    printf("No existe ese registro\n");
                }
                break;
            case 4:
                imprimirLista(&lista);
                break;
            case 5:
                printf("Id a buscar: ");
                scanf("%d", &id);
                n = buscar(&lista, id);
                if(n != NULL){
                    imprimirNodo(n);
                }else{
                    printf("No existe ese registro\n"); 
                }
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida\n");
        }
    }while(opcion != 0);

    liberar(&lista);
    return 0;
}