#include <stdio.h>

struct Datos {
    int id;
    char nombre[50];
    char telefono[13];
    int edad;
};

int main() {
    int num = 0;
    printf("Ingrese el numero de estudiantes: ");
    scanf("%d", &num);

    struct Datos estudiante[num];
    for (int i = 0; i < num; i++) {
        printf("Ingrese los datos del estudiante %d:\n", i + 1);
        estudiante[i].id = i + 1;
        printf("Nombre: ");
        scanf("%s", estudiante[i].nombre);
        printf("Telefono: ");
        scanf("%s", estudiante[i].telefono);
        printf("Edad: ");
        scanf("%d", &estudiante[i].edad);
        printf("________________________________\n");
        printf("\n");
    }

    //Que hacer con los datos ingresados?
    printf("Datos de los estudiantes:\n");
    for (int i = 0; i < num; i++) {
        printf("Id: %d\n", estudiante[i].id);
        printf("Nombre: %s\n", estudiante[i].nombre);
        printf("Telefono: %s\n", estudiante[i].telefono);
        printf("Edad: %d\n", estudiante[i].edad);
        printf("________________________________\n");
    }
    return 0;
}