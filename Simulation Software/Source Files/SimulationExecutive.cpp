#include <iostream>
#include "SimulationExecutive.h"

using namespace std;

class SimulationExecutive
{
public:
	static void InitializeSimulation();
	/*
	Behavior - Initializes the simulation.  Simulation time is initialized to 0.
	*/

	static Time GetSimulationTime();
	/*
	Behavior - returns the current simulation time.
	*/

	static void RunSimulation();
	/*
	Behavior - Executes the simulation.  It continually selects the event with
	the smallest timestamp for execution until there are no events left to
	execute (selection of simultaneous events not specified).  Prior to
	advancing time, the set of registered conditional events are each evaluated
	and should their condition be met, their event action is executed.
	*/

	static void RunSimulation(Time endTime);
	/*
	Behavior - Identical to the above behavior except that the simulation will
	also terminate if (simulation time > endTime).
	*/

	static void ScheduleEventIn(Time delta, EventAction* ea);
	/*
	Behavior - Scheduled the event action ea to be executed when the simulation
	time equals the current simulation time plus delta.
	*/

	static void ScheduleEventAt(Time time, EventAction* ea);
	/*
	Behavior - Scheduled the event action ea to be executed when the simulation
	time equals time.
	*/

private:
	struct Event;
	class EventSet;
	static EventSet _eventSet;
	static Time _simTime;
};


struct SimulationExecutive::Event
{
	Event(Time time, EventAction *ea)
	{
		_time = time;
		_ea = ea;
		_nextEvent = 0;
	}
	Time _time;
	EventAction *_ea;
	Event *_nextEvent;
};

class SimulationExecutive::EventSet
{
public:
	EventSet()
	{
		_size = 0;
		_heap = new Event * [5000];
	}

	~EventSet() {
		for (int i = 0; i < _size; i++) {
			delete _heap[i];
		}
		delete _heap;
	}

	void AddEvent(Time time, EventAction* ea)
	{
		_heap[_size] = new Event(time, ea);
		_size++;
	}

	Event* GetEvent()
	{
		Event* root = _heap[0];
		_heap[0] = _heap[_size - 1];
		_size--;
		Heapify(0);
		return root;
	}

	Time GetTime()
	{
		return _heap[0]->_time;
	}

	bool HasEvent()
	{
		return (_size > 0);
	}

	void PrintHeap() {
		std::cout << "\n----- PRINTING HEAP. -----" << std::endl;
		for (int index = 0; index < _size; index++) {
			std::cout << "\t- Index " << index << " Time: " << _heap[index]->_time << std::endl;
		}
	}

	void PrintHeapSize() {
		std::cout << "HEAP SIZE: " << _size << std::endl;
	}

	void SetSize(int size) {
		_size = size;
	}

	void SetHeapSize(int heapsize) {

		for (int i = 0; i < _size; i++) {
			delete _heap[i];
		}
		delete _heap;

		_heap = new Event * [heapsize];
	}

private:
	void Heapify(int currNode)
	{
		// lc = 2i+1
		// Rc = 2i+2
		// P = floor[(i-1)/2]

		int i = currNode;
		int lc = 2 * i + 1;
		int rc = 2 * i + 2;

		// check if left child is smaller than current 
		if (lc < _size && _heap[lc]->_time < _heap[i]->_time) {
			i = lc;
		}
		// check if right child is smaller than current 
		if (rc < _size && _heap[rc]->_time < _heap[i]->_time) {
			i = rc;
		}

		if (i != currNode) {
#if SIM_OUTPUT
			std::cout << "SWAPPING " << _heap[i]->_time << " AND " << _heap[currNode]->_time << "\t\t HEAP SIZE: " << _size << std::endl;
#endif // SIM_OUTPUT
			std::swap(_heap[currNode], _heap[i]);

			Heapify(i);
		}
	}

	Event** _heap;
	int _size;
};

SimulationExecutive::EventSet SimulationExecutive::_eventSet;
Time SimulationExecutive::_simTime = 0.0;

void SimulationExecutive::InitializeSimulation()
{
	_simTime = 0.0;
	_eventSet = SimulationExecutive::EventSet();
}

Time SimulationExecutive::GetSimulationTime()
{
	return _simTime;
}

void SimulationExecutive::RunSimulation()
{
	while (_eventSet.HasEvent()) {
		_simTime = _eventSet.GetTime();
		Event *e = _eventSet.GetEvent();
		e->_ea->Execute();
		delete e;
	}
}

void SimulationExecutive::RunSimulation(Time endTime)
{
	while (_eventSet.HasEvent() && _simTime <= endTime) {
		_simTime = _eventSet.GetTime();
		Event* e = _eventSet.GetEvent();
		e->_ea->Execute();
		delete e;
	}
}

void SimulationExecutive::ScheduleEventIn(Time delta, EventAction *ea)
{
	_eventSet.AddEvent(_simTime + delta, ea);
}

void SimulationExecutive::ScheduleEventAt(Time time, EventAction *ea)
{
	_eventSet.AddEvent(time, ea);
}

void InitializeSimulation() {
	SimulationExecutive::InitializeSimulation();
}
Time GetSimulationTime() {
	return SimulationExecutive::GetSimulationTime();
}
void RunSimulation() {
	SimulationExecutive::RunSimulation();
}
void RunSimulation(Time endtime) {
	SimulationExecutive::RunSimulation(endtime);
}
void ScheduleEventIn(Time delta, EventAction* ea) {
	SimulationExecutive::ScheduleEventIn(delta, ea);
}
void ScheduleEventAt(Time time, EventAction* ea) {
	SimulationExecutive::ScheduleEventIn(time, ea);
}