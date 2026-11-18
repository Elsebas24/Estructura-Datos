/*
   Autor: Geisler Jimenez Torres
   Proposito: Atender pacientes con dos colas FIFO, una de prioridad alta y otra normal. La alta siempre pasa primero.
*/

//Bibliotecas
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct Paciente {
    char nombre[50];
    char sintoma[100];
    int prioridad; // 1 = Alta, 0 = Normal
    struct Paciente *siguiente;
} Paciente;
        
int totalAtendidos = 0; // Contador de pacientes ya atendidos

// La cola esta vacia cuando el frente no apunta a ningun nodo
bool estaVacia(Paciente *frente) {
    return frente == NULL;
}

// Agrega un paciente al final de la cola (FIFO)
bool encolar(Paciente **frente, Paciente **final, char nombre[], char sintoma[], int prioridad) {
    Paciente *nuevo = (Paciente *)malloc(sizeof(Paciente));
    if (nuevo == NULL) {
        return false; // Error al asignar memoria
    }
    strcpy(nuevo->nombre, nombre);   // strcpy copia el texto (destino, origen). Con = solo se copiaria la direccion
    strcpy(nuevo->sintoma, sintoma);
    nuevo->prioridad = prioridad;
    nuevo->siguiente = NULL;

    if (estaVacia(*frente)) {
        *frente = nuevo;
        *final = nuevo;
    } else {
        (*final)->siguiente = nuevo;
        *final = nuevo;
    }
    return true;
}

// Saca al paciente del frente y lo devuelve. NULL si la cola esta vacia.
// No hace free aqui porque todavia hay que imprimir sus datos.
Paciente *desencolar(Paciente **frente, Paciente **final) {
    if (estaVacia(*frente)) {
        return NULL; // Cola vacia
    }
    Paciente *atendido = *frente;
    *frente = atendido->siguiente;

    if (*frente == NULL) {
        *final = NULL; // La cola quedo vacia
    }
    return atendido;
}

// Recorre la cola e imprime a cada paciente en su turno
void mostrarCola(Paciente *frente) {
    if (estaVacia(frente)) {
        printf("   (sin pacientes)\n");
        return;
    }
    Paciente *actual = frente;
    int turno = 1;
    while (actual != NULL) {
        printf("   %d. %s - %s\n", turno, actual->nombre, actual->sintoma);
        actual = actual->siguiente;
        turno++;
    }
}

// Cuenta los pacientes que hay en una cola
int contarPacientes(Paciente *frente) {
    int cuenta = 0;
    Paciente *actual = frente;
    while (actual != NULL) {
        cuenta++;
        actual = actual->siguiente;
    }
    return cuenta;
}

// Busca por nombre y devuelve su lugar en la fila, o 0 si no esta
int buscarPaciente(Paciente *frente, char nombre[]) {
    Paciente *actual = frente;
    int lugar = 1;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) { // strcmp compara textos, devuelve 0 si son iguales
            return lugar;
        }
        actual = actual->siguiente;
        lugar++;
    }
    return 0; // No encontrado
}

// Guarda en un archivo a cada paciente conforme se le atiende
void guardarEnBitacora(Paciente *p) {
    FILE *archivo = fopen("bitacora.txt", "a");
    if (archivo == NULL) {
        return; // Si no se puede abrir, se ignora
    }
    fprintf(archivo, "%d. %s - %s (%s)\n", totalAtendidos, p->nombre, p->sintoma, (p->prioridad == 1) ? "ALTA" : "NORMAL");
    fclose(archivo);
}

// Vacia la cola y libera toda su memoria
void liberarCola(Paciente **frente, Paciente **final) {
    Paciente *p;
    while ((p = desencolar(frente, final)) != NULL) {
        free(p);
    }
}

int main() {
    Paciente *frenteAlta = NULL, *finalAlta = NULL;
    Paciente *frenteNormal = NULL, *finalNormal = NULL;

    char nombre[50], sintoma[100];
    int prioridad, opcion;

    do {
        printf("\n===== SISTEMA DE TURNOS =====\n");
        printf("1. Agregar paciente\n");
        printf("2. Atender siguiente paciente\n");
        printf("3. Mostrar colas\n");
        printf("4. Buscar paciente\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar(); // scanf deja el ENTER en el buffer, si no se limpia el fgets se lo traga

        if (opcion == 1) {
            printf("Nombre: ");
            fgets(nombre, 50, stdin);        // fgets lee la linea completa (destino, maximo, teclado). scanf("%s") se corta en el primer espacio
            nombre[strcspn(nombre, "\n")] = '\0'; // fgets tambien guarda el ENTER, strcspn lo localiza y ahi se cierra la cadena con '\0'

            printf("Sintoma: ");
            fgets(sintoma, 100, stdin);
            sintoma[strcspn(sintoma, "\n")] = '\0';

            printf("Prioridad (1 = Alta / 0 = Normal): ");
            scanf("%d", &prioridad);
            getchar();

            if (prioridad == 1) {
                encolar(&frenteAlta, &finalAlta, nombre, sintoma, 1);
            } else {
                encolar(&frenteNormal, &finalNormal, nombre, sintoma, 0);
            }
            printf("Paciente registrado.\n");

        } else if (opcion == 2) {
            // Primero se intenta la cola alta, solo si esta vacia se usa la normal
            Paciente *atendido = desencolar(&frenteAlta, &finalAlta);
            if (atendido == NULL) {
                atendido = desencolar(&frenteNormal, &finalNormal);
            }

            if (atendido == NULL) {
                printf("No hay pacientes en espera.\n");
            } else {
                totalAtendidos++;
                printf("\nAtendiendo a: %s\n", atendido->nombre);
                printf("Sintoma: %s\n", atendido->sintoma);
                printf("Prioridad: %s\n", (atendido->prioridad == 1) ? "ALTA" : "NORMAL");
                printf("Total atendidos: %d\n", totalAtendidos);
                guardarEnBitacora(atendido);
                free(atendido); // ya se imprimio, ahora si se libera
            }

        } else if (opcion == 3) {
            printf("\n[ PRIORIDAD ALTA ] %d en espera\n", contarPacientes(frenteAlta));
            mostrarCola(frenteAlta);
            printf("\n[ NORMAL ] %d en espera\n", contarPacientes(frenteNormal));
            mostrarCola(frenteNormal);

        } else if (opcion == 4) {
            printf("Nombre a buscar: ");
            fgets(nombre, 50, stdin);
            nombre[strcspn(nombre, "\n")] = '\0';

            int lugar = buscarPaciente(frenteAlta, nombre);
            if (lugar > 0) {
                printf("%s esta en la cola ALTA, lugar %d.\n", nombre, lugar);
            } else {
                lugar = buscarPaciente(frenteNormal, nombre);
                if (lugar > 0) {
                    printf("%s esta en la cola NORMAL, lugar %d.\n", nombre, lugar);
                } else {
                    printf("Ese paciente no esta en espera.\n");
                }
            }

        } else if (opcion != 5) {
            printf("Opcion invalida.\n");
        }

    } while (opcion != 5);

    // Libera a los pacientes que se quedaron sin atender
    liberarCola(&frenteAlta, &finalAlta);
    liberarCola(&frenteNormal, &finalNormal);

    printf("Sistema cerrado.\n");
    return 0;
}