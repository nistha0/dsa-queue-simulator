#include <stdio.h>
#include "queue.h"

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue *q) {
    if (q->front == -1)
        return 1;
    return 0;
}

int isFull(Queue *q) {
    if (q->rear == MAX_SIZE - 1)
        return 1;
    return 0;
}

void enqueue(Queue *q, int value) {
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }

    if (isEmpty(q)) {
        q->front = 0;
    }

    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue *q) {
    int value;

    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1;
    }

    value = q->items[q->front];

    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }

    return value;
}
