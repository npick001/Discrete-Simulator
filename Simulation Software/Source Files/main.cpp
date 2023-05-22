#include "SimulationExecutive.h"
#include "Distribution.h"
#include "SpecificNodes.h"
#include "Statistics.h"

void TestBasicSSSQModel() {

	Distribution* arrivalRate = new Triangular(1, 2, 3);
	Distribution* delayTime = new Triangular(100, 200, 300);
	Distribution* serviceTime = new Triangular(5, 6, 7);

	Source* src = new Source("Test1", 1, new MyEntity(GetSimulationTime()), new Triangular(1, 2, 3));
	//ServerQueue* server = new ServerQueue("Test server", new Triangular(5, 6, 7));
	Delay* delay = new Delay("Delay", delayTime);
	Sink* sink = new Sink("Sink");

	src->SetNext(delay);
	//server->SetNext(sink);
	delay->SetNext(sink);
}

int main() {

	/*StatisticsObject statobj(50);
	statobj.StatTesting();*/

	InitializeSimulation();

	TestBasicSSSQModel();

	RunSimulation();

	return 0;
}