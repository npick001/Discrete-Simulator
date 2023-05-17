#pragma once
#include <iostream>
#include "Task.h"
#include "Distribution.h"
#include "SimulationExecutive.h"
#include "Set.h"

using namespace std;

class Source : public Task
{
public:
	Source(std::string name, int numGen, Entity* entity, Distribution* dist);

	void TaskProcess(Entity* e)
	{
		std::cout << "ERROR -> SHOULD NOT BE CALLED";
	}

private:
	Distribution* _dist;
	int _numGen;
	Entity* _entity;
	class ArriveEA;
	void ArriveEM();
};

class ServerQueue : public Task {
public:
	ServerQueue(std::string name, Distribution* serviceTime);
	void TaskProcess(Entity* e);
private:

	class StartProcessingEA;
	void StartProcessingEM();
	class EndProcessingEA;
	void EndProcessingEM(Entity* e);

	enum ServerState { busy, idle };
	ServerState _state;

	Distribution* _serviceTime;

	static LinkedList<Entity>* _queue;
};

/****************************************************************/
/* SystemOfSSSQs:                                               */
/* defines a system consisting of n SSSQs                       */
/* provides functionality to get the reference to a random SSSQ */
/****************************************************************/
class SystemOfSSSQs
{
public:
	SystemOfSSSQs(int numSSSQs, Distribution* distr);
	/* initialize the system of SSSQs to have n SSSQs with a service time of distr */
	ServerQueue* GetRandomSSSQ();
	/* returns a pointer to a random SSSQ */

	void SetNextTasks(Task* next);

private:
	int _numSSSQs;
	ServerQueue** _sssqs;
};

class Delay : public Task {
public:
	Delay(std::string name, Distribution* d);
	void TaskProcess(Entity* e);
private:
	Distribution* _delay;

	class DepartEA;
	void DepartEM(Entity* e);
};

class Sink : public Task
{
public:
	Sink(string name) : Task(name) {};

	void TaskProcess(Entity* e);
};

typedef bool(*DecisionFn)(Entity*);
typedef int(*DecisionNFn)(Entity*);

class DecisionTask : public Task
{
public:
	DecisionTask(std::string name, DecisionFn Decision);
	/*
	Behavior:		Constructor, defining the decision function
	Parameters:		name:		task name
					Decision:	function used to evaluate decision
	Return Value:	none
	*/
	void SetNextTasks(Task* trueTask, Task* falseTask);
	/*
	Behavior:		Defines the next tasks based on the decision outcome
	Parameters:		trueTask:	next task if decision is true
					falseTask:	next task if decision is falst
	Return Value:	none
	*/
private:
	void TaskProcess(Entity* entity);
	/*
	Behavior:		This task evaluations the function _Decision( entity).  If
					true, the next task is set to _trueTask, else to _falstTask.
	Parameters:		entity:	the entity the task is to work on
	Return Value:	none
	*/
	DecisionFn _Decision;			//decision logic function
	Task* _trueTask, * _falseTask;	//tasks entity is sent to based on the Decision function
};

class DecisionNTask : public Task
{
public:
	DecisionNTask(string name, int n, DecisionNFn DecisionN);
	/*
	Behavior:		Constructor, defining the decision function
	Parameters:		name:		task name
					n:			number of decision options
					Decision:	function used to evaluate decision
	Return Value:	none
	*/
	void SetNextTasks(int index, Task* nextTask);
	/*
	Behavior:		Defines the next task based on the decision outcome
					indicated by index
	Parameters:		nextTask:	next task if decision is indicated by index
	Return Value:	none
	*/
private:
	void TaskProcess(Entity* entity);
	/*
	Behavior:		This task evaluations the function _DecisionN( entity), which gives
					an index back to select the next task from tasks.
	Parameters:		entity:	the entity the task is to work on
	Return Value:	none
	*/
	DecisionNFn _DecisionN;	//decision function returning 1 of N outcomes
	Task** tasks;			//array of size N possible decision outcomes
};

/*
The following class defines a new entity, batch, which is a collection of
entities grouped together to pass through a set of tasks as one unit.
*/
class Batch : public Entity
{
public:
	Batch();
	~Batch();
	void AddEntity(Entity* entity);
	Entity* GetEntity();
	Entity* New();
	int GetSize();
private:
	LinkedList<Entity> _b;
};

/*
The Batch task accepts the arrival of entities, forming them into batches of entities
(of type Batch) of a fixed size.  Once a batch is complete, it is sent on to the next task.
*/
class BatchTask : public Task
{
public:
	BatchTask(string name, int batchSize);
private:
	void TaskProcess(Entity* entity);
	int _batchSize;
	int _count;
	Batch* _batch;
};

/*
This task accepts a batch of entities (type Batch), removes each entity from the batch,
sending them all on to the next task.  The batch is then disposed of.
*/
class UnBatchTask : public Task
{
public:
	UnBatchTask(string name);
private:
	void TaskProcess(Entity* entity);
};

class ResourcePool
{
public:
	ResourcePool(std::string name, int numResources);
	void Request(EventAction* ea);
	void Release();
	std::string GetName();
private:
	int _resources;
	std::string _name;
	LinkedList<EventAction>* _requestQueue;
};

class AcquireTask : public Task
{
public:
	AcquireTask(std::string name, ResourcePool* rp);
private:
	class SendRequestEA;
	void SendRequestEM(Entity* e);
	class RequestGrantedEA;
	void RequestGrantedEM(Entity* e);

	void TaskProcess(Entity* entity);
	ResourcePool* _rp;
};

class ReleaseTask : public Task
{
public:
	ReleaseTask(std::string name, ResourcePool* rp);
private:
	void TaskProcess(Entity* entity);
	ResourcePool* _rp;
};

class ForkTask : public Task
{
public:
	ForkTask(string name, int n);
	void SetNextTasks(int index, Task* nextTask);
private:
	void TaskProcess(Entity* entity);
	int _numNextTasks;
	Task** tasks;			//array of size N possible decision outcomes
};

class JoinTask : public Task
{
public:
	JoinTask(string name, int n);
	void SetPreviousTaskID(int index, int id);
private:
	void TaskProcess(Entity* entity);
	int _numIncomingQueues;
	Set<Entity>* _incomingQueues;			//array of size N possible incoming queues of entities
	int* _previousTaskID;
};

