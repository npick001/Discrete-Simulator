#pragma once

#include <list>

#include "wx/wx.h"

#include "GraphicalElement.h"
#include "GraphicalNode.h"
#include "GraphicalEdge.h"

class Canvas : public wxPanel {
public:
	Canvas(wxWindow* parent, wxStatusBar* status);
	~Canvas();

	// Adds a new graphical node to the canvas at a certain location with a given label
	void AddNode(wxPoint2DDouble center, const std::string& label);
	void AddNode(wxPoint2DDouble center);

private:
	ElementKey m_nextID;

	// Used to identify and write to specific fields in the debug status bar
	enum DebugField : unsigned int {
		SELECTION_STATE,
		COMPONENT_SELECTED,
		COMPONENTS_CONNECTED,
		FIELDS_MAX
	};

	// IDs used for popup menu options
	enum {
		ID_ADD_NODE = 200,
		ID_RENAME_NODE,
		ID_DELETE_NODE,
		ID_REMOVE_EDGE
	};

	// Debug status bar used to display node information
	wxStatusBar* m_debugStatusBar;

	// Popup menus
	wxMenu* m_canvasMenu;
	wxMenu* m_nodeMenu;
	wxMenu* m_ioMenu;

	ElementList m_elements;
	NodeMap m_nodes;
	EdgeMap m_edges;

	Selection m_selection;

	GraphicalEdge* m_incompleteEdge;

	wxPoint2DDouble m_previousMousePosition;

	bool m_isPanning = false;
	wxAffineMatrix2D m_cameraPan;
	wxAffineMatrix2D m_cameraZoom;

	wxAffineMatrix2D GetCameraTransform() const;

	// SelectionInfo contains the graphical node which was selected, if any, and the
	// state of the selection, i.e. graphical node, input, output, or none
	// Given a click position, returns a SelectionInfo object describing the action
	Selection Select(wxPoint2DDouble clickPosition);

	void AddNode(const GraphicalNode& obj);
	void DeleteNode();

	// Displacement is based on clickPosition and m_previousPosition which is
	// handled by the mouse event functions
	void PanCamera(wxPoint2DDouble clickPosition);
	void MoveNode(wxPoint2DDouble clickPosition);

	// Popup menu event handlers
	void OnMenuAddNode(wxCommandEvent& event);
	void OnMenuDeleteNode(wxCommandEvent& event);

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
	void OnRightUp(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
};

