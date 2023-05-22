#pragma once

#include <string>

#include "wx/affinematrix2d.h"
#include "wx/graphics.h"
#include "wx/wx.h"

#include "GraphicalEdge.h"

class GraphicalEdge;

class GraphicalNode {
public:
	GraphicalNode(wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(wxWindow* window, wxPoint2DDouble center);

	enum SelectionState : unsigned int {
		NONE,
		NODE,
		OUTPUT,
		INPUT,
		STATES_MAX
	};

	static const std::string ms_selectionStateNames[SelectionState::STATES_MAX];

	GraphicalNode* m_inputObject;
	GraphicalNode* m_outputObject;

	// Return a copy of m_transform because outside code should not
	// be able to change this variable directly
	inline wxAffineMatrix2D GetTransform() const { return wxAffineMatrix2D(m_transform); }
	
	inline wxString GetText() const { return m_text; }
	inline void SetText(const std::string& text) { m_text = text; }

	// Returns the points at which an edge should be drawn between
	// Points are in world coordinates
	wxPoint2DDouble GetOutputPoint() const;
	wxPoint2DDouble GetInputPoint() const;

	void SetOutputEdge(GraphicalEdge* outputEdge);
	void SetInputEdge(GraphicalEdge* inputEdge);

	inline bool isOutputConnected() const { return m_outputEdge; }
	inline bool isInputConnected() const { return m_inputEdge; }

	void RemoveOutputEdge();
	void RemoveInputEdge();

	void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const;
	SelectionState GetSelectionState(wxAffineMatrix2D cameraTransform,
		wxPoint2DDouble clickPosition) const;

	void Move(wxPoint2DDouble displacement);

	// Nodes are compared based on their ID
	inline bool operator==(const GraphicalNode& other) const {
		return m_id == other.m_id;
	}

private:
	// IDs are unique for each node
	static unsigned int ms_nextID;
	unsigned int m_id;

	wxAffineMatrix2D m_transform;

	wxString m_text;
	wxRect2DDouble m_rect;

	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;
	
	GraphicalEdge* m_inputEdge;
	GraphicalEdge* m_outputEdge;

	static const wxSize ms_bodySize;
	static const wxColor ms_bodyColor;

	static const wxSize ms_ioSize;
	static const wxColor ms_ioColor;

	static const wxColor ms_textColor;
};

// Node selection information
struct SelectionInfo {
	GraphicalNode* node;
	GraphicalNode::SelectionState state;
};