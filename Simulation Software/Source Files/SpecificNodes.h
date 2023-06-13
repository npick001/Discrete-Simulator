#pragma once
#include <iostream>
#include "Distribution.h"
#include "SimulationExecutive.h"
#include "GenericNode.h"
#include "Statistics.h"
#include "FIFO.h"
#include "FIFO_Queue.h"
#include "Set.h"

using namespace std;
class FIFO_Queue;

/***************************************

Naming conventions for this library:

- Member variables begin with m_
- Statistics member variables begin with sm_
- Public member functions are CamelCase

***************************************/


// BEGIN IMPLEMENTED NODES
// NEED TO GO BACK AND CHANGE CODE FROM .h TO .cpp -- 05/23/23 


/****************************************************************/
/* SourceNode:                                                  */
/* defines an entity generating object                          */
/* provides basic entity creation								*/
/* the source object provides statistics					    */
/****************************************************************/
class SourceNode : public GenericNode
{
public:

	void Test() { std::cout << "SOURCE NODE EXISTS" << std::endl; }

	// For finite entity generation
	SourceNode(std::string name, int numGen, Entity* entity, Distribution* dist);

	// For infinite entity generation => NOT COMPLETED
	SourceNode(std::string name, Entity* entity, Distribution* dist);

	// THERE ARE MORE THINGS TO DELETE
	~SourceNode() {
		std::string header = "SOURCE " + std::to_string(GetID()) + "\n";
		m_myStats->Report(header);
	}

	void NodeProcess(Entity* e) override
	{
		std::cout << "ERROR -> SourceNode.Arrive() SHOULD NOT BE CALLED";
	}

	void UpdateStatistics() {
		delete m_myStats;
		m_myStats = new MyStatistics(sm_entitiesCreated, sm_totalEntitiesCreated);
	}

	// inherit the statistics wrapper to populate with 
	class SourceStatistics : public StatisticsWrapper
	{
	public:
		SourceStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
			m_stats = stats;
		}

		~SourceStatistics() { delete m_stats; }

		void ReportStats() override {
			std::string header = "SOURCE " + std::to_string(m_id) + "\n";
			m_stats->Report(header);
		}
	private:
		Statistics* m_stats;
	};

	// Override the getStatistics method
	std::unique_ptr<StatisticsWrapper> GetStatistics() override {
		UpdateStatistics();
		return std::make_unique<SourceStatistics>(m_myStats, GetID());
	}

protected:

	// Get custom statistics for source
	class MyStatistics : public Statistics{

	public:
		MyStatistics() {
			m_sm_entitiesCreated = 0;
			m_sm_totalEntitiesCreated = 0;
		}

		MyStatistics(int entitiesCreated, int totalEntitiesCreated) {
			m_sm_entitiesCreated = entitiesCreated;
			m_sm_totalEntitiesCreated = totalEntitiesCreated;
		}

		// write statistics to a file
		void Report(std::string header) override {

			auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

			// final parameter of write file is 1 for reset, 0 for append to current file
			// Generate the header
			WriteFile(outputFile, header, 0);

			// to_string the data
			std::string data = "\t- Entities Created: " + std::to_string(m_sm_entitiesCreated) +
				"\n\t- Total Entities Created: " + std::to_string(m_sm_totalEntitiesCreated);

			// write data to the file
			WriteFile(outputFile, data, 0);
		}

	private:
		int m_sm_entitiesCreated;
		int m_sm_totalEntitiesCreated;
	};

private:
	// Generation distribution
	Distribution* m_arrivalDistribution;

	// Entity generation members
	Entity* m_entity;
	int m_numberToGenerate;
	bool m_infiniteGeneration;
	
	// Source Events
	class ArriveEA;
	void ArriveEM();

	// Statistics handling
	Statistics* m_myStats;
	int sm_entitiesCreated;
	static int sm_totalEntitiesCreated;
};

/****************************************************************/
/* Sink Node:                                                   */
/* defines an entity deletion object                            */
/* the sink object provides statistics				     	    */
/****************************************************************/
class SinkNode : public GenericNode
{
public:
	
	void Test() { std::cout << "SINK NODE EXISTS" << std::endl; }

	SinkNode(string name) : GenericNode(name) {
		m_nodeType = GenericNode::SINK;
		sm_entitiesDestroyed = 0;
		m_myStats = new MyStatistics(sm_entitiesDestroyed, sm_totalEntitiesDestroyed);
	};

	~SinkNode() {}

	void NodeProcess(Entity* e);

	void UpdateStatistics() {
		delete m_myStats;
		m_myStats = new MyStatistics(sm_entitiesDestroyed, sm_totalEntitiesDestroyed);
	}

	// inherit the statistics wrapper to populate 
	class SinkStatistics : public StatisticsWrapper
	{
	public:
		SinkStatistics(Statistics* stats, int id) : StatisticsWrapper(id) {
			m_stats = stats;
		}
		~SinkStatistics() { delete m_stats; }

