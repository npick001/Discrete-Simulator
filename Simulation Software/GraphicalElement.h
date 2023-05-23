#pragma once

#include <unordered_map>

class GraphicalElement;

typedef unsigned int GraphicalKey;
typedef std::unordered_map<GraphicalKey, GraphicalElement> ElementMap;

class GraphicalElement {
public:
	GraphicalElement();

private:
	// Stores all elements
	static ElementMap ms_elements;

	static GraphicalKey ms_nextID;
	GraphicalKey m_id;


};

