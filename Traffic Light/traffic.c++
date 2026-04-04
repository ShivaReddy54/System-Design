#include <bits/stdc++.h>
using namespace std;

enum Signal { GREEN, YELLOW, RED };
enum Direction { NORTH, EAST, SOUTH, WEST };

string signalToString(Signal s) {
    if (s == GREEN) return "GREEN";
    if (s == YELLOW) return "YELLOW";
    return "RED";
}

string directionToString(Direction d) {
    if (d == NORTH) return "NORTH";
    if (d == EAST) return "EAST";
    if (d == SOUTH) return "SOUTH";
    return "WEST";
}

class Structure {
public:
    Direction direction;
    Signal color;
    int timer;

    Structure(Direction d, Signal c, int t) {
        direction = d;
        color = c;
        timer = t;
    }
};

class Traffic_Light {
private:
    vector<Structure> lights;
    int activeIndex; // current GREEN

public:
    Traffic_Light() {
        lights = {
            Structure(NORTH, RED, 0),
            Structure(EAST, RED, 0),
            Structure(SOUTH, RED, 0),
            Structure(WEST, RED, 0)
        };

        activeIndex = 0;
        lights[0].color = GREEN;
        lights[0].timer = 5;
    }

    Structure getCurrentState(Direction d) {
        return lights[d];
    }

    void switchState(Direction d, Signal color, int timer = 0) {
        lights[d].color = color;
        lights[d].timer = timer;
    }

    void tick() {
        Structure &curr = lights[activeIndex];
        curr.timer--;

        if (curr.timer > 0) return;

        if (curr.color == GREEN) {
            curr.color = YELLOW;
            curr.timer = 2;
        } else if (curr.color == YELLOW) {
            curr.color = RED;

            activeIndex = (activeIndex + 1) % 4;

            lights[activeIndex].color = GREEN;
            lights[activeIndex].timer = 5;
        }
    }

    void setTimer(Direction d, int timer) {
        lights[d].timer = timer;
    }

    void handleEmergency(Direction d) {
        for (auto &l : lights) {
            l.color = RED;
            l.timer = 0;
        }

        activeIndex = d;
        lights[d].color = GREEN;
        lights[d].timer = 5;
    }

    void printState() {
        for (auto &l : lights) {
            cout << directionToString(l.direction) << " : "
                 << signalToString(l.color)
                 << " (" << l.timer << ")\n";
        }
        cout << "-----------------\n";
    }
};

int main() {
    Traffic_Light t;

    for (int i = 0; i < 15; i++) {
        cout << "Time: " << i << "\n";
        t.printState();
        t.tick();
    }

    cout << "🚨 Emergency at EAST\n";
    t.handleEmergency(EAST);
    t.printState();

    return 0;
}