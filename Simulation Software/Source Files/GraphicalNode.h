#pragma once

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

	wxAffineMatrix2D m_transform;

	wxRect2DDouble m_rect;

	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;

	GraphicalEdge* m_inputEdge;
	GraphicalEdge* m_outputEdge;

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

	// Return a copy of m_transform because outside code should not
	// be able to change this variable directly
	inline const wxAffineMatrix2D& GetTransform() const { return m_transform; }

	// Check connection status before using
	inline const GraphicalEdge& GetOutputEdge() const { return *m_outputEdge; }
	inline const GraphicalEdge& GetInputEdge() const { return *m_inputEdge; }

	inline bool IsOutputConnected() const { return m_outputEdge != nullptr; }
	inline bool IsInputConnected() const { return m_inputEdge != nullptr; }

	void DisconnectOutput();
	void DisconnectInput();

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

typedef SpecificElementContainer<GraphicalNode> NodeContainer;