// OnixartsTaskManager Hello World demo. 
// 
// Code explanation: TODO
// Github: https://github.com/Onixarts/Onixarts_TaskManager
// Author's site: http://onixarts.pl
// Contact: software@onixarts.pl
// 
// Copyright (C) 2017  Bartosz Rosa (onixarts.pl)
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.

#include "Arduino.h"
#include <OnixartsTaskManager.h>

using namespace Onixarts::Tools;

// declare taskManager object. Put tasks count in <x>
TaskManager::Manager<2> taskManager;

// tasks objects definition. Namespace Task is for clarity - it groups all the tasks objects into one namespace
namespace Task
{
	TaskManager::Task oneSecondBlinker(OnOneSecondBlinkerExecute, 1000, TaskManager::TaskState::Stopped, 0);
	TaskManager::Task task2(OnTask2Execute, 1000, TaskManager::TaskState::Stopped, 4);
	//Onixarts::Tools::Task task3(OnTask3Execute, 500, Onixarts::Tools::Task::TaskState::Stopped, 4); // You should also put 3 in taskmanager definition when this line is uncommented
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Onixarts task manager demo");

	// Add all the tasks to taskManager
	taskManager.AddTask(Task::oneSecondBlinker);
	taskManager.AddTask(Task::task2);
	//taskManager.AddTask(Task::task3);


	// demo example, set as output
	pinMode(LED_BUILTIN, OUTPUT);

	// run first task, it will blink LED7 each second
	Task::oneSecondBlinker.Start();
	//Task::task2.Start();
}

void loop()
{
	// update task manager will notify all the added tasks
	taskManager.Update();
}

// called by task Manager when OneSecondBlinker task should be executed 
void OnOneSecondBlinkerExecute()
{
	Serial.println("OneSecondBlinker Executed");
	digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW);
}

// called by task Manager when Task2 task should be executed 
void OnTask2Execute()
{
	Serial.println("Task2 Executed");
	digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW);
}