		void ReportStats() override {
			std::string header = "SINK " + std::to_string(m_id) + "\n";
			m_stats->Report(header);
		}
	private:
		Statistics* m_stats;
	};

	// Override the GetStatistics method
	std::unique_ptr<StatisticsWrapper> GetStatistics() override {
		UpdateStatistics();
		return std::make_unique<SinkStatistics>(m_myStats, GetID());
	}

protected:

	// Get custom statistics for sink
	class MyStatistics : public Statistics {

	public:
		MyStatistics() {
			m_sm_entitiesDestroyed = 0;
			m_sm_totalEntitiesDestroyed = 0;
		}

		MyStatistics(int entitiesDestroyed, int totalEntitiesDestroyed) {
			m_sm_entitiesDestroyed = entitiesDestroyed;
			m_sm_totalEntitiesDestroyed = totalEntitiesDestroyed;
		}

		// write statistics to a file
		void Report(std::string header) override {

			auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

			// final parameter of write file is 1 for reset, 0 for append to current file
			// Generate the header
			WriteFile(outputFile, header, 0);

			// to_string the data
			std::string data = "\t- Entities Created: " + std::to_string(m_sm_entitiesDestroyed) +
				"\n\t- Total Entities Created: " + std::to_string(m_sm_totalEntitiesDestroyed);

			// write data to the file
			WriteFile(outputFile, data, 0);
		}

	private:
		int m_sm_entitiesDestroyed;
		int m_sm_totalEntitiesDestroyed;
	};
private:

