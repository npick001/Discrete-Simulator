#include <random>
#include <iostream>
#include "SpecificNodes.h"

int SourceNode::sm_totalEntitiesCreated = 0;
int SinkNode::sm_totalEntitiesDestroyed = 0;


// Get custom statistics for source
class SourceNode::MyStatistics : public Statistics {

public:
	MyStatistics() {
		m_sm_entitiesCreated = 0;
		m_sm_totalEntitiesCreated = 0;
	}

	MyStatistics(int entitiesCreated, int totalEntitiesCreated) {
		m_sm_entitiesCreated = entitiesCreated;
		m_sm_totalEntitiesCreated = totalEntitiesCreated;
	}

	// write statistics to the file
	void Report(std::string header) override {

		auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

		// final parameter of write file is 1 for reset, 0 for append to current file
		// Generate the header
		WriteToFile(outputFile, header, 0);

		// to_string the data
		std::string data = "\t- Entities Created: " + std::to_string(m_sm_entitiesCreated) +
			"\n\t- Total Entities Created: " + std::to_string(m_sm_totalEntitiesCreated) + '\n';

		// write data to the file
		WriteToFile(outputFile, data, 0);
	}

private:
	int m_sm_entitiesCreated;
	int m_sm_totalEntitiesCreated;
};

// inherit the statistics wrapper to populate stats 
class SourceNode::SourceStatistics : public StatisticsWrapper
{
public:
	SourceStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
		m_stats = stats;
	}

	~SourceStatistics() {}

	void ReportStats() override {
		std::string header = "SOURCE " + std::to_string(m_id) + '\n';
		m_stats->Report(header);
	}

	void DeleteStats() override {
		delete m_stats;
	}

private:
	Statistics* m_stats;
};

class SourceNode::ArriveEA : public EventAction {

public:
	ArriveEA(SourceNode* src) { 
		_src = src;
	}

	void Execute() {
		_src->ArriveEM();
	}
private:
	SourceNode* _src;
};

void SourceNode::ArriveEM() {
	
	Depart(m_entity->New());

	if (!m_infiniteGeneration) {
		m_numberToGenerate--;
	}

	sm_entitiesCreated++;
	SourceNode::sm_totalEntitiesCreated++;

	if (m_numberToGenerate > 0 || m_infiniteGeneration) {
		Time arrivalDelta = m_arrivalDistribution->GetRV();

		std::string message = "Scheduling source arrival event in " + std::to_string(arrivalDelta);
		wxLogMessage("%s", message.c_str());
		
		ScheduleEventIn(arrivalDelta, new ArriveEA(this));
	}
}

SourceNode::SourceNode() : GenericNode("Default Source")
{
	SetNodeType(SOURCE);

	m_infiniteGeneration = false;
	m_numberToGenerate = 10;
	m_arrivalDistribution = new Exponential(0.25);
	m_entity = new MyEntity(GetSimulationTime());

	sm_entitiesCreated = 0;
	m_myStats = new MyStatistics(sm_entitiesCreated, sm_totalEntitiesCreated);

	ScheduleEventIn(0.0, new ArriveEA(this));
}

// For finite entity generation
SourceNode::SourceNode(std::string name, int numGen, Entity* entity, Distribution* dist) : GenericNode(name) {

	SetNodeType(SOURCE);

	m_infiniteGeneration = false;
	m_numberToGenerate = numGen;
	m_arrivalDistribution = dist;
	m_entity = entity;

	sm_entitiesCreated = 0;
	m_myStats = new MyStatistics(sm_entitiesCreated, sm_totalEntitiesCreated);

	ScheduleEventIn(0.0, new ArriveEA(this));
};

// For infinite entity generation => NOT COMPLETED
SourceNode::SourceNode(std::string name, Entity* entity, Distribution* dist) : GenericNode(name) {

	SetNodeType(SOURCE);

	m_infiniteGeneration = true;
	m_numberToGenerate = -1;
	m_arrivalDistribution = dist;
	m_entity = entity;

	sm_entitiesCreated = 0;
	m_myStats = new MyStatistics(sm_entitiesCreated, sm_totalEntitiesCreated);

	ScheduleEventIn(0.0, new ArriveEA(this));
}

//SourceNode::~SourceNode() {
//
//	delete m_arrivalDistribution;
//	delete m_entity;
//
//	std::string header = "SOURCE " + std::to_string(GetID()) + "\n";
//	m_myStats->Report(header);
//}

