#pragma once
#include <iostream>
#include "Distribution.h"
#include "SimulationExecutive.h"
#include "FIFO.h"
#include "FIFO_Queue.h"
#include "Set.h"
#include "GenericNode.h"

using namespace std;
class FIFO_Queue;
class Source : public GenericNode
{
public:
	Source(std::string name, int numGen, Entity* entity, Distribution* dist);

	void NodeProcess(Entity* e) override
	{
		std::cout << "ERROR -> Source.Arrive() SHOULD NOT BE CALLED";
	}

private:
	Distribution* m_arrivalDistribution;
	int m_numberToGenerate;
	Entity* m_entity;
	class ArriveEA;
	void ArriveEM();
};

class ServerQueue : public GenericNode {
public:
	ServerQueue(std::string name, Distribution* serviceTime);
	void NodeProcess(Entity* e);
private:

	class StartProcessingEA;
	void StartProcessingEM();
	class EndProcessingEA;
	void EndProcessingEM(Entity* e);

	enum ServerState { busy, idle };
	ServerState m_state;

	Distribution* m_serviceTime;

	//static FIFO<Entity>* m_queue;
	FIFO_Queue* m_queue;
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

	void SetNextNodes(GenericNode* next);

private:
	int m_numSSSQs;
	ServerQueue** m_sssqs;
};

class Delay : public GenericNode {
public:
	Delay(std::string name, Distribution* d);
	void NodeProcess(Entity* e);
private:
	Distribution* m_delayTime;

	class DepartEA;
	void DepartEM(Entity* e);
};

class Sink : public GenericNode
{
public:
	Sink(string name) : GenericNode(name) {};

	void NodeProcess(Entity* e);
};

typedef bool(*DecisionFn)(Entity*);
typedef int(*DecisionNFn)(Entity*);

class DecisionNode : public GenericNode
{
public:
	DecisionNode(std::string name, DecisionFn Decision);
	/*
	Behavior:		Constructor, defining the decision function
	Parameters:		name:		GenericNode name
					Decision:	function used to evaluate decision
	Return Value:	none
	*/
	void SetNextNodes(GenericNode* trueGenericNode, GenericNode* falseGenericNode);
	/*
	Behavior:		Defines the next GenericNodes based on the decision outcome
	Parameters:		trueGenericNode:	next GenericNode if decision is true
					falseGenericNode:	next GenericNode if decision is false
	Return Value:	none
	*/
private:
	void NodeProcess(Entity* entity);
	/*
	Behavior:		This GenericNode evaluations the function _Decision( entity).  If
					true, the next GenericNode is set to _trueGenericNode, else to _falseGenericNode.
	Parameters:		entity:	the entity the GenericNode is to work on
	Return Value:	none
	*/
	DecisionFn m_decisionFn;			//decision logic function
	GenericNode* m_trueNode, * m_falseNode;	//GenericNodes entity is sent to based on the Decision function
};

class DecisionNNode : public GenericNode
{
public:
	DecisionNNode(string name, int n, DecisionNFn DecisionN);
	/*
	Behavior:		Constructor, defining the decision function
	Parameters:		name:		GenericNode name
					n:			number of decision options
					Decision:	function used to evaluate decision
	Return Value:	none
	*/
	void SetNextNodes(int index, GenericNode* nextGenericNode);
	/*
	Behavior:		Defines the next GenericNode based on the decision outcome
					indicated by index
	Parameters:		nextGenericNode:	next GenericNode if decision is indicated by index
	Return Value:	none
	*/
private:
	void NodeProcess(Entity* entity);
	/*
	Behavior:		This GenericNode evaluations the function _DecisionN( entity), which gives
					an index back to select the next GenericNode from GenericNodes.
	Parameters:		entity:	the entity the GenericNode is to work on
	Return Value:	none
	*/
	DecisionNFn m_decisionNFn;	//decision function returning 1 of N outcomes
	GenericNode** m_outcomes;			//array of size N possible decision outcomes
};

/*
The following class defines a new entity, batch, which is a collection of
entities grouped together to pass through a set of GenericNodes as one unit.
*/
class Batch : public Entity
{
public:
	Batch(Time creationTime);
	~Batch();
	void AddEntity(Entity* entity);
	Entity* GetEntity();
	Entity* New();
	int GetSize();
private:
	FIFO<Entity> m_entityList;
};

/*
The Batch GenericNode accepts the arrival of entities, forming them into batches of entities
(of type Batch) of a fixed size.  Once a batch is complete, it is sent on to the next GenericNode.
*/
class BatchNode : public GenericNode
{
public:
	BatchNode(string name, int batchSize);
private:
	void NodeProcess(Entity* entity);
	int m_batchSize;
	int m_count;
	Batch* m_batch;
};

/*
This GenericNode accepts a batch of entities (type Batch), removes each entity from the batch,
sending them all on to the next GenericNode.  The batch is then disposed of.
*/
class UnBatchNode : public GenericNode
{
public:
	UnBatchNode(string name);
private:
	void NodeProcess(Entity* entity);
};

class ResourcePool
{
public:
	ResourcePool(std::string name, int numResources);
	void Request(EventAction* ea);
	void Release();
	std::string GetName();
private:
	int m_numResources;
	std::string m_name;
	FIFO<EventAction>* m_requestQueue;
};

class AcquireNode : public GenericNode
{
public:
	AcquireNode(std::string name, ResourcePool* rp);
private:
	class SendRequestEA;
	void SendRequestEM(Entity* e);
	class RequestGrantedEA;
	void RequestGrantedEM(Entity* e);

	void NodeProcess(Entity* entity);
	ResourcePool* m_rpToRequest;
};

class ReleaseNode : public GenericNode
{
public:
	ReleaseNode(std::string name, ResourcePool* rp);
private:
	void NodeProcess(Entity* entity);
	ResourcePool* m_rpToRelease;
};

class ForkNode : public GenericNode
{
public:
	ForkNode(string name, int n);
	void SetNextNodes(int index, GenericNode* nextGenericNode);
private:
	void NodeProcess(Entity* entity);
	int m_numNextNodes;
	GenericNode** m_forkOutcomes;			//array of size N possible decision outcomes
};

class JoinNode : public GenericNode
{
public:
	JoinNode(string name, int n);
	void SetPreviousNodeID(int index, int id);
private:
	void NodeProcess(Entity* entity);
	int m_numIncomingQueues;
	Set<Entity>* m_incomingQueues;			//array of size N possible incoming queues of entities
	int* m_previousNodeID;
};

