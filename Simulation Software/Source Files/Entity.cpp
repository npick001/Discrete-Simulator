#include "Entity.h"

int Entity::m_nextID = 0;
Time Entity::m_totalTimeStopped = 0;

Entity::Entity(Time creationTime)
{
	m_entityType = entity;
	m_id = m_nextID++;
	m_source = 0;
	m_creationTime = creationTime;
	m_destinationTime = -1;
	m_waitTime = 0;
	m_timesStopped = 0;
	m_enterQ = 0;
	m_exitQ = 0;
}

int Entity::GetID()
{
	return m_id;
}

void Entity::SetType(EntityType myType)
{
	m_entityType = myType;
}

EntityType Entity::GetType()
{
	return m_entityType;
}

void Entity::EnterQueue(Time timeNow)
{
	m_enterQ = timeNow;
}

Time Entity::LeaveQueue(Time timeNow)
{
	m_exitQ = timeNow;
	m_waitTime += (m_exitQ - m_enterQ);
	Entity::m_totalTimeStopped += m_waitTime;
	return m_waitTime;
}

void Entity::SetSource( int id)
{
	m_source = id;
}

int Entity::GetSource()
{
	return(m_source);
}
//
//Transport::Transport(int maxEntities, Time creationTime) : Entity(creationTime)
//{
//	m_entityType = transport;
//	_maxEntities = maxEntities;
//}
//
//void Transport::AddEntity(Entity *entity)
//{
//	if (_entities.GetSize() < _maxEntities) {
//		_entities.AddEntity(entity); }
//}
//
//Entity *Transport::RemoveEntity()
//{
//	return _entities.GetEntity();
//}
//
//bool Transport::IsEmpty()
//{
//	return(_entities.GetSize() == 0);
//}
//
//bool Transport::IsFull()
//{
//	return(_entities.GetSize() == _maxEntities);
//}
