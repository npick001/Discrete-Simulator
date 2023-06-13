#pragma once

#include <list>
#include <string>

#include "wx/graphics.h"
#include "wx/wx.h"

#include "GenericNode.h"
#include "GraphicalElement.h"
#include "GraphicalEdge.h"
#include "Action.h"

class GraphicalEdge;
class MoveNodeAction;

class GraphicalNode : public GraphicalElement {
protected:
	friend class GraphicalEdge;

	GenericNode::Type m_type;

	wxColor m_bodyColor;
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
	GenericNode::Type GetType() const
		{ return m_type; }

	GraphicalNode();
	GraphicalNode(ElementKey id);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(const GraphicalNode& other);

	GraphicalNode& operator=(const GraphicalNode& other);

	// Also disconnects attached edges, preparing them for deletion
	~GraphicalNode();

	inline wxPoint2DDouble GetPosition() { return m_position; }
	inline void SetPosition(const wxPoint2DDouble& position) { m_position = position; }
	wxAffineMatrix2D GetTransform() const;

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

	static const wxSize ms_ioSize;
	static const wxColor ms_ioColor;

	static const wxColor ms_labelColor;
};

typedef SpecificElementContainer<GraphicalNode> NodeMap;

class GraphicalSource : public GraphicalNode {
private:
	static const wxColor ms_bodyColor;

public:
	GraphicalSource();
	GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center);
};

class GraphicalSink : public GraphicalNode {
private:
	static const wxColor ms_bodyColor;

public:
	GraphicalSink();
	GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center);
};