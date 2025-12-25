#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fa, *fb, *fc, *fd;

    srand(time(NULL));

    fa = fopen("laneA.txt", "a");
    fb = fopen("laneB.txt", "a");
    fc = fopen("laneC.txt", "a");
    fd = fopen("laneD.txt", "a");

    if (!fa || !fb || !fc || !fd) {
        printf("Error opening lane files\n");
        return 1;
    }

    for (int i = 0; i < 3; i++) {
    fprintf(fa, "%d\n", rand() % 1000);
    fprintf(fb, "%d\n", rand() % 1000);
    fprintf(fc, "%d\n", rand() % 1000);
    fprintf(fd, "%d\n", rand() % 1000);
}

    fclose(fa);
    fclose(fb);
    fclose(fc);
    fclose(fd);

    return 0;
}
