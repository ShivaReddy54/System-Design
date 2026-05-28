#include <iostream>
#include <map>
#include <list>
#include <unordered_map>

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

    void setPriority(int p){
        priority = p;
    }
};

class TaskScheduler{
private:
    // priority -> list of tasks
    map<int, list<Task>, greater<int>> priorityMap;

    /*
        taskId ->
        (
            priority,
            iterator to task
        )
    */
    unordered_map<int, pair<int, list<Task>::iterator>> taskIndex;

public:

    void addTask(int id, int priority){
        // Add task to priority bucket
        priorityMap[priority].push_back(Task(id, priority));

        // Get iterator to newly inserted task
        auto it = prev(priorityMap[priority].end());

        // Store in index
        taskIndex[id] = {priority, it};
    }

    bool updatePriority(int id, int newPriority){
        // Task NOT present -> add it
        if(taskIndex.find(id) == taskIndex.end()){
            addTask(id, newPriority);
            return false;
        }

        // Existing task metadata
        auto [oldPriority, taskIt] = taskIndex[id];

        // Copy task
        Task task = *taskIt;

        // Remove from old list
        priorityMap[oldPriority].erase(taskIt);

        // Remove empty bucket
        if(priorityMap[oldPriority].empty()) priorityMap.erase(oldPriority);

        // Update task priority
        task.setPriority(newPriority);

        // Insert into new bucket
        priorityMap[newPriority].push_back(task);

        auto newIt = prev(priorityMap[newPriority].end());

        // Update index
        taskIndex[id] = {newPriority, newIt};

        return true;
    }

    Task getNextTask(){
        auto bucketIt = priorityMap.begin();
        auto& taskList = bucketIt->second;

        Task task = taskList.front();

        taskList.pop_front();

        // Remove from index
        taskIndex.erase(task.getId());

        // Remove empty bucket
        if(taskList.empty()) priorityMap.erase(bucketIt);

        return task;
    }

    void display(){
        cout << "\nScheduler State:\n";

        for(auto& [priority, tasks] : priorityMap){
            cout << "Priority "
                 << priority
                 << " -> ";

            for(auto& task : tasks) cout << task.getId() << " ";
            cout << endl;
        }
    }
};

int main(){
    TaskScheduler scheduler;

    scheduler.addTask(1, 5);
    scheduler.addTask(2, 10);
    scheduler.addTask(3, 5);

    scheduler.display();

    cout << "\nUpdating Task 1 -> Priority 20\n";

    scheduler.updatePriority(1, 20);

    scheduler.display();

    cout << "\nUpdating Task 100 -> Priority 7\n";

    scheduler.updatePriority(100, 7);

    scheduler.display();

    cout << "\nExecution Order:\n";

    while(true){
        try{
            Task t = scheduler.getNextTask();

            cout
                << "Task "
                << t.getId()
                << " Priority "
                << t.getPriority()
                << endl;
        }
        catch(...){
            break;
        }
    }
}