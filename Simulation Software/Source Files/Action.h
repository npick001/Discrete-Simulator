#pragma once

#include <deque>

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
	unsigned int m_maxHistory;
	std::deque<Action*> m_history;
	unsigned int m_current;

public:
	History(unsigned int maxHistory);
	~History();

	template<typename T>
	void LogAction(const T& action) {
		if (m_current > 0) {
			auto it = m_history.begin();
			it = it + m_current;

			// Delete allocated actions before erasing pointers
			for (unsigned int i = 0; i < m_current; i++)
				delete m_history[i];

			m_history.erase(m_history.begin(), it);
			m_current = 0;
		}

		m_history.push_front(new T(action));

		// Delete allocated action before erasing pointer
		if (m_history.size() > m_maxHistory) {
			delete m_history[m_maxHistory];
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