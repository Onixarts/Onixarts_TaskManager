// Onixarts_TaskManager is a task schedulling library for Arduino.
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

#ifndef _TaskManager_h
#define _TaskManager_h

#include "Arduino.h"

#define BEGIN_TASK(taskClass, taskInterval, initialState, repeatCount, parentClass)\
		class taskClass : public Onixarts::Tools::TaskManager::Task{\
			parentClass& m_parent;\
			public:\
			taskClass(parentClass* parent) : m_parent(*parent)\
			, Onixarts::Tools::TaskManager::Task(NULL, taskInterval, initialState, repeatCount ){};\
			parentClass& GetParent() {return m_parent;}\
		
#define END_TASK(instanceName)	} instanceName;


namespace Onixarts 
{ 
	namespace Tools 
	{
		namespace TaskManager
		{
			namespace TaskState
			{
				const byte Stopped = 0;
				const byte Running = 1;
			};

			typedef void(*TaskExecuteDelegate)(void);
			typedef void(*TaskStateChangeDelegate)(void);

			class Task
			{
			public:
				Task(TaskExecuteDelegate executeDelegate, unsigned long taskInterval, byte taskState = TaskState::Running, int repeatCount = 0);

				void Update(unsigned long now);			// Update task will check if it should Execute user function
				void Execute();							// Executes user function.
				void Start();							// Starts current task. If task is already running, it does not do anything. 
				void Restart();							// Restart current task. If task is running its time progress and repeat loop will be reset. If task is stopped it will start. OnStartEvent is raised.
				void Stop();							// Stops current task if it is running and raise OnStopEvent.

				void SetLastExecutionTime(unsigned long lastExecutionTime) { this->m_lastExecutionTime = lastExecutionTime; }	// Sets last execution time
				bool IsRunning() { return m_state == TaskState::Running; }		// Check if task is running
				byte GetState() { return m_state; }			// Returns task current state
				void SetRepeatCount(int repeatCount) { m_repeatCount = repeatCount; m_repeatIndex = 0; } // Set repeated Task 
				void SetTaskInterval(unsigned long interval) { m_taskInterval = interval; }


				void SetStartEventDelegate(TaskStateChangeDelegate startDelegate) { m_startDelegate = startDelegate; }
				void SetStopEventDelegate(TaskStateChangeDelegate stopDelegate) { m_stopDelegate = stopDelegate; }

				virtual void OnExecute() {}
				virtual void OnStart() {} // not called on autostart
				virtual void OnStop() {}

			protected:
				byte m_state;

				TaskExecuteDelegate m_executeDelegate;	// task execute callback delegate
				TaskStateChangeDelegate m_startDelegate;	// task start callback delegate, not called on autostart
				TaskStateChangeDelegate m_stopDelegate;	// task stop callback delegate

				unsigned long m_taskInterval;				// interval in milliseconds between executions, or time from start to one time execution
				unsigned long m_lastExecutionTime;		// last execution time in milliseconds
				int	m_repeatCount;						// How many times Task should be executed, 0 = infinite
				int m_repeatIndex;						// current repeat index
			};
			//-------------------------------------------------------------------------------------------------------------------

			template <uint8_t MAX_TASKS>
			class Manager
			{
			private:
				Task* tasks[MAX_TASKS];
				uint8_t numberOfTasks;
			public:
				Manager()
					: numberOfTasks(0)
				{
				}

				bool AddTask(Task& task)
				{
					if (numberOfTasks == MAX_TASKS)
						return false; // to many tasks, increase Scheduler<MAX_TASKS>

					task.SetLastExecutionTime(millis()); // store current milis as lastExecutionTime when task added to manager
					tasks[numberOfTasks++] = &task;
					return true;
				}

				// update all the tasks in the list
				void Update()
				{
					unsigned long now = millis();

					for (uint8_t i = 0; i < numberOfTasks; i++)
						tasks[i]->Update(now);
				}
			};
		}
	}
}

#endif
