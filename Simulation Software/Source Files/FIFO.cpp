#include <limits.h>
#include "FIFO.h"
#include "Entity.h"

FIFO::FIFO()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;

	m_total = 0;
	m_queueSizeMin = 0;
	m_queueSizeMax = 0;
	m_queueSizeSum = 0;
	m_count = 0;
	m_cdfWaits = 0;
}

void FIFO::AddEntity(Entity* e)
{
	Node* node = new Node(e);
	if (m_head == 0) {	//empty list
		m_head = m_tail = node;
	}
	else {
		m_tail = m_tail->next = node;
	}
	m_size++;

	if (m_queueSizeMax < m_size) {
		m_queueSizeMax = m_size;
	}

	e->EnterQueue(GetSimulationTime());
}

Entity* FIFO::GetEntity()
{
	if (m_head == 0) return 0;
	else {
		Node* n = m_head;
		Entity* e = m_head->m_entity;
		m_head = m_head->next;
		m_size--;

		if (m_queueSizeMin > m_size) {
			m_queueSizeMin = m_size;
		}

		m_cdfWaits += e->LeaveQueue(GetSimulationTime());
		m_queueSizeSum += m_size;
		return e;
	}
}

Entity* FIFO::ViewEntity()
{
	return (m_head->m_entity);
}

double FIFO::GetAverageWaitTime()
{
	return (m_cdfWaits / m_total);
}

double FIFO::GetAverageQueueSize()
{
	return (m_queueSizeSum / m_total);
}

double FIFO::GetMinimumQueueSize()
{
	return m_queueSizeMin;
}

double FIFO::GetMaximumQueueSize()
{
	return m_queueSizeMax;
}

