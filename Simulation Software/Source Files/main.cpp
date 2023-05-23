#include "SimulationExecutive.h"
#include "Distribution.h"
//#include "MyTask.h"
#include "SpecificNodes.h"
#include "Statistics.h"

bool Rework()
{
	static Uniform reworkProb(0.0, 1.0);
	return (reworkProb.GetRV() > 0.50);
}
int RandomIndex()
{
	static Triangular nextLocation(0, 1, 2);
	int next = (int)nextLocation.GetRV();
	return (next);
}
void NodeTesting() {

	/******************************

		DELAY TESTING => WORKING

	*******************************/
	//Distribution* delayTime = new Triangular(100, 200, 300);
	//Source* src = new Source("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//Delay* delay = new Delay("Delay", delayTime);
	//Sink* sink = new Sink("Sink");
	//src->SetNext(delay);
	//delay->SetNext(sink);

	/******************************

		SERVER TESTING => WORKING

	*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* serviceTime = new Triangular(5, 6, 7);
	//Source* src = new Source("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//ServerQueue* server = new ServerQueue("Server", serviceTime);
	//Sink* sink = new Sink("Sink");
	//src->SetNext(server);
	//server->SetNext(sink);

	/******************************

	DECISION NODE TESTING => WORKING

	*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delayTime = new Triangular(100, 200, 300);
	//Source* src = new Source("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//Delay* delay = new Delay("Delay", delayTime);
	//DecisionNode* testDecision = new DecisionNode("Test Decision", (DecisionFn)&Rework);
	//Sink* sink = new Sink("Sink");
	//src->SetNext(testDecision);
	//testDecision->SetNextNodes(delay, sink);
	//delay->SetNext(sink);

	/******************************

   DECISION N NODE TESTING => WORKING

	*******************************/
	Distribution* arrivalRate = new Triangular(1, 2, 3);
	Distribution* delayTime = new Triangular(1000, 2000, 3000);
	Distribution* serviceTime = new Triangular(1, 2, 3);
	Source* src = new Source("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	Sink* sink = new Sink("Sink");
	Delay* delay = new Delay("Delay", delayTime);
	ServerQueue* sssq = new ServerQueue("Server", serviceTime);
	DecisionNNode* testDecision = new DecisionNNode("Test Decision", 3, (DecisionNFn)&RandomIndex);
	src->SetNext(testDecision);
	delay->SetNext(sink);
	sssq->SetNext(sink);
	testDecision->SetNextNodes(0, delay);
	testDecision->SetNextNodes(1, sssq);
	testDecision->SetNextNodes(2, sink);

	/******************************

		BATCH TESTING => WORKING

	*******************************/
	//Source* src1 = new Source("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//Source* src2 = new Source("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//Sink* batchSink = new Sink("Batch Sink");
	//Sink* sink = new Sink("Entity Sink");
	//BatchNode* batching = new BatchNode("Batch 1", 3);
	//UnBatchNode* unbatching = new UnBatchNode("Unbatch 1");
	//src1->SetNext(batching);
	//batching->SetNext(unbatching);
	//unbatching->SetNext(sink);
	//src2->SetNext(batching);
	//batching->SetNext(batchSink);

	/******************************

	RESOURCE POOL TESTING => WORKING

	*******************************/
	//Source* src1 = new Source("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//Source* src2 = new Source("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//ResourcePool* mechanics = new ResourcePool("Mechanics", 1);
	//AcquireNode* aq1 = new AcquireNode("Test AQ 1", mechanics);
	//AcquireNode* aq2 = new AcquireNode("Test AQ 2", mechanics);
	//Delay* delay1 = new Delay("Delay 1", new Triangular(1, 2, 3));
	//Delay* delay2 = new Delay("Delay 2", new Triangular(1, 2, 3));
	//ReleaseNode* r1 = new ReleaseNode("Release AQ 1", mechanics);
	//ReleaseNode* r2 = new ReleaseNode("Release AQ 2", mechanics);
	//Sink* sink = new Sink("Sink");
	//src1->SetNext(aq1);
	//aq1->SetNext(delay1);
	//delay1->SetNext(r1);
	//r1->SetNext(sink);
	//src2->SetNext(aq2);
	//aq2->SetNext(delay2);
	//delay2->SetNext(r2);
	//r2->SetNext(sink);

	/******************************

	  FORKJOIN TESTING => WORKING

	*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delay = new Triangular(1000, 2000, 3000);

	//Source* src = new Source("Source", 10, new MyEntity(GetSimulationTime()), arrivalRate);
	//ForkNode* fork = new ForkNode("Fork", 2);
	//JoinNode* join = new JoinNode("Join", 2);
	//Delay* d1 = new Delay("Delay1", delay);
	//Delay* d2 = new Delay("Delay2", delay);
	//Sink* sink = new Sink("Sink");
	//src->SetNext(fork);
	//fork->SetNextNodes(0, d1);
	//fork->SetNextNodes(1, d2);
	//d1->SetNext(join);
	//d2->SetNext(join);
	//join->SetPreviousNodeID(0, d1->GetID());
	//join->SetPreviousNodeID(1, d2->GetID());
	//join->SetNext(sink);
}

int main() {

	/*StatisticsObject statobj(50);
	statobj.StatTesting();*/

	NodeTesting();

	RunSimulation();

	return 0;
}