void SourceNode::NodeProcess(Entity* e)
{
	std::string message = "ERROR -> SourceNode.Arrive() SHOULD NOT BE CALLED\n";
	wxLogMessage("%s", message.c_str());
}

// commit changes to statistics
// should be improved by adding a timer to call this,
// investigate more later.
void SourceNode::UpdateStatistics() {
	delete m_myStats;
	m_myStats = new MyStatistics(sm_entitiesCreated, sm_totalEntitiesCreated);
}

// Override the GetStatistics method
std::unique_ptr<GenericNode::StatisticsWrapper> SourceNode::GetStatistics() {
	UpdateStatistics();
	return std::make_unique<SourceStatistics>(m_myStats, GetID());
}

// Get custom statistics for sink
class SinkNode::MyStatistics : public Statistics {

public:
	MyStatistics() {
		m_sm_entitiesDestroyed = 0;
		m_sm_totalEntitiesDestroyed = 0;
	}

	MyStatistics(int entitiesDestroyed, int totalEntitiesDestroyed) {
		m_sm_entitiesDestroyed = entitiesDestroyed;
		m_sm_totalEntitiesDestroyed = totalEntitiesDestroyed;
	}

	// write statistics to the file
	void Report(std::string header) override {

		auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

		// final parameter of write file is 1 for reset, 0 for append to current file
		// Generate the header
		WriteToFile(outputFile, header, 0);

		// to_string the data
		std::string data = "\t- Entities Created: " + std::to_string(m_sm_entitiesDestroyed) +
			"\n\t- Total Entities Created: " + std::to_string(m_sm_totalEntitiesDestroyed) + '\n';

		// write data to the file
		WriteToFile(outputFile, data, 0);
	}

private:
	int m_sm_entitiesDestroyed;
	int m_sm_totalEntitiesDestroyed;
};

class SinkNode::SinkStatistics : public StatisticsWrapper
{
public:
	SinkStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
		m_stats = stats;
	}
	~SinkStatistics() {}

	void ReportStats() override {
		std::string header = "SINK " + std::to_string(m_id) + "\n";
		m_stats->Report(header);
	}

	void DeleteStats() override {
		delete m_stats;
	}

private:
	Statistics* m_stats;
};

SinkNode::SinkNode() : GenericNode("Default Sink")
{
	SetNodeType(SINK);
	sm_entitiesDestroyed = 0;
	m_myStats = new MyStatistics(sm_entitiesDestroyed, sm_totalEntitiesDestroyed);
}

SinkNode::SinkNode(string name) : GenericNode(name) {
	SetNodeType(SINK);
	sm_entitiesDestroyed = 0;
	m_myStats = new MyStatistics(sm_entitiesDestroyed, sm_totalEntitiesDestroyed);
};

void SinkNode::NodeProcess(Entity* entity) {

	std::string message = "Deleting " + std::to_string(entity->GetID()) + '\n';
	wxLogMessage("%s", message.c_str());

	entity->SetDeletionTime(GetSimulationTime());

	sm_entitiesDestroyed++;
	SinkNode::sm_totalEntitiesDestroyed++;

	delete entity;
}

void SinkNode::UpdateStatistics() {
	delete m_myStats;
	m_myStats = new MyStatistics(sm_entitiesDestroyed, sm_totalEntitiesDestroyed);
}

// Override the GetStatistics method
std::unique_ptr<GenericNode::StatisticsWrapper> SinkNode::GetStatistics() {
	UpdateStatistics();
	return std::make_unique<SinkStatistics>(m_myStats, GetID());
}

// statics to be initialized
int SSSQ::sm_totalProcessed = 0;
double SSSQ::sm_totalWaitTime = 0;
double SSSQ::sm_totalIdleTime = 0;

class SSSQ::MyStatistics : public Statistics
{
public:
	MyStatistics() {
		m_sm_processed = 0;
		m_sm_waitTime = 0.0;
		m_sm_totalServiceTime = 0.0;
		m_sm_idleTime = 0.0;
		m_sm_utilization = 0.0;
		m_sm_QavgWaitTime = 0.0;
		m_sm_QavgSize = 0.0;
		m_sm_QminSize = 0.0;
		m_sm_QmazSize = 0.0;
	}

