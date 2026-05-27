#include <math.h>
#include <omp.h>
#include <stdio.h>

int es_primo(long long int n) {
    int es_compuesto = 0;
    long long int lim = (long long int)sqrt(n);

    #pragma omp parallel for reduction(|| : es_compuesto)
    for (long long int i = 2; i <= lim; i++) {
        if (!es_compuesto) {
            if (n % i == 0)
                es_compuesto = 1;
        }
    }

    return !es_compuesto;
}

int main() {
    long long int n1 = (long long)2e17;
    long long int n2 = 9223372036854775783;

    printf("%d\n", es_primo(n1));
    printf("%d\n", es_primo(n2));
}
