# Traffic Light Management Simulator

A queue-based traffic management system for handling vehicles at a four-way junction with priority lane support.

## Project Overview

This project simulates a traffic junction with 4 lanes where vehicles are managed using queue data structures. Lane BL2 is designated as a priority lane that receives preferential treatment when vehicle count exceeds a threshold.

## Files

- `simulator.c` - Main traffic simulator program
- `traffic_generator.c` - Automatic vehicle generation program  
- `vehicles.data` - Vehicle data file (generated automatically)
- `README.md` - This file

## Data Structures Used

### Vehicle Structure
```c
typedef struct {
    int id;           // Unique vehicle identifier
    char lane[10];    // Lane name (AL1, BL2, CL3, DL2)
    time_t arrival_time;
    int direction;    // 0=Straight, 1=Left, 2=Right
} Vehicle;
```

### Queue (Linked List Implementation)
```c
typedef struct Node {
    Vehicle data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;
```

### Lane Structure
```c
typedef struct {
    char name[10];
    int priority;      // Priority level (1=normal, 10=high)
    Queue* vehicles;   // Queue of waiting vehicles
} Lane;
```

## Algorithm

### Traffic Management Algorithm

1. **Vehicle Loading**: Read vehicles from `vehicles.data` file
2. **Priority Check**: 
   - If BL2 has ≥10 vehicles: Set priority to 10
   - If BL2 has <5 vehicles: Reset priority to 1
3. **Lane Selection**: Choose lane with highest priority
4. **Vehicle Processing**: 
   - Calculate average vehicles: `(AL1 + BL2 + CL3 + DL2) / 4`
   - Process that many vehicles from selected lane
5. **Repeat**: Continue until all lanes processed

### Time Complexity

| Operation | Time Complexity |
|-----------|----------------|
| Enqueue | O(1) |
| Dequeue | O(1) |
| Find Priority Lane | O(n) where n=4 lanes |
| Load Vehicles | O(m) where m=number of vehicles |
| Process Cycle | O(n × k) where k=vehicles per lane |

**Overall**: O(m + n×k) per cycle - Linear in number of vehicles

## Compilation

```bash
gcc simulator.c -o simulator
gcc traffic_generator.c -o traffic_generator
```

## Running the Simulator

### Step 1: Start Traffic Generator
Open terminal 1:
```bash
./traffic_generator 120 2
```
- `120` = run for 120 seconds
- `2` = generate vehicles every 2 seconds

### Step 2: Start Simulator
Open terminal 2:
```bash
./simulator 120
```
- `120` = run simulation for 120 seconds

### Default Values
If you don't provide arguments:
- Traffic generator: 300 seconds, 2 second interval
- Simulator: 120 seconds

## Output Examples

### Traffic Generator
```
=== Traffic Generator Started ===
[Generator] Vehicle 1 added to BL2 (Direction: 0) at 1703612400
[Generator] Vehicle 2 added to AL1 (Direction: 1) at 1703612400
[Generator] Vehicle 3 added to BL2 (Direction: 0) at 1703612402
```

### Simulator
```
╔════════════════════════════════════════════════════════════════╗
║          TRAFFIC LIGHT MANAGEMENT SIMULATOR                    ║
╚════════════════════════════════════════════════════════════════╝

┌──────────────────── JUNCTION STATUS ────────────────────────┐
│  Lane AL1: [🔴 STOP] Vehicles: 5    Priority: 1              │
│  Lane BL2: [🟢 GO  ] Vehicles: 12   Priority: 10 (PRIORITY)  │
│  Lane CL3: [🔴 STOP] Vehicles: 3    Priority: 1              │
│  Lane DL2: [🔴 STOP] Vehicles: 2    Priority: 1              │
│  Active Lane: BL2                                             │
└───────────────────────────────────────────────────────────────┘

[ALERT] Lane BL2 has 12 vehicles! Setting HIGH PRIORITY

[Processing] BL2 - Serving 5 vehicles
  ✓ Vehicle 42 passed through BL2 (Direction: Straight)
  ✓ Vehicle 43 passed through BL2 (Direction: Left)
```

## How It Works

### Normal Condition
All lanes have equal priority (priority = 1). The system processes lanes in order, serving an average number of vehicles from each lane.

### High-Priority Condition
When BL2 accumulates more than 10 vehicles, its priority increases to 10. The system then serves BL2 first until vehicle count drops below 5, then returns to normal operation.

### Communication
Both programs communicate through the `vehicles.data` file:
- Traffic generator writes vehicle data
- Simulator reads and processes vehicles
- File format: `id,lane,timestamp,direction`

## Functions Implemented

### Queue Operations
- `createQueue()` - Initialize empty queue
- `enqueue()` - Add vehicle to rear of queue
- `dequeue()` - Remove vehicle from front of queue  
- `isEmpty()` - Check if queue has no vehicles

### Traffic Management
- `loadVehicles()` - Read vehicles from data file
- `updatePriorities()` - Adjust lane priorities based on vehicle count
- `getHighestPriorityLane()` - Find lane with highest priority
- `calculateVehiclesToServe()` - Compute average for fair serving
- `processLane()` - Handle vehicle flow through a lane
- `displayStatus()` - Show current junction state
- `displayStats()` - Show simulation statistics

## Testing Scenarios

1. **Normal Traffic**: All lanes under 5 vehicles - equal treatment
2. **Priority Activation**: BL2 reaches 10+ vehicles - gets priority  
3. **Priority Deactivation**: BL2 drops below 5 - returns to normal
4. **No Traffic**: Empty queues - system waits

## Troubleshooting

**Simulator shows no vehicles:**
- Make sure traffic_generator is running first
- Wait 5-10 seconds for vehicles to generate
- Check if `vehicles.data` file exists

**Compilation errors:**
- Ensure you're using GCC: `gcc --version`
- Check file names match exactly

**Permission denied:**
```bash
chmod +x simulator traffic_generator
```


## Author

Nistha Dhakal 
Roll Number: 19
Course: Data Structures and Algorithms (COMP202)  
Assignment: #1
