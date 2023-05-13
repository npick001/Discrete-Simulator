#include "MyTask.h"
#include <random>
#include <iostream>

class Source::ArriveEA : public EventAction {

public:
	ArriveEA(Source* src) {
		_src = src;
	}

	void Execute() {
		_src->ArriveEM();
	}
private:
	Source* _src;
};

void Source::ArriveEM() {

	Depart(_entity->New());

	if (_numGen > 0) {
		Time t = _dist->GetRV();

		std::cout << "Scheduling source arrival event in " << t << std::endl;
		ScheduleEventIn(t, new ArriveEA(this));
		_numGen--;
	}
}

Source::Source(std::string name, int numGen, Entity* entity, Distribution* dist) : Task(name) {
	_numGen = numGen;
	_dist = dist;
	_entity = entity;
	ScheduleEventIn(0.0, new ArriveEA(this));
};

void Sink::TaskProcess(Entity* entity) {
	cout << "Deleting " << entity->GetID() << endl;
	delete(entity);
}

LinkedList<Entity>* ServerQueue::_queue = new LinkedList<Entity>;

ServerQueue::ServerQueue(std::string name, Distribution* serviceTime) : Task(name) {
	_state = idle;
	_serviceTime = serviceTime;
}

class ServerQueue::StartProcessingEA : public EventAction {
public:
	StartProcessingEA(ServerQueue* s) {
		_s = s;
	}

	void Execute() {
		_s->StartProcessingEM();
	}
private:
	ServerQueue* _s;
};

class ServerQueue::EndProcessingEA : public EventAction {
public:
	EndProcessingEA(ServerQueue* s, Entity* e) {
		_s = s;
		_e = e;
	}

	void Execute() {
		_s->EndProcessingEM(_e);
	}
private:
	ServerQueue* _s;
	Entity* _e;
};

void ServerQueue::StartProcessingEM() {
	_state = busy;
	Entity* e = _queue->GetEntity();
	ScheduleEventIn(_serviceTime->GetRV(), new EndProcessingEA(this, e));
}

void ServerQueue::EndProcessingEM(Entity* e) {
	_state = idle;
	if (!_queue->IsEmpty()) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}
	Depart(e);
}

void ServerQueue::TaskProcess(Entity* e) {
	_queue->AddEntity(e);

	if (_state == idle) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}
}

SystemOfSSSQs::SystemOfSSSQs(int numSSSQs, Distribution* distr)
{
	_numSSSQs = numSSSQs;
	_sssqs = new ServerQueue * [numSSSQs];
	for (int i = 0; i < numSSSQs; i++) {
		_sssqs[i] = new ServerQueue("Server", distr);
	}
}

ServerQueue* SystemOfSSSQs::GetRandomSSSQ()
{
	static std::default_random_engine generator;
	static std::uniform_int_distribution<> unif_distr(0, _numSSSQs - 1); // define the range

	int id = unif_distr(generator);
	return _sssqs[id];
}

void SystemOfSSSQs::SetNextTasks(Task* next) {

	for (int i = 0; i < _numSSSQs; i++) {
		_sssqs[i]->SetNextTask(next);
	}

}

class Delay::DepartEA : public EventAction {

public:
	DepartEA(Delay* d, Entity* e) {
		_e = e;
		_d = d;
	}

	void Execute() {
		_d->DepartEM(_e);
	}

private:
	Entity* _e;
	Delay* _d;
};

void Delay::DepartEM(Entity* e) {
	Depart(e);
}

void Delay::TaskProcess(Entity* e) {
	ScheduleEventIn(_delay->GetRV(), new DepartEA(this, e));
}

Delay::Delay(std::string name, Distribution* d) : Task(name) {
	_delay = d;
}

/***************************************
	DECISION TASK IMPLEMENTATION
***************************************/

DecisionTask::DecisionTask(std::string name, DecisionFn Decision) : Task(name) {
	_Decision = Decision;
	_trueTask = NULL;
	_falseTask = NULL;
}

void DecisionTask::SetNextTasks(Task* trueTask, Task* falseTask) {
	_trueTask = trueTask;
	_falseTask = falseTask;
}

void DecisionTask::TaskProcess(Entity* entity) {

	bool route = _Decision(entity);

	if (route) {
		SetNextTask(_trueTask);
		std::cout << "Decision: Sending to task: " << _trueTask->GetName() << std::endl;
	}
	else {
		SetNextTask(_falseTask);
		std::cout << "Decision: Sending to task: " << _falseTask->GetName() << std::endl;
	}
	Depart(entity);
}

DecisionNTask::DecisionNTask(string name, int n, DecisionNFn DecisionN) : Task(name) {
	tasks = new Task*[n];
	_DecisionN = DecisionN;
}

void DecisionNTask::SetNextTasks(int index, Task* nextTask) {
	tasks[index] = nextTask;
}

void DecisionNTask::TaskProcess(Entity* entity) {

	int decision = _DecisionN(entity);
	SetNextTask(tasks[decision]);
	std::string taskName = tasks[decision]->GetName();
	std::cout << "DecisionN: Sending to task: " << taskName << std::endl;
}

