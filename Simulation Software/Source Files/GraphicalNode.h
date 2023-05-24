#pragma once

#include <string>

#include "wx/affinematrix2d.h"
#include "wx/graphics.h"
#include "wx/wx.h"

#include "GraphicalElement.h"
#include "GraphicalEdge.h"

class GraphicalEdge;

class GraphicalNode : public GraphicalElement {
private:
	friend class GraphicalEdge;

	wxAffineMatrix2D m_transform;

	wxRect2DDouble m_rect;

	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;

	GraphicalEdge* m_inputEdge;
	GraphicalEdge* m_outputEdge;

public:
	GraphicalNode(ElementKey id);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(const GraphicalNode& other);

	GraphicalNode& operator=(const GraphicalNode& other);

	// Also disconnects attached edges, preparing them for deletion
	~GraphicalNode();

	enum SelectionState : unsigned int {
		NONE,
		NODE,
		OUTPUT,
		INPUT,
		STATES_MAX
	};

	static const std::string ms_selectionStateNames[SelectionState::STATES_MAX];

	// Return a copy of m_transform because outside code should not
	// be able to change this variable directly
	inline const wxAffineMatrix2D& GetTransform() const { return m_transform; }

	// Returns the points at which an edge should be drawn between
	// Points are in world coordinates
	wxPoint2DDouble GetOutputPoint() const;
	wxPoint2DDouble GetInputPoint() const;

	inline const GraphicalEdge& GetOutputEdge() { return *m_outputEdge; }
	void SetOutputEdge(GraphicalEdge* outputEdge);

	inline const GraphicalEdge& GetInputEdge() { return *m_inputEdge; }
	void SetInputEdge(GraphicalEdge* inputEdge);

	inline bool isOutputConnected() const { return m_outputEdge != nullptr; }
	inline bool isInputConnected() const { return m_inputEdge != nullptr; }

	void DisconnectOutput();
	void DisconnectInput();

	void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const override;

	SelectionState GetSelectionState(wxAffineMatrix2D cameraTransform,
		wxPoint2DDouble clickPosition) const;

	void Move(wxPoint2DDouble displacement);

private:
	static const wxSize ms_bodySize;
	static const wxColor ms_bodyColor;

	static const wxSize ms_ioSize;
	static const wxColor ms_ioColor;

	static const wxColor ms_labelColor;
};

// Node selection information
struct SelectionInfo {
	GraphicalNode* node;
	GraphicalNode::SelectionState state;
};