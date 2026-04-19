#include <stdio.h>
#include <unistd.h>

#define N_VISITANTES 2000000

int visitantes = 0;

int main() {
    for (int i = 0; i < N_VISITANTES * 2; i++) {
        visitantes++;
    }

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
