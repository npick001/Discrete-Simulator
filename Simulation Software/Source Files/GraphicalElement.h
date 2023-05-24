#pragma once

#include <utility>
#include <string>
#include <unordered_map>

#include "wx/graphics.h"

typedef unsigned int ElementKey;

class GraphicalElement {
protected:
	ElementKey m_id;
	
	std::string m_label;
	wxPoint2DDouble m_labelPos;

public:
	GraphicalElement(ElementKey id);
	GraphicalElement(const GraphicalElement& other);
	GraphicalElement& operator=(const GraphicalElement& other);

	virtual ~GraphicalElement();

	inline const ElementKey& GetID() const { return m_id; }

	inline std::string GetLabel() const { return m_label; }
	inline void SetLabel(const std::string& new_label) { m_label = new_label; }

	virtual void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const;

	// Elements are guarenteed to be the same if they share the same day
	inline bool operator==(const GraphicalElement& other) const {
		return m_id == other.m_id;
	}
};