/******************************

		 BATCH ENTITY

*******************************/
Batch::Batch()
{
}
Batch::~Batch() {
	while (_b.GetSize() > 0) {
		delete _b.GetEntity();
	}
}
void Batch::AddEntity(Entity* entity) {
	_b.AddEntity(entity);
}
Entity* Batch::GetEntity() {
	return _b.GetEntity();
}
Entity* Batch::New() {
	return this;
}
int Batch::GetSize() {
	return _b.GetSize();
}
/******************************

		   BATCH TASK

*******************************/
BatchTask::BatchTask(std::string name, int batchSize) : Task(name) {
	_batchSize = batchSize;
	_batch = new Batch();
	_count = 0;
}
void BatchTask::TaskProcess(Entity* entity) {
	_batch->AddEntity(entity);
	_count++;

	if (!(_count < _batchSize)) {
		Depart(_batch);
		_count = 0;
		_batch = new Batch();
	}
}
/******************************

		 UNBATCH TASK

*******************************/
UnBatchTask::UnBatchTask(std::string name) : Task(name) {
}
void UnBatchTask::TaskProcess(Entity* entity) {
	Batch* _batch = (Batch*)entity;
	while (_batch->GetSize() > 0) {
		Depart(_batch->GetEntity());
	}
	delete _batch;
}
/******************************

		RESOURCE POOL

*******************************/
ResourcePool::ResourcePool(std::string name, int numResources) {
	_resources = numResources;
	_name = name;
	_requestQueue = new LinkedList<EventAction>();
}
void ResourcePool::Request(EventAction* ea) {

	cout << "Resource Pool" << _name << " Request" << endl;

	if (_resources > 0) {
		ScheduleEventIn(0.0, ea);
		_resources--;
	}
	else {
		_requestQueue->AddEntity(ea);
	}
}
void ResourcePool::Release() {

	cout << "Resource Pool " << _name << " Release" << endl;

	_resources++;
	if (_requestQueue->GetSize() > 0) {
		ScheduleEventIn(0.0, _requestQueue->GetEntity());
	}
}
std::string ResourcePool::GetName() {
	return _name;
}

/******************************

		 ACQUIRE TASK

*******************************/
class AcquireTask::SendRequestEA : public EventAction{
public:
	SendRequestEA(AcquireTask* aq, Entity* entity) {
		_origin = aq;
		_entity = entity;
	}
	void Execute() {
		_origin->SendRequestEM(_entity);
	}
private:
	AcquireTask* _origin;
	Entity* _entity;
};
class AcquireTask::RequestGrantedEA : public EventAction{
public:
	RequestGrantedEA(AcquireTask* aq, Entity* e) {
		_origin = aq;
		_e = e;
	}
	void Execute() {
		_origin->RequestGrantedEM(_e);
	}
private:
	AcquireTask* _origin;
	Entity* _e;
};
void AcquireTask::SendRequestEM(Entity* e) {
	_rp->Request(new RequestGrantedEA(this, e));
}
void AcquireTask::RequestGrantedEM(Entity* e) {
	Depart(e);
}
AcquireTask::AcquireTask(std::string name, ResourcePool* rp) : Task(name) {
	_rp = rp;
}
void AcquireTask::TaskProcess(Entity* entity) {
	ScheduleEventIn(0.0, new SendRequestEA(this, entity));
}

/******************************

		 RELEASE TASK

*******************************/
ReleaseTask::ReleaseTask(std::string name, ResourcePool* rp) : Task(name) {
	_rp = rp;
}
void ReleaseTask::TaskProcess(Entity* e) {
	_rp->Release();
	Depart(e);
}

ForkTask::ForkTask(string name, int n) : Task(name)
{
	_numNextTasks = n;
	tasks = new Task * [n];
}

void ForkTask::SetNextTasks(int index, Task* nextTask)
{
	tasks[index] = nextTask;
}

void ForkTask::TaskProcess(Entity* entity)
{
	cout << "In fork task process" << endl;
	for (int i = 0; i < _numNextTasks; i++) {
		_nextTask = tasks[i];
		cout << "Fork " << GetName() << " sending to " << _nextTask->GetName() << endl;
		Depart(entity);
	}
}

JoinTask::JoinTask(string name, int n) : Task(name)
{
	_numIncomingQueues = n;

	_incomingQueues = new Set<Entity>[n];

	_previousTaskID = new int[n];
}

void JoinTask::SetPreviousTaskID(int index, int id)
{
	_previousTaskID[index] = id;
}

void JoinTask::TaskProcess(Entity* entity)
{
	bool InAllSets = false;
	int numSetsWithEntity = 0;
	int previousTask = -1;

	// find where its coming from
	for (int i = 0; i < _numIncomingQueues; i++) {
		if (_previousTaskID[i] == entity->GetSource()) {
			previousTask = i;
		}
	}

	// add to the right queue
	_incomingQueues[previousTask].AddEntity(entity);

	// check if it exists in all incoming queues
	for (int i = 0; i < _numIncomingQueues; i++) {
		bool inThisSet = false;
		inThisSet = _incomingQueues[i].HasEntity(entity);

		if (inThisSet) {
			numSetsWithEntity++;
			if (numSetsWithEntity == _numIncomingQueues) {
				InAllSets = true;
			}
		}
	}

	if (InAllSets) {
		Depart(entity);
	}
}
