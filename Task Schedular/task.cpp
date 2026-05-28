#include <iostream>
#include <map>
#include <queue>
#include <stdexcept>

using namespace std;

class Task{
private:
    int id;
    int priority;

public:
    Task(int id, int priority)
        : id(id), priority(priority) { }

    int getId(){
        return id;
    }

    int getPriority(){
        return priority;
    }
};

class TaskScheduler{
private:
    // Higher priority first
    map<int, queue<Task>, greater<int>> scheduler;

public:
    void addTask(int id, int priority){
        Task task(id, priority);
        scheduler[priority].push(task);
    }

    Task getNextTask(){
        if(scheduler.empty()) throw runtime_error("No tasks available");

        // Highest priority bucket
        auto it = scheduler.begin();

        Task task = it->second.front();
        it->second.pop();

        // Remove empty queue
        if(it->second.empty()) scheduler.erase(it);

        return task;
    }

    Task peekNextTask(){
        if(scheduler.empty()) throw runtime_error("No tasks available");

        auto it = scheduler.begin();
        return it->second.front();
    }

    bool empty(){
        return scheduler.empty();
    }

    int size(){
        int total = 0;
        for(auto& entry : scheduler) total += entry.second.size();

        return total;
    }

    void display(){
        cout << "\nScheduler State:\n";

        for(auto& entry : scheduler){
            int priority = entry.first;
            queue<Task> q = entry.second;

            cout << "Priority " << priority << " -> ";

            while(!q.empty()){
                cout << q.front().getId() << " ";
                q.pop();
            }

            cout << endl;
        }
    }
};

int main(){
    TaskScheduler scheduler;

    scheduler.addTask(101, 5);
    scheduler.addTask(102, 10);
    scheduler.addTask(103, 5);
    scheduler.addTask(104, 2);
    scheduler.addTask(105, 10);

    scheduler.display();

    cout << "\nExecuting Tasks:\n";

    while(!scheduler.empty()){
        Task t = scheduler.getNextTask();

        cout
            << "Task ID: "
            << t.getId()
            << " Priority: "
            << t.getPriority()
            << endl;
    }
}