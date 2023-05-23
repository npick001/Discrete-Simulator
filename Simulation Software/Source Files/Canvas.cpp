#include "Canvas.h"

#include "wx/graphics.h"
#include "wx/dcbuffer.h"

Canvas::Canvas(wxWindow* parent, wxStatusBar* statusBar)
	: wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// UI Elements
	m_debugStatusBar = statusBar;
	m_debugStatusBar->SetFieldsCount(DebugField::FIELDS_MAX);

	m_canvasMenu = new wxMenu("Canvas");
	m_canvasMenu->Append(ID_ADD_NODE, "Add node", "Add a node to the canvas");
	m_canvasMenu->Bind(wxEVT_MENU, &Canvas::OnMenuAddNode, this, ID_ADD_NODE);

	m_nodeMenu = new wxMenu("");
	m_nodeMenu->Append(ID_DELETE_NODE, "Delete", "Delete the currently selected node");
	m_nodeMenu->Bind(wxEVT_MENU, &Canvas::OnMenuDeleteNode, this, ID_DELETE_NODE);

	// Canvas Elements
	AddNode(FromDIP(wxPoint(-150, 0)));
	AddNode(FromDIP(wxPoint(   0, 0)));
	AddNode(FromDIP(wxPoint( 150, 0)));

	wxSize size = parent->GetSize();
	m_cameraZoom.Translate(size.GetWidth() / 2, size.GetHeight() / 2);

	m_nodeSelectionState = GraphicalNode::SelectionState::NONE;

	// Event bindings
	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &Canvas::OnSize, this);
	this->Bind(wxEVT_MIDDLE_DOWN, &Canvas::OnMiddleDown, this);
	this->Bind(wxEVT_MIDDLE_UP, &Canvas::OnMiddleUp, this);
	this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnLeftDown, this);
	this->Bind(wxEVT_LEFT_UP, &Canvas::OnLeftUp, this);
	this->Bind(wxEVT_MOTION, &Canvas::OnMotion, this);
	this->Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouseWheel, this);
	this->Bind(wxEVT_RIGHT_UP, &Canvas::OnRightUp, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnLeaveWindow, this);
	this->Bind(wxEVT_ENTER_WINDOW, &Canvas::OnEnterWindow, this);
}

Canvas::~Canvas() {
	m_canvasMenu->Destroy(ID_ADD_NODE);
	delete m_canvasMenu;

	m_nodeMenu->Destroy(ID_DELETE_NODE);
	delete m_nodeMenu;
}

// Adds a graphical node to the canvas
void Canvas::AddNode(wxPoint2DDouble center, const std::string& text) {
	GraphicalNode obj(this, center, text);
	m_nodes.push_back(obj);

	Refresh();
}

// Adds a graphical node to the canvas, name is auto populated with id number
void Canvas::AddNode(wxPoint2DDouble center) {
	GraphicalNode obj(this, center);
	m_nodes.push_back(obj);

	Refresh();
}

// Deletes the currently selected node
void Canvas::DeleteNode() {
	if (!m_selectedNode)
		return;

	// Get input and output edge IDs, before removing all references, for later removal
	unsigned int outputID = 0;
	unsigned int inputID = 0;

	if (m_selectedNode->isOutputConnected())
		outputID = m_selectedNode->GetOutputEdge().GetID();

	if (m_selectedNode->isInputConnected())
		inputID = m_selectedNode->GetInputEdge().GetID();

	// Node is deleted and it's edges are disconnected on all sides
	m_nodes.remove(*m_selectedNode);

	// Edges are then deleted afterwards
	if (outputID || inputID)
		m_edges.remove_if([inputID, outputID](const GraphicalEdge& edge) {
			return edge.GetID() == inputID || edge.GetID() == outputID;
		});

	Refresh();
}

// Returns the camera transform which is the scaling matrix multiplied by the translation matrix
wxAffineMatrix2D Canvas::GetCameraTransform() const {
	wxAffineMatrix2D cameraTransform = m_cameraZoom;
	cameraTransform.Concat(m_cameraPan);
	return cameraTransform;
}

// Return selection information containing which graphical node, if any, was selected and the state of the selection
SelectionInfo Canvas::GetNodeSelectionInfo(wxPoint2DDouble clickPosition) {

	if (m_nodes.empty())
		return { nullptr, GraphicalNode::SelectionState::NONE };

	GraphicalNode::SelectionState state;

	auto selectedComponentIter = std::find_if(m_nodes.rbegin(), m_nodes.rend(),
		[this, &state, clickPosition](const GraphicalNode& node) mutable {
		state = node.GetSelectionState(GetCameraTransform(), clickPosition);
		return (state != GraphicalNode::SelectionState::NONE);
	});

	m_debugStatusBar->SetStatusText("Selection State: " + GraphicalNode::ms_selectionStateNames[state],
		DebugField::SELECTION_STATE);

	if (selectedComponentIter == m_nodes.rend()) {
		m_debugStatusBar->SetStatusText("No object selected", DebugField::COMPONENT_SELECTED);
		return { nullptr, GraphicalNode::SelectionState::NONE };
	}

	m_debugStatusBar->SetStatusText("Object Selected: " + selectedComponentIter->GetText(),
		DebugField::COMPONENT_SELECTED);

	return { &(*selectedComponentIter), state};
} 

