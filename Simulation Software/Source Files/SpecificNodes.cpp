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

	Depart(m_entity->New());

	if (m_numberToGenerate > 0) {
		Time t = m_arrivalDistribution->GetRV();

		std::cout << "Scheduling source arrival event in " << t << std::endl;
		ScheduleEventIn(t, new ArriveEA(this));
		m_numberToGenerate--;
	}
}

Source::Source(std::string name, int numGen, Entity* entity, Distribution* dist) : GenericNode(name) {
	m_numberToGenerate = numGen;
	m_arrivalDistribution = dist;
	m_entity = entity;
	ScheduleEventIn(0.0, new ArriveEA(this));
};

void Sink::NodeProcess(Entity* entity) {
	cout << "Deleting " << entity->GetID() << endl;
	delete(entity);
}

//FIFO<Entity>* ServerQueue::m_queue = new FIFO<Entity>;

ServerQueue::ServerQueue(std::string name, Distribution* serviceTime) : GenericNode(name) {
	m_state = idle;
	m_serviceTime = serviceTime;
	m_queue = new FIFO_Queue();
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
	m_state = busy;
	Entity* e = m_queue->GetEntity();
	ScheduleEventIn(m_serviceTime->GetRV(), new EndProcessingEA(this, e));
}

void ServerQueue::EndProcessingEM(Entity* e) {
	m_state = idle;
	if (!m_queue->IsEmpty()) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}
	Depart(e);
}

void ServerQueue::NodeProcess(Entity* e) {
	m_queue->AddEntity(e);

	if (m_state == idle) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}
}

SystemOfSSSQs::SystemOfSSSQs(int numSSSQs, Distribution* distr)
{
	m_numSSSQs = numSSSQs;
	m_sssqs = new ServerQueue * [numSSSQs];
	for (int i = 0; i < numSSSQs; i++) {
		m_sssqs[i] = new ServerQueue("Server", distr);
	}
}

ServerQueue* SystemOfSSSQs::GetRandomSSSQ()
{
	static std::default_random_engine generator;
	static std::uniform_int_distribution<> unif_distr(0, m_numSSSQs - 1); // define the range

	int id = unif_distr(generator);
	return m_sssqs[id];
}

