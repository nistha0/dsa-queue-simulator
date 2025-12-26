gcc simulator.c queue.c -o simulator
gcc traffic_generator.c -o generator

## Traffic Processing Algorithm

The traffic management system works using queue data structures to control the flow of vehicles at a junction.

### Normal Traffic Condition
Under normal conditions, all lanes are treated equally.  
Vehicles are served in a round-robin manner, where one vehicle from each lane is allowed to pass in sequence.  
This ensures fairness and avoids starvation of any lane.

### Priority Traffic Condition
Lane AL2 is considered a priority lane.  
When the number of vehicles waiting in AL2 exceeds 10, the system switches to priority mode.

### Threshold Logic
In priority mode, vehicles from AL2 are served continuously until the number of vehicles in AL2 drops below 5.  
Once the count goes below the threshold, the system exits priority mode and resumes normal traffic operation.

