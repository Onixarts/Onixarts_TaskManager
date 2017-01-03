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

to be continued...
