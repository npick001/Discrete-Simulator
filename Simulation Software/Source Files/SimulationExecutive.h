 #pragma once
#include "Utility.h"

#include <map>
#include <string>

typedef double Time;

enum TimeUnit{
	SECONDS = 999,
	MINUTES,
	HOURS,
	YEARS
};

// MAP FOR TIME UNIT TO STRING CONVERSION
static const std::map<TimeUnit, std::string> TimeToString = { {SECONDS, "Seconds"}, {MINUTES, "Minutes"},
												 {HOURS, "Hours"}, {YEARS, "Years"} };
// MAP FOR STRING TO TIME UNIT CONVERSION
static const std::map<std::string, TimeUnit> StringToTimeUnit = { {"Seconds", SECONDS}, {"Minutes", MINUTES},
												 {"Hours", HOURS}, {"Years", YEARS} };

class EventAction
{
public:
	EventAction() {};
	virtual void Execute() = 0;
};

class Entity;

Time GetSimulationTime();
void RunSimulation();
void RunSimulation(Time endTime);
void ScheduleEventIn(Time delta, EventAction *ea);
void ScheduleEventAt(Time time, EventAction *ea);
void RegisterCreatedEntity(Entity* entity);
