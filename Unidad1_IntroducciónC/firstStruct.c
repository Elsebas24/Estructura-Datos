#include <stdio.h>

struct Datos {
    int id;
    char nombre[50];
    char telefono[13];
    int edad;
    int carrera; // 0: ISCO, 1:IGTI, 2:IETE, 3: IISI
};

int main() {
    struct Datos estudiante[3] = {{1, "Geisler Jimenez", "+528262135314", 17, 0}, 
                                  {2, "Karen Rachel", "+528262135315", 19, 1}, 
                                  {3, "Hiram Chable", "+528262084576", 18, 2}};

    for (int i = 0; i < 3; i++) {
        printf("Id: %d\n", estudiante[i].id);
        printf("Nombre: %s\n", estudiante[i].nombre);
        printf("Telefono: %s\n", estudiante[i].telefono);
        printf("Edad: %d\n", estudiante[i].edad);
        switch (estudiante[i].carrera) {
            case 0:
                printf("Carrera: ISCO\n");
                break;
            case 1:
                printf("Carrera: IGTI\n");
                break;
            case 2:
                printf("Carrera: IETE\n");
                break;
            case 3:
                printf("Carrera: IISI\n");
                break;
            default:
                printf("Carrera no reconocida\n");
                break;
        }
        printf("________________________________\n");
    }

    return 0;
}