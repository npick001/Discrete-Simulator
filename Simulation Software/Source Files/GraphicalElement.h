#pragma once

#include <unordered_map>

#include "wx/graphics.h"

typedef int ElementKey;
class GraphicalElement;

// Element selection information
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

	inline void reset() {
		element = nullptr;
		state = State::NONE;
	}

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

	virtual ~GraphicalElement();

	inline const ElementKey& GetID() const { return m_id; }

	inline std::string GetLabel() const { return m_label; }
	inline void SetLabel(const std::string& new_label) { m_label = new_label; }

	virtual void Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) const = 0;

	virtual Selection::State Select(const wxAffineMatrix2D& camera,
		wxPoint2DDouble clickPosition) const = 0;

	virtual Type GetType() const = 0;

	// Elements are guarenteed to be the same if they share the same day
	inline bool operator==(const GraphicalElement& other) const
		{ return (m_id == other.m_id); }

	inline bool operator!=(const GraphicalElement& other) const
		{ return !((*this) == other); }
};

// Element container will store the pointers to element objects
class ElementContainer {
private:
	typedef std::list<GraphicalElement*> ElementList;

	ElementList m_elements;

public:
	typedef ElementList::iterator iterator;
	typedef ElementList::const_iterator const_iterator;
	typedef ElementList::reverse_iterator reverse_iterator;

	ElementContainer();

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
	ElementContainer* m_link;

public:
	SpecificElementContainer(ElementContainer* const& link) {
		m_link = link;
	}

	inline void add_new(const T& element) {
		m_elements[element.GetID()] = element;
		m_link->push_back(&m_elements[element.GetID()]);
	}

	inline T& operator[](const ElementKey& key)
		{ return m_elements[key]; }

	inline T& operator[](const Selection& selection) {
		return m_elements[selection.element->GetID()];
	}

	inline bool empty() const
		{ return m_elements.empty(); }

	inline bool contains(const ElementKey& key) const
		{ return bool(m_elements.count(key)); }

	inline std::size_t erase(const ElementKey& key) {
		if (!contains(key))
			return 0;

		m_link->remove(&m_elements[key]);
		return m_elements.erase(key);
	}

	inline std::size_t erase(const Selection& selection) {
		if (!contains(selection.element->GetID()))
			return 0;

		m_link->remove(selection.element);
		return m_elements.erase(selection.element->GetID());
	}
};