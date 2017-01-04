# Onixarts_TaskManager
Task schedulling library for Arduino. You can add many tasks that will call execution functions in defined time period.

## Features
- Unlimited task count
- Start, Execute and Stop events for each task
- Task autostart or start controlled by code
- Infinite loop or defined times execution
- Task can be stopped, started and restarted
- No timer, using millis()
- millis overflow safe
- Events global callback functions
- Can be used internally in class objects
- No dynamic memory allocation

## Overview
Library is build with two classes:

`Task` defines the job, how often it should be executed and how many times. For example, it could define blinking LED 5 times for 200ms.

`Manager` is a helper class that dispatch current milis() for all the tasks handled by manager. After defining `Tasks` You call `AddTask(task)` method and in main `loop()` function You call `manager.Update()` method to all handled tasks work properly. Manager is defined as class template with handled tasks count parameter. 


## Basic usage
You should instantiate `Manager` class like this:

```C++
using namespace Onixarts::Tools;
TaskManager::Manager<8> taskManager;
```

It will generate `Manager` object with maximum of 8 tasks. This value should be equal or greater than physical task count.

Declaration of tasks is as simple as:

```C++ 
TaskManager::Task task1(OnTask1Execute, 1000, TaskManager::TaskState::Running, 0);
```
First parameter is the `ExecuteEventDelegate` callback function which is called when the time defined in `taskInterval`(parameter 2) has expired from task's start time. Third parameter defines if the task should be initially running or stopped. The last one is `repeatCount` parameter, which determines how many times task would execute callback function. 0 means the task is executed in infinite loop. You can always stop it by calling `task1.Stop()` method, and run it again by `task1.Start()` or `task1.Restart()` methods. 

Next, in `setup()` function add defined tasks into manager.

```C++
void setup()
{
    taskManager.AddTask(task1);
}
```

The last thing You should do is to keep each task alive. For that call `taskManager.Update()` in `loop()` function.

```C++
void loop()
{
    taskManager.Update();
}
```

It will notify all tasks registred by `AddTask()` method with current `millis()` timestamp.

You may also want to write callback function that will be called by Task:

```C++
void OnTask1Execute()
{
  // put Your code here
}
```

## Task's events
Task can be Started, Restarted and Stopped by calling methods `Start()`, `Restart()` and `Stop()`. When task is started or stopped it can call callback function, just like one being called when task's interval is expired. The last one function delegate is passed into Task constructor as first parameter. You can always pass NULL, so task will not call any function at all.
Setting callback functions is easy:

```C++
task1.SetStartEventDelegate(OnTask1Started);
task1.SetStopEventDelegate(OnTask1Stopped);
```
and implementation of this functions:

```C++
void OnTask1Started()
{
    //put task started code here
}

void OnTask1Stopped()
{
    //put task stopped code here
}
```
This functions are usefull if your tasks are started and stopped. For example, You can turn siren ON in start event and turn it OFF on stop event, when task is configuread as executed 1 time for 30 seconds. You simply call `sirenTask.Start()` and it will stay enabled for 30 seconds.

## Start vs Restart
Both methods will start task if it is currently stopped. Calling `Start()` on running task won't do anything, while `Restart()` will reset task (current time progress and loop counter if specified) and start it again. Note that start event will be generated on each task restart.

## Using tasks in class
As You may know, You can't simply pass object method as callback function, because in compilation time `this` pointer is undefined - so the compiler won't know which object's method should be called. In such cases You are usually define global callback function and then in function body You call `myObject1.DoSomething()` or declare class method as static - but it has no access to the object's data however. 

In Onixarts_TaskManager library You can implement derived tasks inside Your class. There are some macros defined that will simplify proces to minimum lines od code.

Below is an example of custom class `MyClass` with defined `task1` inside.

```C++
class MyClass : public TaskManager::Manager<1>
{
public:
    MyClass()
    : task1(this)
    {
       AddTask(task1);
    }
    
    BEGIN_TASK(Task1, 1000, TaskManager::TaskState::Running, 0, MyClass)
    void OnExecute()
    {
        // put Your task executed code here
        GetParent().DoSomething();
    }
    END_TASK(task1)
    
    void DoSomething()
    {
        // put Your code here
    }
};
```
You define task with `BEGIN_TASK` and `END_TASK` macros - always put them together. In begin macro put derived task name (it could be `Task1` as above), taskInterval (1000), default state (Running), repeat count (0) and name of Your parent class (`MyClass` here). In between those macros You can put one of the three methods that override empty methods from base `Task` class: `OnStart()`, `OnExecute()` and `OnStop()`. You don't have to add callback function like I did it before. This methods will be called automatically when events occured.

To access Your parent class members from inside the task, use `GetParent()` method.

In `TASK_END` macro You specify task instance member like `task1`. It is also important to instantiate this task when Your main class object is being constructed, so in `MyClass` constructor initialization list put each defined tasks like `task1(this)`.

How about updating tasks inside class? Well, there are few methods. You can put `TaskManager::Manager` in global space and `AddTask()` for each object's task, then call `Update()` in `loop()` function. This is not very good solution in case You add new task somewhere in code, You always must remember to register it in main ino file. Better solution is to put `Manager` instance inside each class. Then You register only current tasks in class constructor. If manager is defined as public You can call it in ino's `loop()` function like `myObject.taskManager.Update()`.

In example above I use third technique. `MyClass` inherits from `TaskManager::Manager` so it becomes a manager itself. In `loop()` function I just called `myObject().Update()` as `MyClass` inherits `Update()` method from manager.
