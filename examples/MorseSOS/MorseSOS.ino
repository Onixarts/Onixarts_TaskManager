// OnixartsTaskManager Morse SOS demo. 
// This demo use one task for emiting morsce code for SOS. Once the letter task is completed (stops) it is restarted with next char values.
// Finnaly builtin LED emits morse code ... _ _ _ ... each 5 seconds.
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

// declare taskManager object. Put tasks count in <x>, could be more, not less.
TaskManager::Manager<4> taskManager;

// tasks objects definition. Namespace Task is for clarity - it groups all the tasks objects into one namespace
namespace Task
{
	// Note, this tasks are stopped, executed 6 times (one for LED on and one for LED off) the same OnBlinkerExecute function with different duration
	TaskManager::Task letter_blinker(OnBlinkerExecute, 100, TaskManager::TaskState::Stopped, 6);

	// this task is autoruning, with unlimited loop count (0 in repeatCount parameter), executed each 5s.
	TaskManager::Task period(OnPeriodExecuted, 5000, TaskManager::TaskState::Running, 0);
}

// variable stores current letter index
byte currentLetter = 0;

void setup()
{
	Serial.begin(115200);
	Serial.println("Onixarts task manager morse SOS demo");

	// Add all the tasks to taskManager for easier management
	taskManager.AddTask(Task::letter_blinker);
	taskManager.AddTask(Task::period);

	// Setup the StopEventDelegate function to be called when specific task is stopped
	Task::letter_blinker.SetStopEventDelegate(Onletter_blinkerStopped);

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
void Onletter_blinkerStopped()
{
	// when task is done (char is emited) set up new task values for new char
	switch (currentLetter)
	{
	case 0:
		// after first S char, set new values for O char
		Task::letter_blinker.SetTaskInterval(300);
		// start blinker again
		Task::letter_blinker.Start();
		break;
	case 1:
		// after second O char, set new values for second S char
		Task::letter_blinker.SetTaskInterval(100);
		// start blinker again
		Task::letter_blinker.Start();
		break;
	case 2:
		// after second S char we do nothing. It is already stops
		break;
	}

	currentLetter++;
	if (currentLetter > 2)
		currentLetter = 0;
}

// this function is called when period task is executed
void OnPeriodExecuted()
{
	// run letter_blinker task to start SOS sequence
	// we call restart here, in case letter_blinker is already running it will be restarted
	Task::letter_blinker.Restart();
}