#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100  // Tamano maximo de la pila (cuantos simbolos de apertura puede guardar a la vez)

// ---------------------------------------------------------
// Estructura de la pila estatica: un arreglo de caracteres
// de tamano fijo (datos) y un indice (top) que indica cual
// es la posicion del ultimo elemento insertado.
// Si top == -1, la pila esta vacia.
// ---------------------------------------------------------
typedef struct {
    int top;
    char datos[MAX];
} Stack;

// Inicializa la pila dejandola vacia (top = -1)
void initStack(Stack *s) {
    s->top = -1;
}

// Indica si la pila ya no tiene espacio disponible
bool isFull(Stack *s) {
    return s->top == MAX - 1;
}

// Indica si la pila no tiene ningun elemento guardado
bool isEmpty(Stack *s) {
    return s->top == -1;
}

// Inserta un valor en la pila (operacion push).
// Primero revisa que haya espacio; si no hay, regresa false.
// Si hay espacio, avanza el indice top y guarda el valor ahi.
bool push(Stack *s, char value) {
    if (isFull(s)) {
        return false;
    }
    s->datos[++(s->top)] = value;
    return true;
}

// Retira y devuelve el valor que esta en el tope de la pila (operacion pop).
// Si la pila esta vacia, regresa '\0' como valor invalido.
// Si tiene elementos, devuelve el valor del tope y retrocede el indice top.
char pop(Stack *s) {
    if (isEmpty(s)) {
        return '\0';
    }
    return s->datos[(s->top)--];
}

// Indica si el caracter es un simbolo de APERTURA: ( { [
bool isOpen(char c) {
    return c == '(' || c == '{' || c == '[';
}

// Indica si el caracter es un simbolo de CIERRE: ) } ]
bool isClose(char c) {
    return c == ')' || c == '}' || c == ']';
}

// Dado un simbolo de cierre, regresa cual es la apertura que
// le corresponde. Por ejemplo, si el cierre es ')', la apertura
// esperada es '('. Sirve para comparar contra lo que se desapilo.
char aperturaEsperada(char cierre) {
    if (cierre == ')') return '(';
    if (cierre == '}') return '{';
    if (cierre == ']') return '[';
    return '\0';
}

// ---------------------------------------------------------
// Funcion principal del balanceador.
// Recorre la expresion caracter por caracter:
//   - Si encuentra una apertura, la apila (push).
//   - Si encuentra un cierre:
//       * Si la pila esta vacia, es un error (cierre sin apertura).
//       * Si no esta vacia, desapila (pop) y compara: el simbolo
//         desapilado debe ser la apertura correspondiente a ese
//         cierre. Si no coincide, es un error (simbolos cruzados,
//         por ejemplo "(]").
// Al terminar de recorrer toda la expresion, si la pila quedo
// vacia significa que todas las aperturas encontraron su cierre,
// por lo tanto la expresion esta balanceada.
// ---------------------------------------------------------
bool esBalanceada(const char *expresion, Stack *s) {
    initStack(s);

    for (int i = 0; expresion[i] != '\0'; i++) {
        char c = expresion[i];

        if (isOpen(c)) {
            push(s, c);
            printf("  Pos %2d: '%c' -> push. Pila: %d elemento(s)\n", i, c, s->top + 1);
        }
        else if (isClose(c)) {
            if (isEmpty(s)) {
                // Se encontro un cierre pero no hay ninguna apertura pendiente
                printf("  Pos %2d: '%c' -> ERROR: Cierre sin apertura previa.\n", i, c);
                return false;
            }
            char desapilado = pop(s);
            char esperado = aperturaEsperada(c);
            if (desapilado != esperado) {
                // El simbolo que estaba en el tope no es el que le toca a este cierre
                printf("  Pos %2d: '%c' -> ERROR: no coincide con '%c' (se esperaba cerrar '%c').\n", i, c, desapilado, esperado);
                return false;
            }
            printf("  Pos %2d: '%c' -> pop '%c'. Pila: %d elemento(s)\n", i, c, desapilado, s->top + 1);
        }
        // Cualquier otro caracter (letras, numeros, espacios, operadores) se ignora
    }

    // Si la pila quedo vacia, todo cerro correctamente
    return isEmpty(s);
}

