#include "Action.h"

// Action

Action::Action() {}

Action::Action(ElementKey id) : m_elementID(id) {}

// History

History::History(unsigned int maxHistory)
	: m_maxHistory(maxHistory), m_history(), m_current(m_history.begin()) {}

History::~History() {
	for (auto it = m_history.begin(); it != m_history.end(); it++)
		delete* it;
}

void History::Redo() {
	if (m_history.size() == 0 || m_current == m_history.begin())
		return;

	m_current--;
	(*m_current)->Redo();
}

void History::Undo() {
	if (m_history.size() == 0 || m_current == m_history.end())
		return;

	(*m_current)->Undo();
	m_current++;
}

// MoveNodeAction

MoveNodeAction::MoveNodeAction() : Action() {}

MoveNodeAction::MoveNodeAction(ElementKey id, SpecificElementContainer<GraphicalNode>* link)
	: Action(id), m_link(link) {}

void MoveNodeAction::Redo() {
	(*m_link)[m_elementID].SetPosition(m_nextPosition);
}

void MoveNodeAction::Undo() {
	(*m_link)[m_elementID].SetPosition(m_previousPosition);
}