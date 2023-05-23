#include "GraphicalElement.h"

ElementMap GraphicalElement::ms_elements = ElementMap();
GraphicalKey GraphicalElement::ms_nextID = 1;

GraphicalElement::GraphicalElement() {
	m_id = ms_nextID;
	ms_nextID++;

	ms_elements[m_id] = *this;
}

GraphicalElement::~GraphicalElement() {
	ms_elements.erase(m_id);
}