	MyStatistics(int processed, double waitTime, double serviceTime, double idleTime, double util, FIFO* queue) {

		m_sm_processed = processed;
		m_sm_waitTime = waitTime;
		m_sm_totalServiceTime = serviceTime;
		m_sm_idleTime = idleTime;
		m_sm_utilization = util;

		m_sm_QavgWaitTime = queue->GetAverageWaitTime();
		m_sm_QavgSize = queue->GetAverageQueueSize();
		m_sm_QminSize = queue->GetMinimumQueueSize();
		m_sm_QmazSize = queue->GetMaximumQueueSize();
	}

	// write statistics to the file
	void Report(std::string header) override {

		auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

		// final parameter of write file is 1 for reset, 0 for append to current file
		// Generate the header
		WriteToFile(outputFile, header, 0);

		// to_string the data
		std::string data = "\t- Entities Processed: " + std::to_string(m_sm_processed) +
			"\n\t- Time Waiting: " + std::to_string(m_sm_waitTime) + "\n\t- Time in Service: " + std::to_string(m_sm_totalServiceTime) +
			"\n\t- Time Idle: " + std::to_string(m_sm_idleTime) + "\n\t- Percent Utilization: " + std::to_string(m_sm_utilization) + 
			"\n\t- Average Queue Wait Time: " + std::to_string(m_sm_QavgWaitTime) + "\n\t- Average Queue Size: " + std::to_string(m_sm_QavgSize) +
			"\n\t- Minimum Queue Size: " + std::to_string(m_sm_QminSize) + "\n\t- Maximum Queue Size: " + std::to_string(m_sm_QmazSize);

		// write data to the file
		WriteToFile(outputFile, data, 0);
	}

private:

	// Server Statistics
	int m_sm_processed;
	double m_sm_waitTime;
	double m_sm_totalServiceTime;
	double m_sm_idleTime;
	double m_sm_utilization;

	// Queue Statistics
	double m_sm_QavgWaitTime;
	double m_sm_QavgSize;
	double m_sm_QminSize;
	double m_sm_QmazSize;
};

class SSSQ::SSSQStatistics : public StatisticsWrapper
{
public:
	SSSQStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
		m_stats = stats;
	}

	~SSSQStatistics() {}

	void ReportStats() override {
		std::string header = "SSSQ " + std::to_string(m_id) + "\n";
		m_stats->Report(header);
	}

	void DeleteStats() override {
		delete m_stats;
	}

private:
	Statistics* m_stats;
};

class SSSQ::StartProcessingEA : public EventAction {
public:
	StartProcessingEA(SSSQ* s) {
		_s = s;
	}

	void Execute() {
		_s->StartProcessingEM();
	}
private:
	SSSQ* _s;
};

class SSSQ::EndProcessingEA : public EventAction {
public:
	EndProcessingEA(SSSQ* s, Entity* e) {
		_s = s;
		_e = e;
	}

	void Execute() {
		_s->EndProcessingEM(_e);
	}
private:
	SSSQ* _s;
	Entity* _e;
};

SSSQ::SSSQ() : GenericNode("Default Server")
{
	SetNodeType(SERVER);

	m_state = idle;
	sm_states.push_back(1.0);
	sm_stateChangeTimes.push_back(GetSimulationTime());

	m_serviceTime = new Triangular(1.0, 2.0, 3.0);
	m_queue = new FIFO();

	m_myStats = new MyStatistics();
	sm_processed = 0;
	sm_waitTime = 0;
	sm_totalServiceTime = 0;
	sm_idleTime = 0;
	sm_utilization = 0;
}

SSSQ::SSSQ(std::string name, Distribution* serviceTime) : GenericNode(name) {
	SetNodeType(SERVER);

	m_state = idle;
	sm_states.push_back(1.0);
	sm_stateChangeTimes.push_back(GetSimulationTime());

	m_serviceTime = serviceTime;
	m_queue = new FIFO();

	m_myStats = new MyStatistics();
	sm_processed = 0;
	sm_waitTime = 0;
	sm_totalServiceTime = 0;
	sm_idleTime = 0;
	sm_utilization = 0;
}

SSSQ::~SSSQ()
{
	sm_states.clear();
	sm_stateChangeTimes.clear();
	delete m_serviceTime;
	delete m_queue;

	//&SSSQStatistics::DeleteStats;
}

