#include <stdlib.h>
#include <crtdbg.h>
#include <cstdlib>
#include <vector>

#include "stats.hpp"
#include "SimulationExecutive.h"
#include "Distribution.h"
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

	StatisticsObject statobj(10);
	//statobj.StatTesting();
	//statobj.GammaFunction(1);

	/*Exponential* exponential = new Exponential(3);
	std::cout << "\nEXPONENTIAL DISTRIBUTION (5)\n";*/

	//Normal* norm = new Normal(5, 1);
	//std::cout << "\nNORMAL DISTRIBUTION (5, 1)\n";

	/*statobj.CreateDummyData(2500, exponential);
	Distribution* bestfit = statobj.GenerateBestFitDistribution(".\\Output Files\\MLEDistributionData.txt");*/

	std::vector<double> data = {28.0, 18.0, 19.0, 25.0, 26.0,
								16.0, 21.0, 16.0, 18.0, 13.0};

	std::vector<double> probs = {20.0, 20.0, 20.0, 20.0, 20.0,
								 20.0, 20.0, 20.0, 20.0, 20.0};


	double critApprox = statobj.ChiSquareTest(data, probs);


	//std::vector<double> x = { 1.8, 0.7, 4.2 };
	//stats::pchisq(x, 4, false);


	//std::vector<double> RKpoints = statobj.RungeKutta4(&PDF_Chi_Square, 0.001, 10.0, (10.0 / 0.1));
	//std::vector<double> CS_Dist = statobj.Chi_Square_Distribution(10, 1000);
	system("Pause");
}

	//NodeTesting();

	//std::vector<std::unique_ptr<GenericNode::StatisticsWrapper>> stats;

	//srand(time(NULL));

	//Distribution* arrivalRate = new Triangular(1, 2, 3);
	//Distribution* serviceTime = new Triangular(0.75, 1.5, 2);

	//SourceNode* src = new SourceNode("Source", 100, new MyEntity(GetSimulationTime()), arrivalRate);
	//SSSQ* server = new SSSQ("SSSQ", serviceTime);
	////SSSQ* server1 = new SSSQ("SSSQ", serviceTime);
	////SSSQ* server2 = new SSSQ("SSSQ", serviceTime);
	////SSSQ* server3 = new SSSQ("SSSQ", serviceTime);
	////SSSQ* server4 = new SSSQ("SSSQ", serviceTime);
	//SinkNode* sink = new SinkNode("Sink");
	////src->SetNext(server);
	////server->SetNext(server1);
	////server1->SetNext(server2);
	////server2->SetNext(server3);
	////server3->SetNext(server4);
	////server4->SetNext(sink);

	//src->SetNext(server);
	//server->SetNext(sink);

	//RunSimulation();
	//
	//std::cout << "\n--------------------------------------------------------\n"
	//	<< "\t\tSIMULATION IS OVER." << "\n--------------------------------------------------------\n";

	//src->Test();
	//server->Test();
	//sink->Test();

	//stats.push_back(src->GetStatistics());
	//stats.push_back(sink->GetStatistics());
	//stats.push_back(server->GetStatistics());
	///*stats.push_back(server1->GetStatistics());
	//stats.push_back(server2->GetStatistics());
	//stats.push_back(server3->GetStatistics());
	//stats.push_back(server4->GetStatistics());*/

	//int size = stats.size();
	//std::cout << "Stats container size: " << stats.size() << std::endl;

	//// Which file to reset and write to
	//auto outputFile = ".\\Output Files\\SimObjStatistics.txt";

	//// final parameter of write file is 1 for reset, 0 for append to current file
	//// Reset the file data before new stats are written
	//WriteFile(outputFile, "Simulation Statistics\n", 1);
	//WriteFile(outputFile, "Simulation End Time: " + std::to_string(GetSimulationTime()) + '\n', 0);

	//for (int i = 0; i < size; i++) {

	//	stats[i].get()->ReportStats();
	//	//stats[i].get()->DeleteStats();
	//	//std::cout << "(o)(o)" << std::endl;
	//}

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();

	////delete arrivalRate;
	////delete serviceTime;
	//delete src;
	//delete server;
	//delete sink;

//	system("Pause");
//}