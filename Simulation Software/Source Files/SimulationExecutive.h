#pragma once

typedef double Time;

class EventAction
{
public:
	EventAction() {};
	virtual void Execute() = 0;
};

void InitializeSimulation();
Time GetSimulationTime();
void RunSimulation();
void RunSimulation(Time endTime);
void ScheduleEventIn(Time delta, EventAction *ea);
void ScheduleEventAt(Time time, EventAction *ea);
