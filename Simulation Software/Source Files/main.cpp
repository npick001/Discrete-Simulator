#include <vector>

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

	///******************************

	//	DELAY TESTING => WORKING

	//*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delayTime = new Triangular(100, 200, 300);
	//SourceNode* src = new SourceNode("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//DelayNode* delay = new DelayNode("Delay", delayTime);
	//SinkNode* sink = new SinkNode("Sink");
	//src->SetNext(delay);
	//delay->SetNext(sink);

	///******************************

	//	SERVER TESTING => WORKING

	//*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* serviceTime = new Triangular(5, 6, 7);
	//SourceNode* src = new SourceNode("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//SSSQ* server = new SSSQ("Server", serviceTime);
	//SinkNode* sink = new SinkNode("Sink");
	//src->SetNext(server);
	//server->SetNext(sink);

	///******************************

	//DECISION NODE TESTING => WORKING

	//*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delayTime = new Triangular(100, 200, 300);
	//SourceNode* src = new SourceNode("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//DelayNode* delay = new DelayNode("Delay", delayTime);
	//DecisionNode* testDecision = new DecisionNode("Test Decision", (DecisionFn)&Rework);
	//SinkNode* sink = new SinkNode("Sink");
	//src->SetNext(testDecision);
	//testDecision->SetNextNodes(delay, sink);
	//delay->SetNext(sink);

	///******************************

 //  DECISION N NODE TESTING => WORKING

	//*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delayTime = new Triangular(1000, 2000, 3000);
	//Distribution* serviceTime = new Triangular(1, 2, 3);
	//SourceNode* src = new SourceNode("Source", 5, new MyEntity(GetSimulationTime()), arrivalRate);
	//SinkNode* sink = new SinkNode("Sink");
	//DelayNode* delay = new DelayNode("Delay", delayTime);
	//SSSQ* sssq = new SSSQ("Server", serviceTime);
	//DecisionNNode* testDecision = new DecisionNNode("Test Decision", 3, (DecisionNFn)&RandomIndex);
	//src->SetNext(testDecision);
	//delay->SetNext(sink);
	//sssq->SetNext(sink);
	//testDecision->SetNextNodes(0, delay);
	//testDecision->SetNextNodes(1, sssq);
	//testDecision->SetNextNodes(2, sink);

	///******************************

	//	BATCH TESTING => WORKING

	//*******************************/
	//SourceNode* src1 = new SourceNode("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//SourceNode* src2 = new SourceNode("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//SinkNode* batchSink = new SinkNode("Batch Sink");
	//SinkNode* sink = new SinkNode("Entity Sink");
	//BatchNode* batching = new BatchNode("Batch 1", 3);
	//UnBatchNode* unbatching = new UnBatchNode("Unbatch 1");
	//src1->SetNext(batching);
	//batching->SetNext(unbatching);
	//unbatching->SetNext(sink);
	//src2->SetNext(batching);
	//batching->SetNext(batchSink);

	///******************************

	//RESOURCE POOL TESTING => WORKING

	//*******************************/
	//SourceNode* src1 = new SourceNode("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//SourceNode* src2 = new SourceNode("Batching Test", 50, new MyEntity(GetSimulationTime()), new Exponential(0.3));
	//ResourcePool* mechanics = new ResourcePool("Mechanics", 1);
	//AcquireNode* aq1 = new AcquireNode("Test AQ 1", mechanics);
	//AcquireNode* aq2 = new AcquireNode("Test AQ 2", mechanics);
	//DelayNode* delay1 = new DelayNode("Delay 1", new Triangular(1, 2, 3));
	//DelayNode* delay2 = new DelayNode("Delay 2", new Triangular(1, 2, 3));
	//ReleaseNode* r1 = new ReleaseNode("Release AQ 1", mechanics);
	//ReleaseNode* r2 = new ReleaseNode("Release AQ 2", mechanics);
	//SinkNode* sink = new SinkNode("Sink");
	//src1->SetNext(aq1);
	//aq1->SetNext(delay1);
	//delay1->SetNext(r1);
	//r1->SetNext(sink);
	//src2->SetNext(aq2);
	//aq2->SetNext(delay2);
	//delay2->SetNext(r2);
	//r2->SetNext(sink);

	///******************************

	//  FORKJOIN TESTING => WORKING

	//*******************************/
	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* delay = new Triangular(1000, 2000, 3000);

	//SourceNode* src = new SourceNode("Source", 10, new MyEntity(GetSimulationTime()), arrivalRate);
	//ForkNode* fork = new ForkNode("Fork", 2);
	//JoinNode* join = new JoinNode("Join", 2);
	//DelayNode* d1 = new DelayNode("Delay1", delay);
	//DelayNode* d2 = new DelayNode("Delay2", delay);
	//SinkNode* sink = new SinkNode("Sink");
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

	//NodeTesting();

	std::vector<std::unique_ptr<GenericNode::StatisticsWrapper>> stats;

	Distribution* arrivalRate = new Triangular(1, 2, 3);
	Distribution* serviceTime = new Triangular(0.75, 1.5, 2);

	SourceNode* src = new SourceNode("Source", 10, new MyEntity(GetSimulationTime()), arrivalRate);
	SSSQ* server = new SSSQ("SSSQ", serviceTime);
	SSSQ* server1 = new SSSQ("SSSQ", serviceTime);
	SSSQ* server2 = new SSSQ("SSSQ", serviceTime);
	SSSQ* server3 = new SSSQ("SSSQ", serviceTime);
	SSSQ* server4 = new SSSQ("SSSQ", serviceTime);
	SinkNode* sink = new SinkNode("Sink");
	src->SetNext(server);
	server->SetNext(server1);
	server1->SetNext(server2);
	server2->SetNext(server3);
	server3->SetNext(server4);
	server4->SetNext(sink);

	RunSimulation();
	
	std::cout << "\n--------------------------------------------------------\n"
		<< "\t\tSIMULATION IS OVER." << "\n--------------------------------------------------------\n";

	src->Test();
	server->Test();
	sink->Test();

	stats.push_back(src->GetStatistics());
	stats.push_back(sink->GetStatistics());
	stats.push_back(server->GetStatistics());
	stats.push_back(server1->GetStatistics());
	stats.push_back(server2->GetStatistics());
	stats.push_back(server3->GetStatistics());
	stats.push_back(server4->GetStatistics());

	int size = stats.size();
	std::cout << "Stats container size: " << stats.size() << std::endl;

	for (int i = 0; i < size; i++) {

		stats[i].get()->ReportStats();
	
		//std::cout << "(o)(o)" << std::endl;
	}

	return 0;
}