void SSSQ::StartProcessingEM() {
	m_state = busy;
	Entity* e = m_queue->GetEntity();

	sm_states.push_back(0.0);
	sm_stateChangeTimes.push_back(GetSimulationTime());
	
	sm_totalWaitTime +=	e->GetWaitTime();

	Time serviceTime = m_serviceTime->GetRV();
	sm_totalServiceTime += serviceTime;

	std::string message = "Time: " + std::to_string(GetSimulationTime()) +
						  "\tSSSQ " + std::to_string(GetID()) +
						  "\tStart Processing\n";
	wxLogMessage("%s", message.c_str());

	ScheduleEventIn(serviceTime, new EndProcessingEA(this, e));
}

void SSSQ::EndProcessingEM(Entity* e) {
	m_state = idle;

	sm_states.push_back(1.0);
	sm_stateChangeTimes.push_back(GetSimulationTime());

	sm_processed++;
	SSSQ::sm_totalProcessed++;

	if (!m_queue->IsEmpty()) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}

	std::string message = "Time: " + std::to_string(GetSimulationTime()) +
		"\tSSSQ " + std::to_string(GetID()) +
		"\tEnd Processing\n";

	Depart(e);
}

void SSSQ::NodeProcess(Entity* e) {
	m_queue->AddEntity(e);

	if (m_state == idle) {
		ScheduleEventIn(0.0, new StartProcessingEA(this));
	}
}

void SSSQ::UpdateStatistics()
{
	double timeUtilized = 0;

	int numStates = sm_states.size();
	int numTimes = sm_stateChangeTimes.size();

	// numerator
	for (int i = 1; i < numTimes; i++) {
		timeUtilized += sm_states[i] * (sm_stateChangeTimes[i] - sm_stateChangeTimes[i - 1]);
	}

	sm_utilization = timeUtilized / GetSimulationTime();
	sm_idleTime = GetSimulationTime() - timeUtilized;

	delete m_myStats;
	m_myStats = new MyStatistics(sm_processed, sm_totalWaitTime, sm_totalServiceTime, sm_idleTime, sm_utilization, m_queue);
}

std::unique_ptr<GenericNode::StatisticsWrapper> SSSQ::GetStatistics() {
	UpdateStatistics();
	return std::make_unique<SSSQStatistics>(m_myStats, GetID());
}


// statics to be initialized
//int SSSQ::sm_totalProcessed = 0;
//double SSSQ::sm_totalWaitTime = 0;
//double SSSQ::sm_totalIdleTime = 0;

class ServerNQueue::MyStatistics : public Statistics
{
public:
	MyStatistics() {
		m_sm_processed = 0.0;
		m_sm_waitTime = 0.0;
		m_sm_totalServiceTime = 0.0;
		m_sm_idleTime = 0.0;
		m_sm_utilization = 0.0;
		m_sm_QavgWaitTime = 0.0;
		m_sm_QavgSize = 0.0;
		m_sm_QminSize = 0.0;
		m_sm_QmazSize = 0.0;
	}

	MyStatistics(int processed, double waitTime, double serviceTime, double idleTime, double util, FIFO* queue) {

		m_sm_processed = processed;
		m_sm_waitTime = waitTime;
		m_sm_totalServiceTime = serviceTime;
		m_sm_idleTime = idleTime;
		m_sm_utilization = util;

		m_sm_QavgWaitTime = queue->GetAverageWaitTime();
		m_sm_QavgSize = queue->GetAverageQueueSize();
		m_sm_QminSize = queue->GetMinimumQueueSize();
		m_sm_QmazSize = queue->GetMaximumQueueSize();
	}

	// write statistics to the file
	void Report(std::string header) override {

		auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

		// final parameter of write file is 1 for reset, 0 for append to current file
		// Generate the header
		WriteToFile(outputFile, header, 0);

		// to_string the data
		std::string data = "\t- Entities Processed: " + std::to_string(m_sm_processed) +
			"\n\t- Time Waiting: " + std::to_string(m_sm_waitTime) + "\n\t- Time in Service: " + std::to_string(m_sm_totalServiceTime) +
			"\n\t- Time Idle: " + std::to_string(m_sm_idleTime) + "\n\t- Percent Utilization: " + std::to_string(m_sm_utilization) +
			"\n\t- Average Queue Wait Time: " + std::to_string(m_sm_QavgWaitTime) + "\n\t- Average Queue Size: " + std::to_string(m_sm_QavgSize) +
			"\n\t- Minimum Queue Size: " + std::to_string(m_sm_QminSize) + "\n\t- Maximum Queue Size: " + std::to_string(m_sm_QmazSize);

		// write data to the file
		WriteToFile(outputFile, data, 0);
	}

private:

