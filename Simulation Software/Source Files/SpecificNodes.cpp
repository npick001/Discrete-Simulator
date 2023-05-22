#include <random>
#include <iostream>
#include "SpecificNodes.h"

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

Source::Source(std::string name, int numGen, Entity* entity, Distribution* dist) : GenericNode(name) {
	_numGen = numGen;
	_dist = dist;
	_entity = entity;
	ScheduleEventIn(0.0, new ArriveEA(this));
};

void Sink::NodeProcess(Entity* entity) {
	cout << "Deleting " << entity->GetID() << endl;
	delete(entity);
}

FIFO<Entity>* ServerQueue::_queue = new FIFO<Entity>;

ServerQueue::ServerQueue(std::string name, Distribution* serviceTime) : GenericNode(name) {
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

void ServerQueue::NodeProcess(Entity* e) {
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

void SystemOfSSSQs::SetNextNodes(GenericNode* next) {

	for (int i = 0; i < _numSSSQs; i++) {
		_sssqs[i]->SetNext(next);
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

void Delay::NodeProcess(Entity* e) {
	ScheduleEventIn(_delay->GetRV(), new DepartEA(this, e));
}

Delay::Delay(std::string name, Distribution* d) : GenericNode(name) {
	_delay = d;
}

/***************************************
	DECISION TASK IMPLEMENTATION
***************************************/

DecisionNode::DecisionNode(std::string name, DecisionFn Decision) : GenericNode(name) {
	_Decision = Decision;
	_trueNode = NULL;
	_falseNode = NULL;
}

void DecisionNode::SetNextNodes(GenericNode* trueTask, GenericNode* falseTask) {
	_trueNode = trueTask;
	_falseNode = falseTask;
}

void DecisionNode::NodeProcess(Entity* entity) {

	bool route = _Decision(entity);

	if (route) {
		SetNext(_trueNode);
		std::cout << "Decision: Sending to task: " << _trueNode->GetName() << std::endl;
	}
	else {
		SetNext(_falseNode);
		std::cout << "Decision: Sending to task: " << _falseNode->GetName() << std::endl;
	}
	Depart(entity);
}

DecisionNNode::DecisionNNode(string name, int n, DecisionNFn DecisionN) : GenericNode(name) {
	_decisionOutcomes = new GenericNode * [n];
	_DecisionN = DecisionN;
}

void DecisionNNode::SetNextNodes(int index, GenericNode* nextTask) {
	_decisionOutcomes[index] = nextTask;
}

void DecisionNNode::NodeProcess(Entity* entity) {

	int decision = _DecisionN(entity);
	SetNext(_decisionOutcomes[decision]);
	wxString taskName = _decisionOutcomes[decision]->GetName();
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
BatchNode::BatchNode(std::string name, int batchSize) : GenericNode(name) {
	_batchSize = batchSize;
	_batch = new Batch();
	_count = 0;
}
void BatchNode::NodeProcess(Entity* entity) {
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
UnBatchNode::UnBatchNode(std::string name) : GenericNode(name) {
}
void UnBatchNode::NodeProcess(Entity* entity) {
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
	_requestQueue = new FIFO<EventAction>();
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
class AcquireNode::SendRequestEA : public EventAction {
public:
	SendRequestEA(AcquireNode* aq, Entity* entity) {
		_origin = aq;
		_entity = entity;
	}
	void Execute() {
		_origin->SendRequestEM(_entity);
	}
private:
	AcquireNode* _origin;
	Entity* _entity;
};
class AcquireNode::RequestGrantedEA : public EventAction {
public:
	RequestGrantedEA(AcquireNode* aq, Entity* e) {
		_origin = aq;
		_e = e;
	}
	void Execute() {
		_origin->RequestGrantedEM(_e);
	}
private:
	AcquireNode* _origin;
	Entity* _e;
};
void AcquireNode::SendRequestEM(Entity* e) {
	_rp->Request(new RequestGrantedEA(this, e));
}
void AcquireNode::RequestGrantedEM(Entity* e) {
	Depart(e);
}
AcquireNode::AcquireNode(std::string name, ResourcePool* rp) : GenericNode(name) {
	_rp = rp;
}
void AcquireNode::NodeProcess(Entity* entity) {
	ScheduleEventIn(0.0, new SendRequestEA(this, entity));
}

/******************************

		 RELEASE TASK

*******************************/
ReleaseNode::ReleaseNode(std::string name, ResourcePool* rp) : GenericNode(name) {
	_rp = rp;
}
void ReleaseNode::NodeProcess(Entity* e) {
	_rp->Release();
	Depart(e);
}


/******************************

		   FORK NODE

*******************************/
ForkNode::ForkNode(string name, int n) : GenericNode(name)
{
	_numNextNodes = n;
	_forkOutcomes = new GenericNode * [n];
}

void ForkNode::SetNextNodes(int index, GenericNode* nextTask)
{
	_forkOutcomes[index] = nextTask;
}

void ForkNode::NodeProcess(Entity* entity)
{
	cout << "In fork task process" << endl;
	for (int i = 0; i < _numNextNodes; i++) {
		m_next = _forkOutcomes[i];
		cout << "Fork " << GetName() << " sending to " << m_next->GetName() << endl;
		Depart(entity);
	}
}

/******************************

		   JOIN NODE

*******************************/
JoinNode::JoinNode(string name, int n) : GenericNode(name)
{
	_numIncomingQueues = n;

	_incomingQueues = new Set<Entity>[n];

	_previousNodeID = new int[n];
}

void JoinNode::SetPreviousNodeID(int index, int id)
{
	_previousNodeID[index] = id;
}

void JoinNode::NodeProcess(Entity* entity)
{
	bool InAllSets = false;
	int numSetsWithEntity = 0;
	int previousTask = -1;

	// find where its coming from
	for (int i = 0; i < _numIncomingQueues; i++) {
		if (_previousNodeID[i] == entity->GetSource()) {
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
