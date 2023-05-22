#pragma once

#include <string>

#include "wx/affinematrix2d.h"
#include "wx/graphics.h"
#include "wx/wx.h"

#include "GraphicalEdge.h"

class GraphicalEdge;

class GraphicalNode {
public:
	GraphicalNode(wxWindow* window, wxPoint center, const std::string& _text);

	enum SelectionState : unsigned int {
		SELECT_NONE,
		SELECT_NODE,
		SELECT_OUTPUT,
		SELECT_INPUT,
		SELECT_STATES_MAX
	};

	static const std::string ms_selectionStateNames[SelectionState::SELECT_STATES_MAX];

	GraphicalNode* m_inputObject;
	GraphicalNode* m_outputObject;

	// Return a copy of m_transform because outside code should not
	// be able to change this variable directly
	inline wxAffineMatrix2D GetTransform() const { return wxAffineMatrix2D(m_transform); }

	inline wxString GetText() const { return m_text;  }
	
	// Returns the points at which an edge should be drawn between
	// Points are in world coordinates
	wxPoint2DDouble GetOutputPoint() const;
	wxPoint2DDouble GetInputPoint() const;

	void SetOutputEdge(GraphicalEdge* outputEdge);
	void SetInputEdge(GraphicalEdge* inputEdge);

	void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const;
	SelectionState GetSelectionState(wxAffineMatrix2D cameraTransform,
		wxPoint2DDouble clickPosition) const;

	void Move(wxPoint2DDouble displacement);

	// DEBUG code
	GraphicalEdge* m_inputEdge;
	GraphicalEdge* m_outputEdge;
private:

	wxAffineMatrix2D m_transform;

	wxString m_text;
	wxRect2DDouble m_rect;

	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;

	static const wxSize ms_bodySize;
	static const wxColor ms_bodyColor;

	static const wxSize ms_ioSize;
	static const wxColor ms_ioColor;

	static const wxColor ms_textColor;
};

struct SelectionInfo {
	GraphicalNode* node;
	GraphicalNode::SelectionState state;
};