	int m_sm_processed;
	double m_sm_waitTime;
	double m_sm_totalServiceTime;
	double m_sm_idleTime;
	double m_sm_utilization;

	double m_sm_QavgWaitTime;
	double m_sm_QavgSize;
	double m_sm_QminSize;
	double m_sm_QmazSize;
};

class ServerNQueue::ServerNQueueStatistics : public StatisticsWrapper
{
public:
	ServerNQueueStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
		m_stats = stats;
	}

	~ServerNQueueStatistics() {}

	void ReportStats() override {
		std::string header = "ServerNQueue: " + std::to_string(m_id) + "\n";
		m_stats->Report(header);
	}

	void DeleteStats() override {
		delete m_stats;
	}

private:
	Statistics* m_stats;
};


class ServerNQueue::StartProcessingEA : public EventAction {
public:
	StartProcessingEA(ServerNQueue* s) {
		_s = s;
	}

	void Execute() {
		_s->StartProcessingEM();
	}
private:
	ServerNQueue* _s;
};

class ServerNQueue::EndProcessingEA : public EventAction {
public:
	EndProcessingEA(ServerNQueue* s, Entity* e) {
		_s = s;
		_e = e;
	}

	void Execute() {
		_s->EndProcessingEM(_e);
	}
private:
	ServerNQueue* _s;
	Entity* _e;
};


ServerNQueue::ServerNQueue(std::string name, int resources, int queues, Distribution* serviceTime)
	: GenericNode(name)
{
	m_remainingResources = resources;
	m_maxResources = resources;
	
	for (int i = 0; i < queues; i++) {
		m_queues.push_back(new FIFO());
	}

	m_serviceTime = serviceTime;

	m_myStats = new MyStatistics();
	sm_processed = 0;
	sm_waitTime = 0;
	sm_totalServiceTime = 0;
	sm_idleTime = 0;
	sm_utilization = 0;
}

ServerNQueue::~ServerNQueue()
{
	delete m_serviceTime;

	&ServerNQueueStatistics::DeleteStats;
}

void ServerNQueue::NodeProcess(Entity* e)
{
}

void ServerNQueue::UpdateStatistics()
{
}

std::unique_ptr<GenericNode::StatisticsWrapper> ServerNQueue::GetStatistics()
{
	return std::unique_ptr<StatisticsWrapper>();
}

void ServerNQueue::StartProcessingEM()
{
}

void ServerNQueue::EndProcessingEM(Entity* e)
{
}

