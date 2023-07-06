#pragma once

#include <list>

#include "wx/geometry.h"

#include "GraphicalElement.h"
#include "GraphicalNode.h"

class GraphicalNode;
class History;

class Action {
private:
	friend class History;

protected:
	ElementKey m_elementID;

public:
	Action();
	Action(ElementKey id);

	virtual void Redo() = 0;
	virtual void Undo() = 0;
};

class History {
private:
	typedef std::list<Action*> ActionList;

	unsigned int m_maxHistory;
	ActionList m_history;
	ActionList::iterator m_current;

public:
	History(unsigned int maxHistory);
	~History();

	template<typename T>
	void LogAction(const T& action) {
		if (m_current != m_history.begin()) {
			// Delete allocated actions before erasing pointers
			for (auto it = m_history.begin(); it != m_current; it++)
				delete *it;

			m_history.erase(m_history.begin(), m_current);
		}

		m_history.push_front(T(action));
		m_current = m_history.begin();

		// Delete allocated action before erasing pointer
		if (m_history.size() > m_maxHistory) {
			delete m_history.back();
			m_history.pop_back();
		}
	}

	void Redo();
	void Undo();
};

class MoveNodeAction : public Action {
private:
	wxPoint2DDouble m_previousPosition;
	wxPoint2DDouble m_nextPosition;
	SpecificElementContainer<GraphicalNode>* m_link;

public:
	MoveNodeAction();
	MoveNodeAction(ElementKey id, SpecificElementContainer<GraphicalNode>* link);

	inline void SetPreviousPosition(const wxPoint2DDouble& previousPosition)
		{ m_previousPosition = previousPosition; }
	inline void SetNextPosition(const wxPoint2DDouble& nextPosition)
		{ m_nextPosition = nextPosition; }

	void Redo() override;
	void Undo() override;
};