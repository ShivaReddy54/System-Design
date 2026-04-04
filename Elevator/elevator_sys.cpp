#include <bits/stdc++.h>
using namespace std;

enum Direction { UP, DOWN, IDLE };

class Elevator {
public:
    int id;
    int currentFloor;
    Direction direction;

    set<int> upStops;    // ascending
    set<int> downStops;  

    Elevator(int id, int floor = 0) {
        this->id = id;
        currentFloor = floor;
        direction = IDLE;
    }

    // Add stop based on direction
    void addStop(int floor) {
        if (floor > currentFloor) {
            upStops.insert(floor);
        } else if (floor < currentFloor) {
            downStops.insert(floor);
        }
        // if same floor → ignore or open door
    }

    // Decide next direction
    void updateDirection() {
        if (direction == UP) {
            if (!upStops.empty()) return;
            if (!downStops.empty()) direction = DOWN;
            else direction = IDLE;
        }
        else if (direction == DOWN) {
            if (!downStops.empty()) return;
            if (!upStops.empty()) direction = UP;
            else direction = IDLE;
        }
        else { // IDLE
            if (!upStops.empty()) direction = UP;
            else if (!downStops.empty()) direction = DOWN;
        }
    }

    // Move one step
    void step() {
        updateDirection();

        if (direction == UP) {
            currentFloor++;
            if (upStops.count(currentFloor)) {
                cout << "Elevator " << id << " stopping at floor " << currentFloor << "\n";
                upStops.erase(currentFloor);
            }
        }
        else if (direction == DOWN) {
            currentFloor--;
            if (downStops.count(currentFloor)) {
                cout << "Elevator " << id << " stopping at floor " << currentFloor << "\n";
                downStops.erase(currentFloor);
            }
        }
    }

    bool isIdle() {
        return direction == IDLE && upStops.empty() && downStops.empty();
    }
};

class ElevatorSystem {
public:
    vector<Elevator> elevators;

    ElevatorSystem(int n) {
        for (int i = 0; i < n; i++) {
            elevators.emplace_back(i, 0);
        }
    }

    // External request
    void requestLift(int floor, Direction dir) {
        int best = findBestElevator(floor, dir);
        cout << "Assigning Elevator " << best << " to floor " << floor << "\n";
        elevators[best].addStop(floor);
    }

    // Internal request
    void selectFloor(int elevatorId, int floor) {
        elevators[elevatorId].addStop(floor);
    }

    // Simple heuristic: closest elevator
    int findBestElevator(int floor, Direction dir) {
        int best = 0;
        int minDist = INT_MAX;

        for (int i = 0; i < elevators.size(); i++) {
            int dist = abs(elevators[i].currentFloor - floor);

            // Prefer idle or same direction
            if (elevators[i].direction == dir || elevators[i].isIdle()) {
                if (dist < minDist) {
                    minDist = dist;
                    best = i;
                }
            }
        }

        return best;
    }

    // Simulate one time step
    void step() {
        for (auto &e : elevators) {
            e.step();
        }
    }

    // Debug
    void status() {
        for (auto &e : elevators) {
            cout << "Elevator " << e.id 
                 << " | Floor: " << e.currentFloor 
                 << " | Dir: " << e.direction << "\n";
        }
        cout << "-------------------\n";
    }
};

int main() {
    ElevatorSystem system(2);

    // External requests
    system.requestLift(5, UP);
    system.requestLift(2, DOWN);

    // Simulate
    for (int i = 0; i < 10; i++) {
        system.step();
        system.status();
    }

    // Internal request
    system.selectFloor(0, 8);

    for (int i = 0; i < 10; i++) {
        system.step();
        system.status();
    }

    return 0;
}