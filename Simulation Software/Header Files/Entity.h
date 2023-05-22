#ifndef ENTITY_H
#define ENTITY_H

#include "FIFO.h"

enum EntityType { entity, transport, batch };

class Entity
{
public:
	Entity();
	int GetID();
	virtual Entity *New() = 0;
	void SetSource(int id);
	int GetSource();
	EntityType GetType();
protected:
	EntityType _entityType;
private:
	int _id;
	static int _nextID;
	int _source;
};

class MyEntity : public Entity
{
public:
	MyEntity() {};

	Entity* New() { return new MyEntity(); };
};

class Transport : public Entity
{
public:
	Transport(int numEntities);
	void AddEntity(Entity *entity);
	Entity *RemoveEntity();
	bool IsFull();
	bool IsEmpty();
protected:
	int _maxEntities;
private:
	FIFO<Entity> _entities;
};

#endif
