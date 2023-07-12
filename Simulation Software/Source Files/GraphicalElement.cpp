#include "GraphicalElement.h"

// Graphical Element

const std::string GraphicalElement::ms_selectionStateNames[Selection::State::STATES_MAX] = {
	"NONE",
	"NODE",
	"NODE_OUTPUT",
	"NODE_INPUT",
	"NODE_SIZER",
	"EDGE"
};

GraphicalElement::GraphicalElement()
	: m_id(-1), m_label("ERROR"), m_labelPos() {}

GraphicalElement::GraphicalElement(ElementKey id)
	: m_id(id), m_label("Element " + std::to_string(id)), m_labelPos() {}

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

// Element Container

ElementList::ElementList() : m_elements() {}