// Prueba una expresion: la manda al balanceador, imprime el
// recorrido paso a paso (dentro de esBalanceada) y al final
// muestra si quedo balanceada o no.
void probarExpresion(const char *expresion) {
    Stack s;
    printf("Expresion: \"%s\"\n", expresion);
    bool resultado = esBalanceada(expresion, &s);
    if (resultado) {
        printf("Esta balanceada, todos los simbolos cierran correctamente.\n");
    } else {
        printf("No esta balanceada, hay simbolos sin cerrar o cruzados.\n");
    }
    printf("--------------------------------------------------\n");
}

// fgets deja el caracter de salto de linea '\n' al final del texto
// leido; esta funcion lo quita para que la expresion quede limpia.
void limpiarSalto(char *texto) {
    int len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    }
}

int main() {
    char expresion[MAX];
    int opcion;

    // Menu principal: se repite hasta que el usuario elija salir (opcion 2)
    do {
        printf("\n===== BALANCEADOR DE SIMBOLOS ( ), { }, [ ] =====\n");
        printf("1. Probar una expresion\n");
        printf("2. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpia el salto de linea que scanf deja pendiente en el buffer

        if (opcion == 1) {
            printf("Escribe la expresion a evaluar: ");
            // Se usa fgets en vez de scanf("%s", ...) porque la expresion
            // puede tener espacios, y scanf("%s") se detendria en el primero.
            fgets(expresion, MAX, stdin);
            limpiarSalto(expresion);
            probarExpresion(expresion);
        }

    } while (opcion != 2);

    printf("Programa finalizado.\n");
    return 0;
}

/*
===================================================================
CASOS DE PRUEBA Y SALIDA ESPERADA
===================================================================
 
CASO 1: Expresion BALANCEADA
Entrada: (a + b) * [c - d]
Salida esperada:
  Pos  0: '(' -> push. Pila: 1 elemento(s)
  Pos  6: ')' -> pop '('. Pila: 0 elemento(s)
  Pos 10: '[' -> push. Pila: 1 elemento(s)
  Pos 16: ']' -> pop '['. Pila: 0 elemento(s)
  Resultado: BALANCEADA (todos los simbolos coinciden)
 
-------------------------------------------------------------------
 
CASO 2: Expresion NO BALANCEADA (falta un cierre)
Entrada: {(a + b) * [c - d]
Salida esperada:
  Pos  0: '{' -> push. Pila: 1 elemento(s)
  Pos  1: '(' -> push. Pila: 2 elemento(s)
  Pos  7: ')' -> pop '('. Pila: 1 elemento(s)
  Pos 11: '[' -> push. Pila: 2 elemento(s)
  Pos 17: ']' -> pop '['. Pila: 1 elemento(s)
  Resultado: NO BALANCEADA (hay simbolos sin pareja o cruzados)
 
-------------------------------------------------------------------
 
CASO 3: Expresion con ERROR DE CIERRE (cierre sin apertura)
Entrada: a + b) * (c - d)
Salida esperada:
  Pos  5: ')' -> ERROR: cierre sin apertura previa.
  Resultado: NO BALANCEADA (hay simbolos sin pareja o cruzados)
 
-------------------------------------------------------------------
 
CASO 4 (extra): Simbolos cruzados
Entrada: (a + [b - c)]
Salida esperada:
  Pos  0: '(' -> push. Pila: 1 elemento(s)
  Pos  5: '[' -> push. Pila: 2 elemento(s)
  Pos 11: ')' -> ERROR: no coincide con '[' (se esperaba cerrar '(').
  Resultado: NO BALANCEADA (hay simbolos sin pareja o cruzados)
 
===================================================================
*/