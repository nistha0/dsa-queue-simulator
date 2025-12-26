#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_LANES 4
#define PRIORITY_HIGH 10
#define PRIORITY_LOW 5
#define VEHICLE_DATA_FILE "vehicles.data"

// Vehicle structure
typedef struct {
    int id;
    char lane[10];
    time_t arrival_time;
    int direction;
} Vehicle;

// Queue node
typedef struct Node {
    Vehicle data;
    struct Node* next;
} Node;

// Queue structure
typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

// Lane structure
typedef struct {
    char name[10];
    int priority;
    Queue* vehicles;
} Lane;

// Function to create a queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// Enqueue operation
void enqueue(Queue* q, Vehicle v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = v;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

// Dequeue operation
Vehicle dequeue(Queue* q) {
    Vehicle v = {0};
    if (q->front == NULL) return v;
    
    Node* temp = q->front;
    v = temp->data;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return v;
}

// Check if queue is empty
int isEmpty(Queue* q) {
    return q->front == NULL;
}

// Load vehicles from file
void loadVehicles(Lane lanes[]) {
    FILE* fp = fopen(VEHICLE_DATA_FILE, "r");
    if (fp == NULL) return;
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Vehicle v;
        if (sscanf(line, "%d,%[^,],%ld,%d", &v.id, v.lane, &v.arrival_time, &v.direction) == 4) {
            // Find which lane this vehicle belongs to
            for (int i = 0; i < MAX_LANES; i++) {
                if (strcmp(lanes[i].name, v.lane) == 0) {
                    // Check if vehicle already exists
                    int exists = 0;
                    Node* current = lanes[i].vehicles->front;
                    while (current != NULL) {
                        if (current->data.id == v.id) {
                            exists = 1;
                            break;
                        }
                        current = current->next;
                    }
                    if (!exists) {
                        enqueue(lanes[i].vehicles, v);
                    }
                    break;
                }
            }
        }
    }
    fclose(fp);
}

// Calculate vehicles to serve based on average
int calculateVehiclesToServe(Lane lanes[]) {
    int total = 0;
    for (int i = 0; i < MAX_LANES; i++) {
        total += lanes[i].vehicles->size;
    }
    if (total == 0) return 0;
    
    int avg = total / MAX_LANES;
    return (avg > 0) ? avg : 1;
}

// Update lane priorities
void updatePriorities(Lane lanes[]) {
    // Check BL2 (index 1) for priority condition
    if (lanes[1].vehicles->size >= PRIORITY_HIGH) {
        lanes[1].priority = 10;
        printf("\n[ALERT] Lane BL2 has %d vehicles! Setting HIGH PRIORITY\n", 
               lanes[1].vehicles->size);
    } else if (lanes[1].vehicles->size < PRIORITY_LOW) {
        lanes[1].priority = 1;
    }
    
    // Set other lanes to normal priority
    for (int i = 0; i < MAX_LANES; i++) {
        if (i != 1 && lanes[i].vehicles->size > 0) {
            lanes[i].priority = 1;
        }
    }
}

// Find lane with highest priority
int getHighestPriorityLane(Lane lanes[]) {
    int maxPriority = -1;
    int laneIndex = -1;
    
    for (int i = 0; i < MAX_LANES; i++) {
        if (lanes[i].vehicles->size > 0 && lanes[i].priority > maxPriority) {
            maxPriority = lanes[i].priority;
            laneIndex = i;
        }
    }
    return laneIndex;
}

// Process a lane
void processLane(Lane* lane, int count) {
    printf("\n[Processing] %s - Serving %d vehicles\n", lane->name, count);
    
    int served = 0;
    const char* directions[] = {"Straight", "Left", "Right"};
    
    while (served < count && !isEmpty(lane->vehicles)) {
        Vehicle v = dequeue(lane->vehicles);
        printf("  ✓ Vehicle %d passed through %s (Direction: %s)\n", 
               v.id, lane->name, directions[v.direction]);
        served++;
        usleep(500000); // 0.5 second delay
    }
    
    printf("[Complete] %s processed %d vehicles. Remaining: %d\n", 
           lane->name, served, lane->vehicles->size);
}

