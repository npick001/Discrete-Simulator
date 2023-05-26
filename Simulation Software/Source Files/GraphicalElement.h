#pragma once

#include <unordered_map>

#include "wx/graphics.h"

typedef int ElementKey;
class GraphicalElement;

// Element selection object
// Use just like a GraphicalElement pointer but you can also access .state
// for more selection information
struct Selection {
	enum State : unsigned int {
		NONE,
		NODE,
		NODE_OUTPUT,
		NODE_INPUT,
		EDGE,
		STATES_MAX
	};

	GraphicalElement* element;
	State state;

	Selection() {
		element = nullptr;
		state = State::NONE;
	}

	Selection(GraphicalElement* _element, State _state) {
		element = _element;
		state = _state;
	}

	Selection(const Selection& other)
		{ (*this) = other; }

	Selection& operator=(const Selection& other) {
		if (this == &other)
			return (*this);

		element = other.element;
		state = other.state;

		return (*this);
	}

	~Selection() {
		element = nullptr;
		state = State::NONE;
	}

	// Reset selection to default setup
	inline void reset() {
		element = nullptr;
		state = State::NONE;
	}

	GraphicalElement* operator->() const
		{ return element; }

	operator bool()
		{ return (element && state); }
};

class GraphicalElement {
protected:
	ElementKey m_id;
	
	std::string m_label;
	wxPoint2DDouble m_labelPos;

public:

	// Derivative elements should have a static member called ms_type that
	// is returned by overriding the GetType function
	enum Type : unsigned int {
		NODE,
		EDGE
	};

	static const std::string ms_selectionStateNames[Selection::State::STATES_MAX];

	GraphicalElement();
	GraphicalElement(ElementKey id);
	GraphicalElement(const GraphicalElement& other);
	GraphicalElement& operator=(const GraphicalElement& other);

	virtual ~GraphicalElement() = default;

	inline const ElementKey& GetID() const { return m_id; }

	inline std::string GetLabel() const { return m_label; }
	inline void SetLabel(const std::string& new_label) { m_label = new_label; }

	virtual void Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) const = 0;

	virtual Selection Select(const wxAffineMatrix2D& camera,
		wxPoint2DDouble clickPosition) = 0;

	virtual Type GetType() const = 0;

	// Elements are guarenteed to be the same if they share the same day
	inline bool operator==(const GraphicalElement& other) const
		{ return (m_id == other.m_id); }

	inline bool operator!=(const GraphicalElement& other) const
		{ return !((*this) == other); }
};

// Element container will store the pointers to element objects
class ElementList {
private:
	typedef std::list<GraphicalElement*> List;

	List m_elements;

public:
	typedef List::iterator iterator;
	typedef List::const_iterator const_iterator;
	typedef List::reverse_iterator reverse_iterator;

	ElementList();

	inline iterator begin()
		{ return m_elements.begin(); }
	inline iterator end()
		{ return m_elements.end(); }

	inline const_iterator cbegin()
		{ return m_elements.cbegin(); }
	inline const_iterator cend()
		{ return m_elements.cend(); }

	inline reverse_iterator rbegin()
		{ return m_elements.rbegin(); }
	inline reverse_iterator rend()
		{ return m_elements.rend(); }

	inline bool empty()
		{ return m_elements.empty(); }

	inline void push_back(GraphicalElement* const& element)
		{ m_elements.push_back(element); }

	inline void remove(GraphicalElement* const& element)
		{ m_elements.remove(element); }

	template <class _Pr>
	inline void remove_if(_Pr predicate)
		{ m_elements.remove_if(predicate); }
};

// Specific element container is meant to be used to map element keys (IDs) to an
// element-derived object e.g. typedef SpecificElementContainer<GraphicalNode> NodeContainer
template <typename T>
class SpecificElementContainer {
private:
	std::unordered_map<ElementKey, T> m_elements;
	ElementList* m_link;
	T* m_recent;

public:
	SpecificElementContainer(ElementList* const& link)
		: m_link(link), m_recent(nullptr) {}

	inline void add_new(const T& element) {
		m_elements[element.GetID()] = element;
		m_recent = &m_elements[element.GetID()];
		m_link->push_back(m_recent);
	}

	inline T& operator[](const ElementKey& key)
		{ return m_elements[key]; }

	inline T& operator[](const Selection& selection) {
		return m_elements[selection->GetID()];
	}

	inline bool empty() const
		{ return m_elements.empty(); }

	inline bool contains(const ElementKey& key) const
		{ return bool(m_elements.count(key)); }

	inline T* recent() const
		{ return m_recent; }

	std::size_t erase(const ElementKey& key) {
		if (!contains(key))
			return 0;

		m_link->remove(&m_elements[key]);
		return m_elements.erase(key);
	}

	std::size_t erase(const Selection& selection) {
		if (!contains(selection->GetID()))
			return 0;

		m_link->remove(selection.element);
		return m_elements.erase(selection->GetID());
	}
};