void SystemOfSSSQs::SetNextNodes(GenericNode* next) {

	for (int i = 0; i < m_numSSSQs; i++) {
		m_sssqs[i]->SetNext(next);
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
	ScheduleEventIn(m_delayTime->GetRV(), new DepartEA(this, e));
}

Delay::Delay(std::string name, Distribution* d) : GenericNode(name) {
	m_delayTime = d;
}

/***************************************
	DECISION TASK IMPLEMENTATION
***************************************/

DecisionNode::DecisionNode(std::string name, DecisionFn Decision) : GenericNode(name) {
	m_decisionFn = Decision;
	m_trueNode = NULL;
	m_falseNode = NULL;
}

void DecisionNode::SetNextNodes(GenericNode* trueTask, GenericNode* falseTask) {
	m_trueNode = trueTask;
	m_falseNode = falseTask;
}

void DecisionNode::NodeProcess(Entity* entity) {

	bool route = m_decisionFn(entity);

	if (route) {
		SetNext(m_trueNode);
		std::cout << "Decision: Sending to task: " << m_trueNode->GetName() << std::endl;
	}
	else {
		SetNext(m_falseNode);
		std::cout << "Decision: Sending to task: " << m_falseNode->GetName() << std::endl;
	}
	Depart(entity);
}

DecisionNNode::DecisionNNode(string name, int n, DecisionNFn DecisionN) : GenericNode(name) {
	m_outcomes = new GenericNode * [n];
	m_decisionNFn = DecisionN;
}

void DecisionNNode::SetNextNodes(int index, GenericNode* nextTask) {
	m_outcomes[index] = nextTask;
}

void DecisionNNode::NodeProcess(Entity* entity) {

	int decision = m_decisionNFn(entity);
	SetNext(m_outcomes[decision]);
	wxString taskName = m_outcomes[decision]->GetName();
	std::cout << "DecisionN: Sending to task: " << taskName << std::endl;
}

/******************************

		 BATCH ENTITY

*******************************/
Batch::Batch(Time creationTime) : Entity(creationTime)
{
}
Batch::~Batch() {
	while (m_entityList.GetSize() > 0) {
		delete m_entityList.GetEntity();
	}
}
void Batch::AddEntity(Entity* entity) {
	m_entityList.AddEntity(entity);
}
Entity* Batch::GetEntity() {
	return m_entityList.GetEntity();
}
Entity* Batch::New() {
	return (new Batch(GetSimulationTime()));
}
int Batch::GetSize() {
	return m_entityList.GetSize();
}
/******************************

		   BATCH TASK

*******************************/
BatchNode::BatchNode(std::string name, int batchSize) : GenericNode(name) {
	m_batchSize = batchSize;
	m_batch = new Batch(GetSimulationTime());
	m_count = 0;
}
void BatchNode::NodeProcess(Entity* entity) {
	m_batch->AddEntity(entity);
	m_count++;

	if (!(m_count < m_batchSize)) {
		Depart(m_batch);
		m_count = 0;
		m_batch = new Batch(GetSimulationTime());
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
	m_numResources = numResources;
	m_name = name;
	m_requestQueue = new FIFO<EventAction>();
}
void ResourcePool::Request(EventAction* ea) {

	cout << "Resource Pool" << m_name << " Request" << endl;

	if (m_numResources > 0) {
		ScheduleEventIn(0.0, ea);
		m_numResources--;
	}
	else {
		m_requestQueue->AddEntity(ea);
	}
}
void ResourcePool::Release() {

	cout << "Resource Pool " << m_name << " Release" << endl;

	m_numResources++;
	if (m_requestQueue->GetSize() > 0) {
		ScheduleEventIn(0.0, m_requestQueue->GetEntity());
	}
}
std::string ResourcePool::GetName() {
	return m_name;
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
	m_rpToRequest->Request(new RequestGrantedEA(this, e));
}
void AcquireNode::RequestGrantedEM(Entity* e) {
	Depart(e);
}
AcquireNode::AcquireNode(std::string name, ResourcePool* rp) : GenericNode(name) {
	m_rpToRequest = rp;
}
void AcquireNode::NodeProcess(Entity* entity) {
	ScheduleEventIn(0.0, new SendRequestEA(this, entity));
}

/******************************

		 RELEASE TASK

*******************************/
ReleaseNode::ReleaseNode(std::string name, ResourcePool* rp) : GenericNode(name) {
	m_rpToRelease = rp;
}
void ReleaseNode::NodeProcess(Entity* e) {
	m_rpToRelease->Release();
	Depart(e);
}


/******************************

		   FORK NODE

*******************************/
ForkNode::ForkNode(string name, int n) : GenericNode(name)
{
	m_numNextNodes = n;
	m_forkOutcomes = new GenericNode * [n];
}

void ForkNode::SetNextNodes(int index, GenericNode* nextTask)
{
	m_forkOutcomes[index] = nextTask;
}

void ForkNode::NodeProcess(Entity* entity)
{
	cout << "In fork task process" << endl;
	for (int i = 0; i < m_numNextNodes; i++) {
		m_next = m_forkOutcomes[i];
		cout << "Fork " << GetName() << " sending to " << m_next->GetName() << endl;
		Depart(entity);
	}
}

/******************************

		   JOIN NODE

*******************************/
JoinNode::JoinNode(string name, int n) : GenericNode(name)
{
	m_numIncomingQueues = n;

	m_incomingQueues = new Set<Entity>[n];

	m_previousNodeID = new int[n];
}

void JoinNode::SetPreviousNodeID(int index, int id)
{
	m_previousNodeID[index] = id;
}

void JoinNode::NodeProcess(Entity* entity)
{
	bool InAllSets = false;
	int numSetsWithEntity = 0;
	int previousTask = -1;

	// find where its coming from
	for (int i = 0; i < m_numIncomingQueues; i++) {
		if (m_previousNodeID[i] == entity->GetSource()) {
			previousTask = i;
		}
	}

	// add to the right queue
	m_incomingQueues[previousTask].AddEntity(entity);

	// check if it exists in all incoming queues
	for (int i = 0; i < m_numIncomingQueues; i++) {
		bool inThisSet = false;
		inThisSet = m_incomingQueues[i].HasEntity(entity);

		if (inThisSet) {
			numSetsWithEntity++;
			if (numSetsWithEntity == m_numIncomingQueues) {
				InAllSets = true;
			}
		}
	}

	if (InAllSets) {
		Depart(entity);
	}
}
