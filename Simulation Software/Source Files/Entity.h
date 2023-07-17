#pragma once
#include "Utility.h"
#include "SimulationExecutive.h"
#include "FIFO.h"

enum EntityType { entity, transport, batch };

class Entity
{
public:
	Entity(Time creationTime);
	~Entity() {};
	virtual Entity *New() = 0;

	// Getters
	int GetID();
	int GetSource();
	EntityType GetType();

	// Setters
	void SetSource(int id);
	void SetType(EntityType myType);
	void SetDeletionTime(Time timeNow);
	
	// Statistics time stamping functions
	void EnterQueue(Time timeNow);
	Time LeaveQueue(Time timeNow);

	// Statistics reporting functions
	inline Time GetTotalTIS() { return m_destinationTime - m_creationTime; }
	inline Time GetWaitTime() { return m_waitTime; }

protected:
	EntityType m_entityType;
private:
	int m_id;
	static int m_nextID;
	int m_source;

	Time m_creationTime, m_destinationTime;
	Time m_waitTime, m_enterQ, m_exitQ, m_totalTime;

	int m_timesStopped;
	static Time m_totalTimeStopped;
};

class MyEntity : public Entity
{
public:
	MyEntity(Time creationTime) : Entity(creationTime) {};
	~MyEntity() {};

	Entity* New() override { return new MyEntity(GetSimulationTime()); }
};

//class Transport : public Entity
//{
//public:
//	Transport(int numEntities, Time creationTime);
//	void AddEntity(Entity *entity);
//	Entity *RemoveEntity();
//	bool IsFull();
//	bool IsEmpty();
//	Transport* New() override { return new Transport(m_defaultTransportSize, GetSimulationTime()); }
//
//protected:
//	int _maxEntities;
//private:
//	FIFO<Entity> _entities;
//
//	const int m_defaultTransportSize = 10;
//};
