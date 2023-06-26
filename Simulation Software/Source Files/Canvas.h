#pragma once

#include <list>

#include "wx/wx.h"

#include "MainFrame.h"
#include "GraphicalElement.h"
#include "GraphicalNode.h"
#include "GraphicalEdge.h"
#include "Action.h"

class Canvas : public wxPanel {
public:
	Canvas(wxWindow* parent, wxStatusBar* status);
	~Canvas();

	// Adds a new graphical node to the canvas at a certain location with a given label
	void AddNode(GenericNode::Type type, wxPoint2DDouble center, const std::string& label);
	void AddNode(GenericNode::Type type, wxPoint2DDouble center);

	void TransformOriginLocation(wxSize canvasSize);

private:
	ElementKey m_nextID;

	// Used to identify and write to specific fields in the debug status bar
	enum DebugField : unsigned int {
		SELECTION_STATE,
		COMPONENT_SELECTED,
		COMPONENTS_CONNECTED,
		MOUSE_POSITION,
		FIELDS_MAX
	};

	// IDs used for popup menu options
	enum {
		ID_ADD_NODE = 200,
		ID_ADD_SOURCE,
		ID_ADD_SERVER,
		ID_ADD_SINK,
		ID_RENAME_NODE,
		ID_DELETE_NODE,
		ID_REMOVE_EDGE
	};

	// Debug status bar used to display node information
	wxStatusBar* m_debugStatusBar;

	// Popup menus
	wxMenu* m_canvasMenu;
	wxMenu* m_nodeMenu;
	wxMenu* m_nodeSubMenu;
	wxMenu* m_ioMenu;
	
	// Element containers
	ElementList m_elements;
	NodeMap m_nodes;
	EdgeMap m_edges;

	// Grid things
	// "variable grid size in world space" approach
	std::vector<double> m_gridSizes;
	wxSize m_canvasSize;

	/*std::vector<GraphicalNode> m_nodes;
	std::vector<GraphicalEdge> m_edges;*/

	// History of actions
	History m_history;
	MoveNodeAction m_moveNodeAction;

	Selection m_selection;

	GraphicalEdge* m_incompleteEdge;

	wxPoint2DDouble m_previousMousePosition;

	bool isCtrlDown = false;
	bool isShiftDown = false;

	bool m_isPanning = false;
	wxAffineMatrix2D m_cameraPan;
	wxAffineMatrix2D m_cameraZoom;
	wxPoint m_origin;
	double m_zoomLevel;

	wxAffineMatrix2D GetCameraTransform() const;
	void DrawGrid(wxGraphicsContext* gc);

	// Selection contains the graphical node which was selected, if any, and the
	// state of the selection, i.e. graphical node, input, output, or none
	// Given a click position, returns a SelectionInfo object describing the action
	Selection Select(wxPoint2DDouble clickPosition);

	void AddNode(GraphicalNode* obj);
	void DeleteNode();

	// Displacement is based on clickPosition and m_previousPosition which is
	// handled by the mouse event functions
	void PanCamera(wxPoint2DDouble clickPosition);
	void MoveNode(wxPoint2DDouble clickPosition);

	// Popup menu event handlers
	void OnMenuAddNode(wxCommandEvent& event);
	void OnMenuAddSource(wxCommandEvent& event);
	void OnMenuAddServer(wxCommandEvent& event);
	void OnMenuAddSink(wxCommandEvent& event);
	void OnMenuDeleteNode(wxCommandEvent& event);

	// Draws custom graphical elements to the canvas such as graphical nodes and connections
	void OnPaint(wxPaintEvent& event);

	// Refresh the canvas upon resizing
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

	// Key events for handling shortcuts
	void OnCharHook(wxKeyEvent& event);
};