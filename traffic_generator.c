#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define VEHICLE_DATA_FILE "vehicles.data"

// Lane names and their probabilities
const char* lanes[] = {"AL1", "BL2", "CL3", "DL2"};
const int probabilities[] = {25, 40, 20, 15}; // BL2 has higher probability

int selectLane() {
    int random = rand() % 100;
    int cumulative = 0;
    
    for (int i = 0; i < 4; i++) {
        cumulative += probabilities[i];
        if (random < cumulative) {
            return i;
        }
    }
    return 0;
}

int selectDirection() {
    int random = rand() % 100;
    if (random < 60) return 0;      // Straight
    else if (random < 80) return 1;  // Left
    else return 2;                   // Right
}

void generateVehicle(int vehicleId) {
    FILE* fp = fopen(VEHICLE_DATA_FILE, "a");
    if (fp == NULL) {
        printf("Error: Cannot open %s\n", VEHICLE_DATA_FILE);
        return;
    }
    
    int laneIndex = selectLane();
    int direction = selectDirection();
    time_t currentTime = time(NULL);
    
    fprintf(fp, "%d,%s,%ld,%d\n", vehicleId, lanes[laneIndex], currentTime, direction);
    fclose(fp);
    
    printf("[Generator] Vehicle %d added to %s (Direction: %d) at %ld\n", 
           vehicleId, lanes[laneIndex], direction, currentTime);
}

void clearFile() {
    FILE* fp = fopen(VEHICLE_DATA_FILE, "w");
    if (fp != NULL) {
        fclose(fp);
    }
    printf("[Generator] Cleared old vehicle data\n");
}

int main(int argc, char* argv[]) {
    int duration = 300;
    int interval = 2;
    
    if (argc > 1) {
        duration = atoi(argv[1]);
    }
    if (argc > 2) {
        interval = atoi(argv[2]);
    }
    
    printf("=== Traffic Generator Started ===\n");
    printf("Duration: %d seconds\n", duration);
    printf("Interval: %d seconds\n", interval);
    printf("Generating vehicles for: AL1, BL2 (Priority), CL3, DL2\n\n");
    
    srand(time(NULL));
    clearFile();
    
    int vehicleId = 1;
    time_t startTime = time(NULL);
    
    while (difftime(time(NULL), startTime) < duration) {
        // Generate 1-3 vehicles at once
        int count = (rand() % 3) + 1;
        
        for (int i = 0; i < count; i++) {
            generateVehicle(vehicleId++);
        }
        
        sleep(interval);
    }
    
    printf("\n[Generator] Finished generating %d vehicles\n", vehicleId - 1);
    printf("=== Traffic Generator Stopped ===\n");
    
    return 0;
}