	// Statistics handling
	Statistics* m_myStats;
	int sm_entitiesDestroyed;
	static int sm_totalEntitiesDestroyed;
};
//
///****************************************************************/
///* SSSQ:                                                        */
///* defines a single server single queue object                  */
///* provides basic server functionality							*/
///* the server object provides statistics and queue statistics   */
///****************************************************************/
//class SSSQ : public GenericNode {
//public:
//	SSSQ(std::string name, Distribution* serviceTime);
//	void NodeProcess(Entity* e);
//private:
//
//	class StartProcessingEA;
//	void StartProcessingEM();
//	class EndProcessingEA;
//	void EndProcessingEM(Entity* e);
//
//	enum ServerState { busy, idle };
//	ServerState m_state;
//
//	Distribution* m_serviceTime;
//
//	//static FIFO<Entity>* m_queue;
//	FIFO_Queue* m_queue;
//};
//
///****************************************************************/
///* ServerNQueue:                                                */
///* defines a single server having N resources and M queues.	    */
///* provides advanced server functionality						*/
///* the server object provides statistics and queue statistics   */
///****************************************************************/
//class ServerNQueue : public GenericNode {
//public:
//	ServerNQueue(std::string name, int resources, int queues, Distribution* serviceTime);
//	void NodeProcess(Entity* e);
//
//	//void ReportStatistics();
//private:
//
//	class StartProcessingEA;
//	void StartProcessingEM();
//	class EndProcessingEA;
//	void EndProcessingEM(Entity* e);
//
//	int m_remainingResources;
//
//	Distribution* m_serviceTime;
//
//	//static FIFO<Entity>* m_queue;
//	FIFO_Queue* m_queue;
//};
//
//
///****************************************************************/
///* SystemOfSSSQs:                                               */
///* defines a system consisting of n SSSQs                       */
///* provides functionality to get the reference to a random SSSQ */
///****************************************************************/
//class SystemOfSSSQs
//{
//public:
//	SystemOfSSSQs(int numSSSQs, Distribution* distr);
//	/* initialize the system of SSSQs to have n SSSQs with a service time of distr */
//	SSSQ* GetRandomSSSQ();
//	/* returns a pointer to a random SSSQ */
//
//	void SetNextNodes(GenericNode* next);
//
//private:
//	int m_numSSSQs;
//	SSSQ** m_sssqs;
//};
//
///****************************************************************/
///* DelayNode:                                                   */
///* defines a simple time passing object                         */
///* provides a simple time delay from the delay distribution.	*/
///****************************************************************/
//class DelayNode : public GenericNode {
//public:
//	DelayNode(std::string name, Distribution* d);
//	void NodeProcess(Entity* e);
//private:
//	Distribution* m_delayTime;
//
//	class DepartEA;
//	void DepartEM(Entity* e);
//};
//
//typedef bool(*DecisionFn)(Entity*);
//typedef int(*DecisionNFn)(Entity*);
//
//class DecisionNode : public GenericNode
//{
//public:
//	DecisionNode(std::string name, DecisionFn Decision);
//	/*
//	Behavior:		Constructor, defining the decision function
//	Parameters:		name:		GenericNode name
//					Decision:	function used to evaluate decision
//	Return Value:	none
//	*/
//	void SetNextNodes(GenericNode* trueNode, GenericNode* falseNode);
//	/*
//	Behavior:		Defines the next GenericNodes based on the decision outcome
//	Parameters:		trueGenericNode:	next GenericNode if decision is true
//					falseGenericNode:	next GenericNode if decision is false
//	Return Value:	none
//	*/
//private:
//	void NodeProcess(Entity* entity);
//	/*
//	Behavior:		This GenericNode evaluations the function _Decision( entity).  If
//					true, the next GenericNode is set to _trueGenericNode, else to _falseGenericNode.
//	Parameters:		entity:	the entity the GenericNode is to work on
//	Return Value:	none
//	*/
//	DecisionFn m_decisionFn;			//decision logic function
//	GenericNode* m_trueNode, * m_falseNode;	//GenericNodes entity is sent to based on the Decision function
//};
//
//class DecisionNNode : public GenericNode
//{
//public:
//	DecisionNNode(string name, int n, DecisionNFn DecisionN);
//	/*
//	Behavior:		Constructor, defining the decision function
//	Parameters:		name:		GenericNode name
//					n:			number of decision options
//					Decision:	function used to evaluate decision
//	Return Value:	none
//	*/
//	void SetNextNodes(int index, GenericNode* nextGenericNode);
//	/*
//	Behavior:		Defines the next GenericNode based on the decision outcome
//					indicated by index
//	Parameters:		nextGenericNode:	next GenericNode if decision is indicated by index
//	Return Value:	none
//	*/
//private:
//	void NodeProcess(Entity* entity);
//	/*
//	Behavior:		This GenericNode evaluations the function _DecisionN( entity), which gives
//					an index back to select the next GenericNode from GenericNodes.
//	Parameters:		entity:	the entity the GenericNode is to work on
//	Return Value:	none
//	*/
//	DecisionNFn m_decisionNFn;	//decision function returning 1 of N outcomes
//	GenericNode** m_outcomes;			//array of size N possible decision outcomes
//};
//
///*
//The following class defines a new entity, batch, which is a collection of
//entities grouped together to pass through a set of GenericNodes as one unit.
//*/
//class Batch : public Entity
//{
//public:
//	Batch(Time creationTime);
//	~Batch();
//	void AddEntity(Entity* entity);
//	Entity* GetEntity();
//	Entity* New();
//	int GetSize();
//private:
//	FIFO<Entity> m_entityList;
//};
//
///*
//The Batch GenericNode accepts the arrival of entities, forming them into batches of entities
//(of type Batch) of a fixed size.  Once a batch is complete, it is sent on to the next GenericNode.
//*/
//class BatchNode : public GenericNode
//{
//public:
//	BatchNode(string name, int batchSize);
//private:
//	void NodeProcess(Entity* entity);
//	int m_batchSize;
//	int m_count;
//	Batch* m_batch;
//};
//
///*
//This GenericNode accepts a batch of entities (type Batch), removes each entity from the batch,
//sending them all on to the next GenericNode.  The batch is then disposed of.
//*/
//class UnBatchNode : public GenericNode
//{
//public:
//	UnBatchNode(string name);
//private:
//	void NodeProcess(Entity* entity);
//};
//
//class ResourcePool
//{
//public:
//	ResourcePool(std::string name, int numResources);
//	void Request(EventAction* ea);
//	void Release();
//	std::string GetName();
//private:
//	int m_numResources;
//	std::string m_name;
//	FIFO<EventAction>* m_requestQueue;
//};
//
//class AcquireNode : public GenericNode
//{
//public:
//	AcquireNode(std::string name, ResourcePool* rp);
//private:
//	class SendRequestEA;
//	void SendRequestEM(Entity* e);
//	class RequestGrantedEA;
//	void RequestGrantedEM(Entity* e);
//
//	void NodeProcess(Entity* entity);
//	ResourcePool* m_rpToRequest;
//};
//
//class ReleaseNode : public GenericNode
//{
//public:
//	ReleaseNode(std::string name, ResourcePool* rp);
//private:
//	void NodeProcess(Entity* entity);
//	ResourcePool* m_rpToRelease;
//};
//
//class ForkNode : public GenericNode
//{
//public:
//	ForkNode(string name, int n);
//	void SetNextNodes(int index, GenericNode* nextGenericNode);
//private:
//	void NodeProcess(Entity* entity);
//	int m_numNextNodes;
//	GenericNode** m_forkOutcomes;			//array of size N possible decision outcomes
//};
//
//class JoinNode : public GenericNode
//{
//public:
//	JoinNode(string name, int n);
//	void SetPreviousNodeID(int index, int id);
//private:
//	void NodeProcess(Entity* entity);
//	int m_numIncomingQueues;
//	Set<Entity>* m_incomingQueues;			//array of size N possible incoming queues of entities
//	int* m_previousNodeID;
//};
//
