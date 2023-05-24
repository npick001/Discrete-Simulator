#pragma once

#include <unordered_map>

#include "wx/graphics.h"

typedef int ElementKey;

class GraphicalElement {
protected:
	ElementKey m_id;
	
	std::string m_label;
	wxPoint2DDouble m_labelPos;

public:

	// Derivative elements should have a static member called ms_type that
	// is returned by overriding the GetType function
	enum Type {
		NODE,
		EDGE
	};

	GraphicalElement(ElementKey id);
	GraphicalElement(const GraphicalElement& other);
	GraphicalElement& operator=(const GraphicalElement& other);

	virtual ~GraphicalElement();

	inline const ElementKey& GetID() const { return m_id; }

	inline std::string GetLabel() const { return m_label; }
	inline void SetLabel(const std::string& new_label) { m_label = new_label; }

	virtual void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const = 0;
	virtual Type GetType() const = 0;

	// Elements are guarenteed to be the same if they share the same day
	inline bool operator==(const GraphicalElement& other) const
		{ return m_id == other.m_id; }
};

// Element container will store the pointers to element objects
class ElementContainer {
private:
	typedef std::list<GraphicalElement*> ElementList;

	ElementList m_elements;

public:
	typedef ElementList::iterator iterator;
	typedef ElementList::const_iterator const_iterator;

	inline iterator begin()
		{ return m_elements.begin(); }
	inline iterator end()
		{ return m_elements.end(); }

	inline const_iterator cbegin() const
		{ return m_elements.cbegin(); }
	inline const_iterator cend() const
		{ return m_elements.cend(); }

	inline iterator insert(const_iterator it, GraphicalElement* element)
		{ return m_elements.insert(it, element); }

	inline iterator erase(iterator it)
		{ return m_elements.erase(it); }

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
	typedef std::unordered_map<ElementKey, T> ElementMap;

	ElementMap m_elements;

public:
	inline T& operator[](const ElementKey& key)
		{ return m_elements[key]; }

	inline bool contains(const ElementKey& key)
		{ return bool(m_elements.count(key)); }

};