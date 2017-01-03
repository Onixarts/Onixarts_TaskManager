// OnixartsTaskManager advanced demo (but less code :) ). Using tasks in class.
// In this case, You don't need to setup delegate functions for Execute, Start and Stop events.
// All You need to do is to declare tasks using BEGIN_TASK and END_TASK macros and override OnStart(), OnExecute(), OnStop() methods.
// This example blinks LED and uses global defined TaskManager. 
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


// declare of our class, that will be using tasks to schedule some work
class MyClass
{
public:
	MyClass()
		: oneSecondTask(this) // init tasks in constructor initialization list
	{
	};

	//----------------------------------------------------------------------------
	// use macro to define task class and initial parameters
	// Put Task name, period, initial state, loop counter, Your parent class Type (MyClass here)
	BEGIN_TASK(OneSecondTask, 1000, TaskManager::TaskState::Running, 0, MyClass)
	void OnStart()
	{
		// put task start code here
		// You can omit this functions if they're not used
	}
	void OnExecute()
	{
		// You can access parent class (MyClass) methods by GetParent() method.
		// here we call ToogleLED() method from parent
		GetParent().ToogleLED();
	}
	void OnStop()
	{
		// put task stop code here
	}
	END_TASK(oneSecondTask) // task object name
	//----------------------------------------------------------------------------

	void ToogleLED()
	{
		digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW);
	}
};

// our class object declaration
MyClass myClassObject;

// and global task manager. Note, You have to put <x> number of tasks using in each class objects
TaskManager::Manager<1> taskManager;

void setup()
{
	Serial.begin(115200);
	Serial.println("Onixarts task manager advanced class demo");

	// add oneSecontTask object defined in MyClass to global taskManager
	taskManager.AddTask(myClassObject.oneSecondTask);

	// demo example
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	// update task manager will notify all the added tasks
	taskManager.Update();

}