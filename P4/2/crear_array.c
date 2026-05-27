#include <fcntl.h>
#include <omp.h>
#include <sys/mman.h>
#include <unistd.h>

#define N (unsigned long long)5e8

int main() {
    unsigned long long sz = sizeof(double) * N;

    int fd;
    fd = open("./arr.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    ftruncate(fd, sz);

    double *pa = mmap(NULL, sz, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    for (unsigned long long i = 0; i < N; i++) {
        pa[i] = i + 1;
    }

    close(fd);
    munmap(pa, sz);

    return 0;
}
