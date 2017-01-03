// OnixartsTaskManager advanced demo (but less code :) ). Using tasks in class.
// In this case, You don't need to setup delegate functions for Execute, Start and Stop events.
// All You need to do is to declare tasks using BEGIN_TASK and END_TASK macros and override OnStart(), OnExecute(), OnStop() methods.
// This example blinks LED and uses TaskManager inheritance. It also shows how to put taskManager object inside custom MyClass class.
// 
// Code explanation: TODO
// Github: TODO
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

// declare our class, that will be using tasks to schedule some work
// You can either inherit from TaskManager::Manager class (less code) or add Manager inside MyClass as (reuire some additional code)
class MyClass : public TaskManager::Manager<1>
{
	// you can also declare taskManager object in class instead of inherite from it
	// TaskManager::Manager<1> taskManager;
public:

	// You can also put Manager here, to make it accessable from out of the class
	// TaskManager::Manager<1> taskManager;

	MyClass()
		: oneSecondTask(this) // init tasks in constructor initialization list
	{
		// add class task(s) to taskManager
		AddTask(oneSecondTask);

		// if MyClass has internal taskManager use this code instead
		// taskManager.AddTask(oneSecondTask);
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

	// if MyClass has provate internal taskManager use this code and call it from main loop() function
	//void Update()
	//{
	//	taskManager.Update();
	//}
};

// our class object declaration
MyClass myClassObject;

void setup()
{
	Serial.begin(115200);
	Serial.println("Onixarts task manager advanced class demo");

	// demo example
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	// update myClassObject if it inherits from TaskManager::Manager
	myClassObject.Update();

	// or call Update method on myClassObject
	//myClassObject.Update()

	// or access internal taskManager if you make it public (here it is private, so You cannot access it directly)
	//myClassObject.taskManager.Update();

}