#pragma once

#include <list>
#include <string>

#include "wx/graphics.h"
#include "wx/wx.h"

#include "GraphicalElement.h"
#include "GraphicalEdge.h"

class GraphicalEdge;

class GraphicalNode : public GraphicalElement {
private:
	friend class GraphicalEdge;

	static GraphicalElement::Type ms_type;

	wxPoint2DDouble m_position;

	wxRect2DDouble m_rect;

	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;

	std::list<GraphicalEdge*> m_inputs;
	std::list<GraphicalEdge*> m_outputs;

	// Returns the points at which an edge should be drawn between
	// Points are in world coordinates
	wxPoint2DDouble GetOutputPoint() const;
	wxPoint2DDouble GetInputPoint() const;

public:
	GraphicalElement::Type GetType() const override
		{ return ms_type; }

	GraphicalNode();
	GraphicalNode(ElementKey id);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(const GraphicalNode& other);

	GraphicalNode& operator=(const GraphicalNode& other);

	// Also disconnects attached edges, preparing them for deletion
	~GraphicalNode();

	wxAffineMatrix2D GetTransform() const;
	inline void SetPosition(const wxPoint2DDouble& position) { m_position = position; }

	// Check connection status before using
	inline std::list<GraphicalEdge*> GetOutputs() const { return m_outputs; }
	inline std::list<GraphicalEdge*> GetInputs() const { return m_inputs; }

	void DisconnectOutputs();
	void DisconnectInputs();

	void Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) const override;

	Selection Select(const wxAffineMatrix2D& camera,
		wxPoint2DDouble clickPosition) override;

	void Move(wxPoint2DDouble displacement);

private:
	static const wxSize ms_bodySize;
	static const wxColor ms_bodyColor;

	static const wxSize ms_ioSize;
	static const wxColor ms_ioColor;

	static const wxColor ms_labelColor;
};

typedef SpecificElementContainer<GraphicalNode> NodeMap;