// Pan the camera by the displacement of the mouse position
void Canvas::PanCamera(wxPoint2DDouble clickPosition) {
	auto dragVector = clickPosition - m_previousMousePosition;

	auto inv = GetCameraTransform();
	inv.Invert();
	dragVector = inv.TransformDistance(dragVector);

	m_cameraPan.Translate(dragVector.m_x, dragVector.m_y);

	m_previousMousePosition = clickPosition;

	Refresh();
}

// Move a graphical node by the displacement of the mouse position
void Canvas::MoveNode(wxPoint2DDouble clickPosition) {
	auto dragVector = clickPosition - m_previousMousePosition;

	auto inv = GetCameraTransform();
	inv.Concat(m_selectedNode->GetTransform());
	inv.Invert();
	dragVector = inv.TransformDistance(dragVector);

	m_selectedNode->Move(dragVector);

	m_previousMousePosition = clickPosition;

	Refresh();
}

// Called upon user selecting add node in popup canvas menu
void Canvas::OnMenuAddNode(wxCommandEvent& event) {
	auto inverse = GetCameraTransform();
	inverse.Invert();

	AddNode(inverse.TransformPoint(m_previousMousePosition));
}

// Called upon user selecting delete node in popup node menu
void Canvas::OnMenuDeleteNode(wxCommandEvent& event) {
	DeleteNode();

	event.Skip();
}

// Capture when user holds down middle mouse button in order to pan
void Canvas::OnMiddleDown(wxMouseEvent& event) {
	m_isPanning = true;
	m_previousMousePosition = event.GetPosition();

	event.Skip();
}

// Capture release of middle mouse button in order to stop panning
void Canvas::OnMiddleUp(wxMouseEvent& event) {
	m_isPanning = false;

	event.Skip();
}

// Capture when user holds down left mouse button in order to drag or connect nodes
// Panning also occurs when selection state is none
void Canvas::OnLeftDown(wxMouseEvent& event) {
	SelectionInfo selection = GetNodeSelectionInfo(event.GetPosition());
	m_selectedNode = selection.node;
	m_nodeSelectionState = selection.state;

	// Defined before switch statement to avoid redefinition
	GraphicalEdge edge;

	switch (m_nodeSelectionState) {

	// Prepare to drag selected node
	case GraphicalNode::SelectionState::NODE:
		m_previousMousePosition = event.GetPosition();
		break;

	// Instatiate an edge and connect source to node's output
	case GraphicalNode::SelectionState::OUTPUT:
		m_edges.push_back(edge);

		// Get pointer to edge that was just added
		m_incompleteEdge = &(*m_edges.rbegin());
		m_incompleteEdge->ConnectSource(m_selectedNode);
		break;

	// Instatiate an edge and connect destination to node's input
	case GraphicalNode::SelectionState::INPUT:
		m_edges.push_back(edge);

		// Get pointer to edge that was just added
		m_incompleteEdge = &(*m_edges.rbegin());
		m_incompleteEdge->ConnectDestination(m_selectedNode);
		break;

	// Panning also works with left click
	case GraphicalNode::SelectionState::NONE:
		m_isPanning = true;
		m_previousMousePosition = event.GetPosition();
		break;
	}

	Refresh();

	event.Skip();
}

// Capture release of left mouse button in order to stop dragging or finalize a connection
void Canvas::OnLeftUp(wxMouseEvent& event) {

	// Holds which node and selection state occurred upon user's left button up
	auto endSelection = GetNodeSelectionInfo(event.GetPosition());

	wxPoint2DDouble outEdgeSourcePoint;

	switch (m_nodeSelectionState) {

	// Check that user selected an input to pair with the output and then connect
	case GraphicalNode::SelectionState::OUTPUT:
		if (endSelection.state == GraphicalNode::SelectionState::INPUT
			&& endSelection.node != m_selectedNode) {

			m_incompleteEdge->ConnectDestination(endSelection.node);

			m_debugStatusBar->SetStatusText("Connected " + m_selectedNode->GetText() + " to "
				+ endSelection.node->GetText(), DebugField::COMPONENTS_CONNECTED);
		}
		else {
			m_incompleteEdge->Disconnect();
			m_edges.pop_back();
		}
		break;

	// Check that user selected an output to pair with the input and then connect
	case GraphicalNode::SelectionState::INPUT:
		if (endSelection.state == GraphicalNode::SelectionState::OUTPUT
			&& endSelection.node != m_selectedNode) {

			m_incompleteEdge->ConnectSource(endSelection.node);

			m_debugStatusBar->SetStatusText("Connected " + endSelection.node->GetText() + " to "
				+ m_selectedNode->GetText(), DebugField::COMPONENTS_CONNECTED);
		}
		else {
			m_incompleteEdge->Disconnect();
			m_edges.pop_back();
		}
		break;

	// User is no longer panning camera
	case GraphicalNode::SelectionState::NONE:
		m_isPanning = false;
	}

	m_selectedNode = nullptr;
	m_incompleteEdge = nullptr;
	m_nodeSelectionState = GraphicalNode::SelectionState::NONE;

	Refresh();

	event.Skip();
}

