typedef struct {
    int id;
    char lane[4];
} Vehicle;

#include <stdio.h>
#include "queue.h"
int countVehicles(Queue *q) {
    if (isEmpty(q))
        return 0;
    return q->rear - q->front + 1;
}
void serveLane(Queue *lane, char *laneName) {
    if (!isEmpty(lane)) {
        int vehicle = dequeue(lane);
        printf("Vehicle %d served from %s\n", vehicle, laneName);
    }
}


int main() {
    Queue AL2, BL2, CL2, DL2;

     Queue *lanes[] = {&AL2, &BL2, &CL2, &DL2};

for (int i = 0; i < 4; i++) {
    initQueue(lanes[i]);
}


    initQueue(&testQueue);
    enqueue(&AL2, 201);
    enqueue(&AL2, 202);

    enqueue(&BL2, 301);
    enqueue(&CL2, 401);
    enqueue(&DL2, 501);


    printf("Adding vehicles to queue...\n");
    enqueue(&testQueue, 101);
    enqueue(&testQueue, 102);
    enqueue(&testQueue, 103);

    printf("AL2 vehicles: %d\n", countVehicles(&AL2));
    printf("BL2 vehicles: %d\n", countVehicles(&BL2));
    printf("CL2 vehicles: %d\n", countVehicles(&CL2));
    printf("DL2 vehicles: %d\n", countVehicles(&DL2));

    int al2Count = countVehicles(&AL2);
    printf("\nChecking priority lane condition...\n");
    printf("AL2 vehicle count: %d\n", al2Count);
    if (al2Count > 10) {
    printf("Priority condition activated for AL2\n");
}


    printf("\nVehicles passing the junction:\n");
    while (!isEmpty(&testQueue)) {
        int vehicle = dequeue(&testQueue);
        printf("Vehicle %d passed\n", vehicle);
    }
    printf("\nNormal traffic condition:\n");

    serveLane(&AL2, "AL2");
    serveLane(&BL2, "BL2");
    serveLane(&CL2, "CL2");
    serveLane(&DL2, "DL2");


    return 0;
}


