#include <limits.h>
#include "FIFO_Queue.h"
#include "Entity.h"

FIFO_Queue::FIFO_Queue()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;

	m_total = 0;
	m_queueSizeMin = LONG_MAX;
	m_queueSizeMax = LONG_MIN;
	m_queueSizeSum = 0;
	m_count = 0;
	m_cdfWaits = 0;
}

FIFO_Queue::~FIFO_Queue()
{
	while (GetEntity()) {
		Entity* e = GetEntity();
		delete e;
	}
}

void FIFO_Queue::AddEntity(Entity* e)
{
	Node* node = new Node(e);
	if (m_head == 0) {	//empty list
		m_head = m_tail = node;
	}
	else {
		m_tail = m_tail->next = node;
	}
	m_size++;
	m_total++;

	if (m_size > m_queueSizeMax) {
		m_queueSizeMax = m_size;
	}

	e->EnterQueue(GetSimulationTime());
}

Entity* FIFO_Queue::GetEntity()
{
	if (m_head == 0) return 0;
	else {
		Node* n = m_head;
		Entity* e = m_head->m_entity;
		m_head = m_head->next;
		m_size--;

		if (m_size < m_queueSizeMin) {
			m_queueSizeMin = m_size;
		}
		e->LeaveQueue(GetSimulationTime());
		m_cdfWaits += e->LeaveQueue(GetSimulationTime());
		m_queueSizeSum += m_size;
		return e;
	}
}

Entity* FIFO_Queue::ViewEntity()
{
	return (m_head->m_entity);
}

double FIFO_Queue::GetAverageWaitTime()
{
	return (m_cdfWaits / m_total);
}

double FIFO_Queue::GetAverageQueueSize()
{
	return (m_queueSizeSum / m_total);
}

double FIFO_Queue::GetMinimumQueueSize()
{
	return m_queueSizeMin;
}

double FIFO_Queue::GetMaximumQueueSize()
{
	return m_queueSizeMax;
}

