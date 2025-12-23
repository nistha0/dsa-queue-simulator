#include <stdio.h>
#include "queue.h"


int main() {
    Queue testQueue;

    initQueue(&testQueue);

    printf("Adding vehicles to queue...\n");
    enqueue(&testQueue, 101);
    enqueue(&testQueue, 102);
    enqueue(&testQueue, 103);

    printf("Removing vehicles from queue...\n");
    while (!isEmpty(&testQueue)) {
        int vehicle = dequeue(&testQueue);
        printf("Vehicle %d passed the junction\n", vehicle);
    }

    return 0;
}