//SystemOfSSSQs::SystemOfSSSQs(int numSSSQs, Distribution* distr)
//{
//	m_numSSSQs = numSSSQs;
//	m_sssqs = new SSSQ * [numSSSQs];
//	for (int i = 0; i < numSSSQs; i++) {
//		m_sssqs[i] = new SSSQ("Server", distr);
//	}
//}
//
//SSSQ* SystemOfSSSQs::GetRandomSSSQ()
//{
//	static std::default_random_engine generator;
//	static std::uniform_int_distribution<> unif_distr(0, m_numSSSQs - 1); // define the range
//
//	int id = unif_distr(generator);
//	return m_sssqs[id];
//}
//
//void SystemOfSSSQs::SetNextNodes(GenericNode* next) {
//
//	for (int i = 0; i < m_numSSSQs; i++) {
//		m_sssqs[i]->SetNext(next);
//	}
//
//}
//
//class DelayNode::DepartEA : public EventAction {
//
//public:
//	DepartEA(DelayNode* d, Entity* e) {
//		_e = e;
//		_d = d;
//	}
//
//	void Execute() {
//		_d->DepartEM(_e);
//	}
//
//private:
//	Entity* _e;
//	DelayNode* _d;
//};
//
//void DelayNode::DepartEM(Entity* e) {
//	Depart(e);
//}
//
//void DelayNode::NodeProcess(Entity* e) {
//	ScheduleEventIn(m_delayTime->GetRV(), new DepartEA(this, e));
//}
//
//DelayNode::DelayNode(std::string name, Distribution* d) : GenericNode(name) {
//	m_delayTime = d;
//}
//
///***************************************
//	DECISION TASK IMPLEMENTATION
//***************************************/
//
//DecisionNode::DecisionNode(std::string name, DecisionFn Decision) : GenericNode(name) {
//	m_decisionFn = Decision;
//	m_trueNode = NULL;
//	m_falseNode = NULL;
//}
//
//void DecisionNode::SetNextNodes(GenericNode* trueTask, GenericNode* falseTask) {
//	m_trueNode = trueTask;
//	m_falseNode = falseTask;
//}
//
//void DecisionNode::NodeProcess(Entity* entity) {
//
//	bool route = m_decisionFn(entity);
//
//	if (route) {
//		SetNext(m_trueNode);
//		std::cout << "Decision: Sending to task: " << m_trueNode->GetName() << std::endl;
//	}
//	else {
//		SetNext(m_falseNode);
//		std::cout << "Decision: Sending to task: " << m_falseNode->GetName() << std::endl;
//	}
//	Depart(entity);
//}
//
//DecisionNNode::DecisionNNode(string name, int n, DecisionNFn DecisionN) : GenericNode(name) {
//	m_outcomes = new GenericNode * [n];
//	m_decisionNFn = DecisionN;
//}
//
//void DecisionNNode::SetNextNodes(int index, GenericNode* nextTask) {
//	m_outcomes[index] = nextTask;
//}
//
//void DecisionNNode::NodeProcess(Entity* entity) {
//
//	int decision = m_decisionNFn(entity);
//	SetNext(m_outcomes[decision]);
//	wxString taskName = m_outcomes[decision]->GetName();
//	std::cout << "DecisionN: Sending to task: " << taskName << std::endl;
//}
//
///******************************
//
//		 BATCH ENTITY
//
//*******************************/
//Batch::Batch(Time creationTime) : Entity(creationTime)
//{
//}
//Batch::~Batch() {
//	while (m_entityList.GetSize() > 0) {
//		delete m_entityList.GetEntity();
//	}
//}
//void Batch::AddEntity(Entity* entity) {
//	m_entityList.AddEntity(entity);
//}
//Entity* Batch::GetEntity() {
//	return m_entityList.GetEntity();
//}
//Entity* Batch::New() {
//	return (new Batch(GetSimulationTime()));
//}
//int Batch::GetSize() {
//	return m_entityList.GetSize();
//}
///******************************
//
//		   BATCH TASK
//
//*******************************/
//BatchNode::BatchNode(std::string name, int batchSize) : GenericNode(name) {
//	m_batchSize = batchSize;
//	m_batch = new Batch(GetSimulationTime());
//	m_count = 0;
//}
//void BatchNode::NodeProcess(Entity* entity) {
//	m_batch->AddEntity(entity);
//	m_count++;
//
//	if (!(m_count < m_batchSize)) {
//		Depart(m_batch);
//		m_count = 0;
//		m_batch = new Batch(GetSimulationTime());
//	}
//}
///******************************
//
//		 UNBATCH TASK
//
//*******************************/
//UnBatchNode::UnBatchNode(std::string name) : GenericNode(name) {
//}
//void UnBatchNode::NodeProcess(Entity* entity) {
//	Batch* _batch = (Batch*)entity;
//	while (_batch->GetSize() > 0) {
//		Depart(_batch->GetEntity());
//	}
//	delete _batch;
//}
///******************************
//
//		RESOURCE POOL
//
//*******************************/
//ResourcePool::ResourcePool(std::string name, int numResources) {
//	m_numResources = numResources;
//	m_name = name;
//	m_requestQueue = new FIFO<EventAction>();
//}
//void ResourcePool::Request(EventAction* ea) {
//
//	cout << "Resource Pool" << m_name << " Request" << endl;
//
//	if (m_numResources > 0) {
//		ScheduleEventIn(0.0, ea);
//		m_numResources--;
//	}
//	else {
//		m_requestQueue->AddEntity(ea);
//	}
//}
//void ResourcePool::Release() {
//
//	cout << "Resource Pool " << m_name << " Release" << endl;
//
//	m_numResources++;
//	if (m_requestQueue->GetSize() > 0) {
//		ScheduleEventIn(0.0, m_requestQueue->GetEntity());
//	}
//}
//std::string ResourcePool::GetName() {
//	return m_name;
//}
//
///******************************
//
//		 ACQUIRE TASK
//
//*******************************/
//class AcquireNode::SendRequestEA : public EventAction {
//public:
//	SendRequestEA(AcquireNode* aq, Entity* entity) {
//		_origin = aq;
//		_entity = entity;
//	}
//	void Execute() {
//		_origin->SendRequestEM(_entity);
//	}
//private:
//	AcquireNode* _origin;
//	Entity* _entity;
//};
//class AcquireNode::RequestGrantedEA : public EventAction {
//public:
//	RequestGrantedEA(AcquireNode* aq, Entity* e) {
//		_origin = aq;
//		_e = e;
//	}
//	void Execute() {
//		_origin->RequestGrantedEM(_e);
//	}
//private:
//	AcquireNode* _origin;
//	Entity* _e;
//};
//void AcquireNode::SendRequestEM(Entity* e) {
//	m_rpToRequest->Request(new RequestGrantedEA(this, e));
//}
//void AcquireNode::RequestGrantedEM(Entity* e) {
//	Depart(e);
//}
//AcquireNode::AcquireNode(std::string name, ResourcePool* rp) : GenericNode(name) {
//	m_rpToRequest = rp;
//}
//void AcquireNode::NodeProcess(Entity* entity) {
//	ScheduleEventIn(0.0, new SendRequestEA(this, entity));
//}
//
///******************************
//
//		 RELEASE TASK
//
//*******************************/
//ReleaseNode::ReleaseNode(std::string name, ResourcePool* rp) : GenericNode(name) {
//	m_rpToRelease = rp;
//}
//void ReleaseNode::NodeProcess(Entity* e) {
//	m_rpToRelease->Release();
//	Depart(e);
//}
//
//
///******************************
//
//		   FORK NODE
//
//*******************************/
//ForkNode::ForkNode(string name, int n) : GenericNode(name)
//{
//	m_numNextNodes = n;
//	m_forkOutcomes = new GenericNode * [n];
//}
//
//void ForkNode::SetNextNodes(int index, GenericNode* nextTask)
//{
//	m_forkOutcomes[index] = nextTask;
//}
//
//void ForkNode::NodeProcess(Entity* entity)
//{
//	cout << "In fork task process" << endl;
//	for (int i = 0; i < m_numNextNodes; i++) {
//		m_next = m_forkOutcomes[i];
//		cout << "Fork " << GetName() << " sending to " << m_next->GetName() << endl;
//		Depart(entity);
//	}
//}
//
///******************************
//
//		   JOIN NODE
//
//*******************************/
//JoinNode::JoinNode(string name, int n) : GenericNode(name)
//{
//	m_numIncomingQueues = n;
//
//	m_incomingQueues = new Set<Entity>[n];
//
//	m_previousNodeID = new int[n];
//}
//
//void JoinNode::SetPreviousNodeID(int index, int id)
//{
//	m_previousNodeID[index] = id;
//}
//
//void JoinNode::NodeProcess(Entity* entity)
//{
//	bool InAllSets = false;
//	int numSetsWithEntity = 0;
//	int previousTask = -1;
//
//	// find where its coming from
//	for (int i = 0; i < m_numIncomingQueues; i++) {
//		if (m_previousNodeID[i] == entity->GetSource()) {
//			previousTask = i;
//		}
//	}
//
//	// add to the right queue
//	m_incomingQueues[previousTask].AddEntity(entity);
//
//	// check if it exists in all incoming queues
//	for (int i = 0; i < m_numIncomingQueues; i++) {
//		bool inThisSet = false;
//		inThisSet = m_incomingQueues[i].HasEntity(entity);
//
//		if (inThisSet) {
//			numSetsWithEntity++;
//			if (numSetsWithEntity == m_numIncomingQueues) {
//				InAllSets = true;
//			}
//		}
//	}
//
//	if (InAllSets) {
//		Depart(entity);
//	}
//}

//SourceNode::MyStatistics::MyStatistics(int entitiesCreated, int totalEntitiesCreated)
//{
//	m_sm_entitiesCreated = entitiesCreated;
//	m_sm_totalEntitiesCreated = totalEntitiesCreated;
//}
