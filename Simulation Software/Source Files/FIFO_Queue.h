#pragma once

#include "SimulationExecutive.h"

/*
FIFO Queue - First In/First Out
This class only handles entities for a FIFO Queue scheme.
This data structure provides built in functions for statistics collection. 
*/
class Entity;
class FIFO_Queue
{
public:
	FIFO_Queue();
	~FIFO_Queue();

	void AddEntity(Entity* e);
	Entity* GetEntity();
	Entity* ViewEntity();

	double GetAverageWaitTime();
	double GetAverageQueueSize();
	double GetMinimumQueueSize();
	double GetMaximumQueueSize();

	bool IsEmpty() { return (m_size == 0); }
	int GetSize() { return m_size; }
private:
	struct Node
	{
		Node(Entity* e) {
			m_entity = e;
			next = 0;
		}
		Entity* m_entity;
		Node *next;
	};

	Node *m_head;
	Node *m_tail;
	int m_size, m_queueSizeMax, m_queueSizeMin,m_count, m_queueSizeSum;
	double m_total;
	Time m_cdfWaits;
};
