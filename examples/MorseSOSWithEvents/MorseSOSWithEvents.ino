// OnixartsTaskManager Morse SOS with Events demo. 
// This demo use few tasks, each for SOS letters. Once the letter task is completed it runs next letter task.
// Finnaly builtin LED emits morse code ... _ _ _ ... each 5 seconds.
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

// declare taskManager object. Put tasks count in <x>, could be more, not less.
TaskManager::Manager<4> taskManager;

// tasks objects definition. Namespace Task is for clarity - it groups all the tasks objects into one namespace
namespace Task
{
	// Note, this tasks are stopped, executed 6 times (one for LED on and one for LED off) the same OnBlinkerExecute function with different duration
	TaskManager::Task S_blinker(OnBlinkerExecute, 100, TaskManager::TaskState::Stopped, 6);
	TaskManager::Task O_blinker(OnBlinkerExecute, 300, TaskManager::TaskState::Stopped, 6);
	TaskManager::Task S2_blinker(OnBlinkerExecute, 100, TaskManager::TaskState::Stopped, 6);

	// this task is autoruning, with unlimited loop count (0 in repeatCount parameter), executed each 5s.
	TaskManager::Task period(OnPeriodExecuted, 5000, TaskManager::TaskState::Running, 0);
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Onixarts task manager morse SOS demo");

	// Add all the tasks to taskManager for easier management
	taskManager.AddTask(Task::S_blinker);
	taskManager.AddTask(Task::O_blinker);
	taskManager.AddTask(Task::S2_blinker);
	taskManager.AddTask(Task::period);

	// Setup the StopEventDelegate functions to be called when specific task is stopped (after 6 executions)
	Task::S_blinker.SetStopEventDelegate(OnS_blinkerStopped);
	Task::O_blinker.SetStopEventDelegate(OnO_blinkerStopped);


	// demo example
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	// update task manager will notify all the added tasks
	taskManager.Update();
}
//---------------------------------------------------------------

// this function is called from each task. It toggles the builtin LED
void OnBlinkerExecute()
{
	digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW);
}

// this function is called when S_blinker task is stopped
void OnS_blinkerStopped()
{
	// run O_blinker task 
	Task::O_blinker.Start();
}

// this function is called when O_blinker task is stopped
void OnO_blinkerStopped()
{
	// run S2_blinker task after O_blinker is stopped
	Task::S2_blinker.Start();
}

// this function is called when period task is executed
void OnPeriodExecuted()
{
	// run S_blinker task to start SOS sequence
	// we call restart here, in case S_blinker is already running it will be restarted
	Task::S_blinker.Restart();
}