#include "Entity.h"

int Entity::_nextID = 0;

Entity::Entity()
{
	_entityType = entity;
	_id = _nextID++;
}

int Entity::GetID()
{
	return _id;
}

EntityType Entity::GetType()
{
	return _entityType;
}

void Entity::SetSource( int id)
{
	_source = id;
}

int Entity::GetSource()
{
	return(_source);
}

Transport::Transport(int maxEntities)
{
	_entityType = transport;
	_maxEntities = maxEntities;
}

void Transport::AddEntity(Entity *entity)
{
	if (_entities.GetSize() < _maxEntities) {
		_entities.AddEntity(entity); }
}

Entity *Transport::RemoveEntity()
{
	return _entities.GetEntity();
}

bool Transport::IsEmpty()
{
	return(_entities.GetSize() == 0);
}

bool Transport::IsFull()
{
	return(_entities.GetSize() == _maxEntities);
}