// Display junction status
void displayStatus(Lane lanes[], int activeLane) {
    printf("\n┌──────────────────── JUNCTION STATUS ────────────────────────┐\n");
    printf("│                                                               │\n");
    
    for (int i = 0; i < MAX_LANES; i++) {
        char status[10];
        if (i == activeLane) {
            strcpy(status, "🟢 GO ");
        } else {
            strcpy(status, "🔴 STOP");
        }
        
        if (i == 1) { // BL2 is priority lane
            printf("│  Lane %s: [%s] Vehicles: %-3d  Priority: %-2d (PRIORITY)   │\n", 
                   lanes[i].name, status, lanes[i].vehicles->size, lanes[i].priority);
        } else {
            printf("│  Lane %s: [%s] Vehicles: %-3d  Priority: %-2d               │\n", 
                   lanes[i].name, status, lanes[i].vehicles->size, lanes[i].priority);
        }
    }
    
    printf("│                                                               │\n");
    if (activeLane >= 0) {
        printf("│  Active Lane: %-10s                                       │\n", 
               lanes[activeLane].name);
    }
    printf("└───────────────────────────────────────────────────────────────┘\n");
}

// Display statistics
void displayStats(Lane lanes[], time_t startTime) {
    time_t now = time(NULL);
    int elapsed = (int)difftime(now, startTime);
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                     SIMULATION STATISTICS                      ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║  Running Time: %d seconds                                      ║\n", elapsed);
    printf("║  Current Queue Status:                                         ║\n");
    for (int i = 0; i < MAX_LANES; i++) {
        if (i == 1) {
            printf("║    Lane %s: %3d vehicles waiting (Priority Lane)             ║\n", 
                   lanes[i].name, lanes[i].vehicles->size);
        } else {
            printf("║    Lane %s: %3d vehicles waiting                             ║\n", 
                   lanes[i].name, lanes[i].vehicles->size);
        }
    }
    printf("╚════════════════════════════════════════════════════════════════╝\n");
}

int main(int argc, char* argv[]) {
    int duration = 120;
    if (argc > 1) {
        duration = atoi(argv[1]);
    }
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║          TRAFFIC LIGHT MANAGEMENT SIMULATOR                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    printf("Starting simulation for %d seconds...\n", duration);
    printf("Make sure traffic_generator is running!\n");
    printf("Press Ctrl+C to stop\n\n");
    
    sleep(2);
    
    // Initialize lanes
    Lane lanes[MAX_LANES];
    const char* laneNames[] = {"AL1", "BL2", "CL3", "DL2"};
    
    for (int i = 0; i < MAX_LANES; i++) {
        strcpy(lanes[i].name, laneNames[i]);
        lanes[i].priority = 1;
        lanes[i].vehicles = createQueue();
    }
    
    time_t startTime = time(NULL);
    time_t lastPoll = startTime;
    time_t lastCycle = startTime;
    
    // Main simulation loop
    while (difftime(time(NULL), startTime) < duration) {
        time_t now = time(NULL);
        
        // Poll for new vehicles every second
        if (difftime(now, lastPoll) >= 1) {
            loadVehicles(lanes);
            lastPoll = now;
        }
        
        // Process traffic every 5 seconds
        if (difftime(now, lastCycle) >= 5) {
            updatePriorities(lanes);
            
            // Process all lanes with vehicles
            int processed = 0;
            while (processed < MAX_LANES) {
                int laneIndex = getHighestPriorityLane(lanes);
                if (laneIndex == -1) break; // No more lanes with vehicles
                
                displayStatus(lanes, laneIndex);
                
                int vehiclesToServe = calculateVehiclesToServe(lanes);
                if (vehiclesToServe == 0) vehiclesToServe = 1;
                
                processLane(&lanes[laneIndex], vehiclesToServe);
                
                // Mark lane as processed by setting priority to 0 temporarily
                lanes[laneIndex].priority = 0;
                processed++;
                
                sleep(1);
            }
            
            // Reset all priorities for next cycle
            for (int i = 0; i < MAX_LANES; i++) {
                if (lanes[i].priority == 0) lanes[i].priority = 1;
            }
            
            displayStats(lanes, startTime);
            lastCycle = now;
        }
        
        sleep(1);
    }
    
    printf("\n\n=== SIMULATION COMPLETED ===\n");
    displayStats(lanes, startTime);
    
    // Cleanup
    for (int i = 0; i < MAX_LANES; i++) {
        while (!isEmpty(lanes[i].vehicles)) {
            dequeue(lanes[i].vehicles);
        }
        free(lanes[i].vehicles);
    }
    
    printf("\nThank you for using the simulator!\n\n");
    return 0;
}