#pragma once

#include <list>

#include "wx/wx.h"

#include "GraphicalNode.h"
#include "GraphicalEdge.h"

class Canvas : public wxPanel {
public:
	Canvas(wxWindow* parent, wxStatusBar* status);

	// Adds a new graphical node to the canvas at a certain location with a given label
	void AddGraphicalNode(wxPoint center, const std::string& text);

private:
	// Used to identify and write to specific fields in the debug status bar
	enum DebugField : unsigned int {
		SELECTION_STATE,
		COMPONENT_SELECTED,
		COMPONENTS_CONNECTED,
		FIELDS_MAX
	};

	wxStatusBar* m_debugStatusBar;

	std::list<GraphicalNode> m_nodes;
	std::list<GraphicalEdge> m_edges;

	GraphicalNode* m_selectedNode;
	GraphicalNode::SelectionState m_nodeSelectionState;

	GraphicalEdge* m_incompleteEdge;

	wxPoint2DDouble m_previousMousePosition;

	bool m_isPanning = false;
	wxAffineMatrix2D m_cameraPan;
	wxAffineMatrix2D m_cameraZoom;

	wxAffineMatrix2D GetCameraTransform() const;

	// SelectionInfo contains the graphical node which was selected, if any, and the
	// state of the selection, i.e. graphical node, input, output, or none
	// Given a click position, returns a SelectionInfo object describing the action
	SelectionInfo GetNodeSelectionInfo(wxPoint2DDouble clickPosition);

	// Displacement is based on clickPosition and m_previousPosition which is
	// handled by the mouse event functions
	void PanCamera(wxPoint2DDouble clickPosition);
	void MoveNode(wxPoint2DDouble clickPosition);

	// OnPaint is triggered by a call to Refresh
	// Draws custom graphical elements to the canvas such as graphical nodes and connections
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	// Mouse event handlers used for interacting with graphical nodes and panning the camera
	void OnMiddleDown(wxMouseEvent& event);
	void OnMiddleUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);
};

