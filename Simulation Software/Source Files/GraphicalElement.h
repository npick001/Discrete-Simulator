#pragma once

#include <utility>
#include <string>
#include <unordered_map>

typedef unsigned int GraphicalKey;

class ElementContainer;

class GraphicalElement {
protected:
	friend class ElementContainer;

	GraphicalKey m_id;
	std::string m_label;

	GraphicalElement(GraphicalKey id);
	GraphicalElement(GraphicalKey id, std::string label);

	GraphicalElement& operator=(const GraphicalElement& other);

public:
	GraphicalElement();
	GraphicalElement(const GraphicalElement& other);
	~GraphicalElement();

	// Elements are guarenteed to be the same if they share the same day
	inline bool operator==(const GraphicalElement& other) const {
		return m_id == other.m_id;
	}
};

class ElementContainer {
public:
	typedef std::unordered_map<GraphicalKey, GraphicalElement> ElementMap;
	typedef std::unordered_map<GraphicalKey, GraphicalElement>::value_type ElementPair;

private:
	GraphicalKey m_nextID;
	std::unordered_map<GraphicalKey, GraphicalElement> map;

public:

	ElementContainer();

	inline std::pair<ElementMap::iterator, bool> insert_new();
	
	inline ElementMap::iterator erase(ElementMap::iterator it) { return map.erase(it); }
	inline ElementMap::size_type erase(GraphicalKey key) { return map.erase(key); }
	
	inline GraphicalElement& operator[](GraphicalKey key) { return map[key]; }
};