#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 10;
    while (i--) {
        char buf[4];
        scanf("%s", buf);
        printf("Obtuve: %s\n", buf);
        sleep(5);
    }

    return 0;
}
