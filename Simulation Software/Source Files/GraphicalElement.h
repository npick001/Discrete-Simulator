#pragma once

#include <unordered_map>

class GraphicalElement;

typedef unsigned int GraphicalKey;
typedef std::unordered_map<GraphicalKey, GraphicalElement> ElementMap;

class GraphicalElement {
public:
	GraphicalElement();
	~GraphicalElement();

	// Elements are always compared by their ID
	inline bool operator==(const GraphicalElement& other) const {
		return m_id == other.m_id;
	}
protected:
	// Stores all elements
	static ElementMap ms_elements;

	static GraphicalKey ms_nextID;
	GraphicalKey m_id;
};

