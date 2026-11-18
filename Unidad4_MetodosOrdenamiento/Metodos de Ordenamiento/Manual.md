# mord.h — Librería de Métodos de Ordenamiento en C

**Creado por alumnos de la materia de Estructura de Datos — Universidad de Montemorelos**

Librería en C que ordena una colección de enteros con **6 métodos**, y que además puede
correrlos **todos al mismo tiempo** (un hilo por método) mostrando una barra de progreso
para cada uno.

* Burbuja
* Inserción
* Selección
* Quicksort
* ShellSort
* Radix (acepta números negativos)


## Inicio rápido

```
gcc mo.c lib/mord/mord.c lib/simdata/simdata.c -o ordenar -lpthread
.\ordenar.exe
```

```
Cuantos datos desea ordenar? 50000

Ordenando de manera simultanea...

Burbuja    [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓-----------]  65.2%
Insercion  [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100.0%  1.2572 s
Seleccion  [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100.0%  1.5583 s
Quicksort  [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100.0%  0.0133 s
ShellSort  [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100.0%  0.0178 s
Radix      [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100.0%  0.0016 s
```

> **Requisitos:** `gcc` con soporte de hilos (`pthread`). En Windows sirve
> [w64devkit](https://github.com/skeeto/w64devkit/releases) (no requiere instalar) o MinGW/MSYS2.


## Estructura del proyecto

```
├── mo.c                 Programa principal (portada + pedir datos + llamar a la librería)
├── beta1.c              Pruebas (no se compila con el programa)
├── lib/
│   ├── mord/            Métodos de ordenamiento, barra de progreso e hilos
│   │   ├── mord.h
│   │   └── mord.c
│   └── simdata/         Generar y revisar los datos
│       ├── simdata.h
│       └── simdata.c
└── web/                 Versión visual en el navegador (abrir web/index.html)
```


## Cómo usar la librería

### Opción 1: los 6 métodos al mismo tiempo

```c
#include <stdlib.h>
#include "lib/mord/mord.h"
#include "lib/simdata/simdata.h"

int main()
{
        int n = 10000;
        int *datos = generar_datos(n, -1000, 1000);   /* n números entre -1000 y 1000 */

        ordenar_simultaneo(datos, n);                  /* barras + tabla de tiempos */

        free(datos);
        return 0;
}
```

### Opción 2: un solo método

Todos los métodos reciben el **arreglo**, la **cantidad de datos** y un **contador de avance**
que el método va subiendo de `0` hasta `n` (sirve para la barra de progreso).

```c
#include <stdio.h>
#include <stdlib.h>
#include "lib/mord/mord.h"
#include "lib/simdata/simdata.h"

int main()
{
        int n = 10000;
        int avance = 0;
        int *arr = generar_datos(n, -1000, 1000);

        quicksort(arr, n, &avance);

        mostrar_barra_de_progreso("Quicksort", avance, n);   /* [▓▓▓...▓] 100.0% */
        printf("\n");

        if (esta_ordenado(arr, n))
                printf("Ordenado correctamente\n");

        free(arr);
        return 0;
}
```

Compilar siempre con los dos `.c` de la librería:

```
gcc mi_programa.c lib/mord/mord.c lib/simdata/simdata.c -o mi_programa -lpthread
```


## Referencia de funciones

### `lib/mord/mord.h` — ordenamiento

| Función | Descripción | Complejidad |
|---|---|---|
| `burbuja(arr, n, &avance)` | Compara vecinos y "sube" el mayor al final | O(n²) |
| `insercion(arr, n, &avance)` | Inserta cada dato en la parte ya ordenada | O(n²) |
| `seleccion(arr, n, &avance)` | Busca el menor y lo pone al inicio | O(n²) |
| `quicksort(arr, n, &avance)` | Divide con un pivote y ordena cada lado | O(n log n) |
| `shellsort(arr, n, &avance)` | Inserción con saltos que se van reduciendo | ≈ O(n^1.5) |
| `radix(arr, n, &avance)` | Ordena dígito por dígito (base 10) | O(d·n) |
| `ordenar_simultaneo(arr, n)` | Corre los 6 métodos en hilos, con barras y tabla de tiempos. **No modifica** `arr` | — |

### `lib/mord/mord.h` — auxiliares

| Función | Descripción |
|---|---|
| `Intercambiar(&a, &b)` | Intercambia dos valores |
| `Particion(arr, bajo, alto)` | Parte el arreglo para Quicksort y regresa la posición del pivote |
| `mostrar_barra_de_progreso(nombre, actual, n)` | Dibuja `Nombre [▓▓▓▓------]  45.3%` (sin salto de línea) |

### `lib/simdata/simdata.h` — datos

| Función | Descripción |
|---|---|
| `generar_datos(n, min, max)` | Regresa un arreglo dinámico con `n` enteros aleatorios entre `min` y `max` |
| `copiar_datos(arr, n)` | Regresa una copia nueva del arreglo |
| `imprimir_datos(arr, n)` | Imprime el arreglo en una línea |
| `esta_ordenado(arr, n)` | Regresa `1` si está en orden ascendente, `0` si no |

> Los arreglos que regresan `generar_datos` y `copiar_datos` se crean con `malloc`:
> hay que liberarlos con `free()`.