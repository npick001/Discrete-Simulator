#include "GraphicalElement.h"

// Graphical Element

GraphicalElement::GraphicalElement(ElementKey id)
	: m_id(id), m_label("Element"), m_labelPos() {}

GraphicalElement::GraphicalElement(const GraphicalElement& other) {
	(*this) = other;
}

GraphicalElement& GraphicalElement::operator=(const GraphicalElement& other) {
	if (this == &other)
		return *this;

	m_id = other.m_id;
	m_label = other.m_label;
	m_labelPos = other.m_labelPos;

	return *this;
}

GraphicalElement::~GraphicalElement() {}