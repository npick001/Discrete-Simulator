#include "GraphicalElement.h"

// Graphical Element

GraphicalElement::GraphicalElement() {
	m_id = 0;
	m_label = "Invalid Element";
}

GraphicalElement::GraphicalElement(GraphicalKey id) {
	m_id = id;
	m_label = "Element " + std::to_string(id);
}

GraphicalElement::GraphicalElement(GraphicalKey id, std::string label) {
	m_id = id;
	m_label = label;
	std::pair<GraphicalKey, GraphicalElement> pair = {m_id, *this};

	const GraphicalKey key = 1;
	auto pair2 = ElementContainer::ElementPair(key, *this);
}

GraphicalElement::GraphicalElement(const GraphicalElement& other) {
	(*this) = other;
}

GraphicalElement& GraphicalElement::operator=(const GraphicalElement& other) {
	if (this == &other)
		return *this;

	m_id = other.m_id;
	m_label = other.m_label;

	return *this;
}

GraphicalElement::~GraphicalElement() {}

// Element Map

ElementContainer::ElementContainer() {
	m_nextID = 1;
	map = ElementMap();
}

std::pair<ElementContainer::ElementMap::iterator, bool> ElementContainer::insert_new() {
	return map.insert(ElementMap::value_type(m_nextID, GraphicalElement(m_nextID)));
}