#include <iostream>

#include "SimulationExecutive.h"
#include "Task.h"

using namespace std;

#define TRACE 0

int Task::_nextID = 0;

Task::Task(string name)
{
	_name = name;
	_nextTask = 0;
	_id = _nextID++;
}

void Task::SetNextTask(Task *nextTask)
{
	_nextTask = nextTask;
}

void Task::Arrive(Entity *entity)
{
	if (entity->GetID() == 4) {
		int i = 5;
	}
#if TRACE
	cout << "time = " << GetSimulationTime() << ", " << _taskType << " " << _name << ", arrive, entity: " << entity->GetID() << endl << flush;
#endif
	TaskProcess(entity);
}

void Task::Depart(Entity *entity)
{
#if TRACE
	cout << "time = " << GetSimulationTime() << ", " << _taskType << " " << _name << ", depart, entity: " << entity->GetID() << endl << flush;
#endif
	entity->SetSource(_id);
	_nextTask->Arrive(entity);
}

string Task::GetName()
{
	return(_name);
}

int Task::GetID()
{
	return(_id);
}
