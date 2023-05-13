#ifndef TASK_H
#define TASK_H

#include <string>
#include "Entity.h"

class Task
{
public:
	void SetNextTask(Task *nextTask);
	/*
	Behavior:		Defines that task an entity goes to next after the current task is complete.
	Parameters:		nextTask:	user supplies the link to the next task to be executed
	Return Value:	none
	*/

	void Arrive(Entity *entity);
	/*
	Behavior:		Method called by the previous task working on an entity to send the entity to this task.
	The method calls TaskProcess( entity) to pass the entity on to code specific to the task
	implementation.
	Parameters:		entity:	the entity the task is to work on
	Return Value:	none
	*/

	void SetName(std::string name);
	/*
	Behavior:		Allows the user to set the task name after task instantiation.
	Parameters:		name:		user supplied task name
	Return Value:	none
	*/

	std::string GetName();
	/*
	Behavior:		Returns the task name to the user.
	Parameters:		none
	Return Value:	task name
	*/

	int GetID();

protected:
	Task *_nextTask;	//next task to be executed after completion of this task
	std::string _taskType;	//string set internally with the name of the implemented task type

	Task(std::string name);
	/*
	Behavior:		constructor
	Parameters:		name:		user supplied task name
	Return Value:	none
	*/

	void Depart(Entity *entity);
	/*
	Behavior:		Method called by a task implementation to send an entity to another task.  Prior to the
	call, _nextEntity should have a reference to the next Task to be executed.
	Parameters:		entity:	the entity to be sent to the next task
	Return Value:	none
	*/

	virtual void TaskProcess(Entity *entity) = 0;
	/*
	Behavior:		Virtual method to be implemented by specific task types.  This method will be the entry point
	to the task implementation called by Arrive.
	Parameters:		entity:	the entity the task is to work on
	Return Value:	none
	*/

private:
	std::string _name;		//task name
	int _id;
	static int _nextID;
};

#endif
