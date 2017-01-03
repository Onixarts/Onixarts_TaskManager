#include "OnixartsTaskManager.h"

using namespace Onixarts::Tools::TaskManager;

Task::Task( TaskExecuteDelegate executeDelegate, unsigned long taskInterval, byte taskState, int repeatCount )
: m_startDelegate( NULL)
, m_stopDelegate(NULL)
, m_repeatIndex(0)
{
	m_executeDelegate = executeDelegate;
	m_taskInterval = taskInterval;
	m_state = taskState;
	m_repeatCount = max(0, repeatCount);
}

void Task::Update(unsigned long now)
{
	if(m_state == TaskState::Running )
	{
		if( now - m_lastExecutionTime > m_taskInterval )
		{
			m_lastExecutionTime = now;
			Execute();
		}
	}
}

void Task::Execute()
{
	OnExecute();
	if( m_executeDelegate != NULL )
		m_executeDelegate();
	if( m_repeatCount > 0 )
	{
		m_repeatIndex++;
		if( m_repeatIndex == m_repeatCount )
			Stop();
	}
}
 
void Task::Start()
{
	if (m_state == TaskState::Stopped)
		Restart();
}

void Task::Restart()
{
	m_lastExecutionTime = millis();
	m_state = TaskState::Running;
	m_repeatIndex = 0;

	OnStart();
	if( m_startDelegate != NULL )
		m_startDelegate();
}

void Task::Stop()
{
	if (m_state == TaskState::Running)
	{
		m_state = TaskState::Stopped;
		m_repeatIndex = 0;
		OnStop();
		if (m_stopDelegate != NULL)
			m_stopDelegate();
	}
}
