#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Car {
    char brand[30];
    char model[30];
    int year;
    struct Owner owner;
};

struct Owner {
    char firstName[30];
    char lastName[30];
};

struct Country {
    char name[60];
    struct Car car_country
};

int main() {

    struct Owner persona = {"Geisler", "Jimenez"};
    struct Car myCar = {"Toyota", "Corolla", 2020, persona};
    struct Country estado = {"Nuevo Leon", myCar};

    printf("Car: %s (%d)\n", myCar.brand, myCar.year);
    printf("Owner: %s %s\n", myCar.owner.firstName, myCar.owner.lastName);
    printf("Country: %s\n", estado.name);

    return 0;
}