typedef struct {
    int id;
    char lane[4];
} Vehicle;

#include <stdio.h>
#include "queue.h"

int main() {
    Queue AL2, BL2, CL2, DL2;

     initQueue(&AL2);
     initQueue(&BL2);
     initQueue(&CL2);
     initQueue(&DL2);


    initQueue(&testQueue);

    printf("Adding vehicles to queue...\n");
    enqueue(&testQueue, 101);
    enqueue(&testQueue, 102);
    enqueue(&testQueue, 103);

    printf("\nVehicles passing the junction:\n");
    while (!isEmpty(&testQueue)) {
        int vehicle = dequeue(&testQueue);
        printf("Vehicle %d passed\n", vehicle);
    }

    return 0;
}