// Capture mouse movement for panning and moving graphical nodes
void Canvas::OnMotion(wxMouseEvent& event) {
	wxPoint2DDouble mousePosition = event.GetPosition();

	if (m_isPanning && (event.ButtonIsDown(wxMOUSE_BTN_LEFT)
		|| event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))) {

		PanCamera(mousePosition);
	}
	else if (!m_selectedNode)
		return;

	// Defined before switch statement to avoid redefinition
	// Used to convert from window to world coordinates
	wxAffineMatrix2D inverse = GetCameraTransform();
	inverse.Invert();

	switch (m_nodeSelectionState) {

	// Move node
	case GraphicalNode::SelectionState::NODE:
		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			MoveNode(mousePosition);
		break;

	// Move destination point of edge
	case GraphicalNode::SelectionState::OUTPUT:

		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			m_incompleteEdge->SetDestinationPoint(inverse.TransformPoint(mousePosition));
		else {
			m_incompleteEdge->Disconnect();
			m_incompleteEdge = nullptr;
			m_edges.pop_back();
		}

		Refresh();
		break;

	// Move source point of edge
	case GraphicalNode::SelectionState::INPUT:

		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			m_incompleteEdge->SetSourcePoint(inverse.TransformPoint(mousePosition));
		else {
			m_incompleteEdge->Disconnect();
			m_incompleteEdge = nullptr;
			m_edges.pop_back();
		}

		Refresh();
		break;
	}

	event.Skip();
}

// Capture mouse scrolling in order to zoom the camera
void Canvas::OnMouseWheel(wxMouseEvent& event) {
	double scaleFactor = pow(2, 0.1 * event.GetWheelRotation() / event.GetWheelDelta());
	m_cameraZoom.Scale(scaleFactor, scaleFactor);

	Refresh();

	event.Skip();
}

// Capture release of right mouse button in order to present user with a popup menu
void Canvas::OnRightUp(wxMouseEvent& event) {
	SelectionInfo selection = GetNodeSelectionInfo(event.GetPosition());
	m_selectedNode = selection.node;
	m_nodeSelectionState = selection.state;

	switch (selection.state) {

	// Popup node menu options
	case GraphicalNode::SelectionState::NODE:
		m_nodeMenu->SetTitle(selection.node->GetText());
		PopupMenu(m_nodeMenu);
		break;

	// Popup I/O menu options and bind output edge disconnection handler
	case GraphicalNode::SelectionState::OUTPUT:
		wxLogMessage("Right clicked output of %s", selection.node->GetText());
		break;

	// Popup I/O menu options and bind input edge disconnection handler
	case GraphicalNode::SelectionState::INPUT:
		wxLogMessage("Right clicked input of %s", selection.node->GetText());
		break;

	// Popup canvas menu options
	case GraphicalNode::SelectionState::NONE:
		m_previousMousePosition = event.GetPosition();
		PopupMenu(m_canvasMenu);
		break;
	}

	event.Skip();
}

// Unable to track mouse position outside of window; therefore, panning and dragging is disabled when the mouse leaves
void Canvas::OnLeaveWindow(wxMouseEvent& event) {

	event.Skip();
}

// Prevent camera from suddenly jerking to new position upon re-entry of window
void Canvas::OnEnterWindow(wxMouseEvent& event) {
	if (m_isPanning)
		m_previousMousePosition = event.GetPosition();
}

// Custom graphical elements such as graphical nodes and connections are drawn here
void Canvas::OnPaint(wxPaintEvent& event) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (!gc)
		return;

	for (const GraphicalNode& node : m_nodes)
		node.Draw(GetCameraTransform(), gc);

	for (const GraphicalEdge& edge : m_edges)
		edge.Draw(GetCameraTransform(), gc);

	delete gc;

	event.Skip();
}

// Refresh, i.e. redraw the window, upon resizing
void Canvas::OnSize(wxSizeEvent& event) {
	Refresh();

	event.Skip();
}