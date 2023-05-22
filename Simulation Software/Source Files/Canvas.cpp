#include "Canvas.h"

#include "wx/graphics.h"
#include "wx/dcbuffer.h"

Canvas::Canvas(wxWindow* parent, wxStatusBar* statusBar)
	: wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_debugStatusBar = statusBar;
	m_debugStatusBar->SetFieldsCount(DebugField::FIELDS_MAX);

	AddGraphicalNode(FromDIP(wxPoint(-150, 0)), "Component 1");
	AddGraphicalNode(FromDIP(wxPoint(   0, 0)), "Component 2");
	AddGraphicalNode(FromDIP(wxPoint( 150, 0)), "Component 3");

	wxSize size = parent->GetSize();
	m_cameraZoom.Translate(size.GetWidth() / 2, size.GetHeight() / 2);

	m_nodeSelectionState = GraphicalNode::SelectionState::SELECT_NONE;

	// Event bindings
	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &Canvas::OnSize, this);
	this->Bind(wxEVT_MIDDLE_DOWN, &Canvas::OnMiddleDown, this);
	this->Bind(wxEVT_MIDDLE_UP, &Canvas::OnMiddleUp, this);
	this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnLeftDown, this);
	this->Bind(wxEVT_LEFT_UP, &Canvas::OnLeftUp, this);
	this->Bind(wxEVT_MOTION, &Canvas::OnMotion, this);
	this->Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouseWheel, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnLeaveWindow, this);
}

// Adds a graphical node to the canvas
void Canvas::AddGraphicalNode(wxPoint center, const std::string& text) {
	GraphicalNode obj(this, center, text);
	m_nodes.push_back(obj);

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
	GraphicalNode::SelectionState state;

	auto selectedComponentIter = std::find_if(m_nodes.rbegin(), m_nodes.rend(),
		[this, &state, clickPosition](const GraphicalNode& graphicalNode) mutable {
		state = graphicalNode.GetSelectionState(GetCameraTransform(), clickPosition);
		return (state != GraphicalNode::SelectionState::SELECT_NONE);
	});

	m_debugStatusBar->SetStatusText("Selection State: " + GraphicalNode::ms_selectionStateNames[state],
		DebugField::SELECTION_STATE);

	if (selectedComponentIter == m_nodes.rend())
		return { nullptr, GraphicalNode::SelectionState::SELECT_NONE };

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
void Canvas::MoveComponent(wxPoint2DDouble clickPosition) {
	auto dragVector = clickPosition - m_previousMousePosition;

	auto inv = GetCameraTransform();
	inv.Concat(m_selectedNode->GetTransform());
	inv.Invert();
	dragVector = inv.TransformDistance(dragVector);

	m_selectedNode->Move(dragVector);

	m_previousMousePosition = clickPosition;

	Refresh();
}

// Capture when user holds down middle mouse button in order to pan
void Canvas::OnMiddleDown(wxMouseEvent& event) {
	m_isPanning = true;
	m_previousMousePosition = event.GetPosition();
}

// Capture release of middle mouse button in order to stop panning
void Canvas::OnMiddleUp(wxMouseEvent& event) {
	m_isPanning = false;
}

// Capture when user holds down left mouse button in order to drag or connect nodes
void Canvas::OnLeftDown(wxMouseEvent& event) {
	SelectionInfo selection = GetNodeSelectionInfo(event.GetPosition());
	m_selectedNode = selection.node;
	m_nodeSelectionState = selection.state;

	// Defined before switch statement to avoid redefinition
	GraphicalEdge edge;

	switch (m_nodeSelectionState) {
	case GraphicalNode::SelectionState::SELECT_NODE:
		m_previousMousePosition = event.GetPosition();
		break;
	case GraphicalNode::SelectionState::SELECT_OUTPUT:
		edge.SetSource(m_selectedNode);
		m_edges.push_back(edge);

		m_incompleteEdge = &(*m_edges.rbegin());
		break;
	case GraphicalNode::SelectionState::SELECT_INPUT:
		edge.SetDestination(m_selectedNode);
		m_edges.push_back(edge);

		m_incompleteEdge = &(*m_edges.rbegin());
		break;
	}

	Refresh();
}

// Capture release of left mouse button in order to stop dragging or finalize a connection
void Canvas::OnLeftUp(wxMouseEvent& event) {

	// Holds which node and selection state occurred upon user's left button up
	auto endSelection = GetNodeSelectionInfo(event.GetPosition());

	switch (m_nodeSelectionState) {
	case GraphicalNode::SelectionState::SELECT_INPUT:
		
		// Inputs can only connect to outputs
		if (endSelection.state == GraphicalNode::SelectionState::SELECT_OUTPUT) {
			m_incompleteEdge->SetSource(endSelection.node);

			m_debugStatusBar->SetStatusText("Connected " + endSelection.node->GetText() + " to "
				+ m_selectedNode->GetText(), DebugField::COMPONENTS_CONNECTED);
		}
		break;
	case GraphicalNode::SelectionState::SELECT_OUTPUT:

		// Outputs can only connect to inputs
		if (endSelection.state == GraphicalNode::SelectionState::SELECT_INPUT) {
			m_incompleteEdge->SetDestination(endSelection.node);

			m_debugStatusBar->SetStatusText("Connected " + m_selectedNode->GetText() + " to "
				+ endSelection.node->GetText(), DebugField::COMPONENTS_CONNECTED);
		}
	}

	m_selectedNode = nullptr;
	m_incompleteEdge = nullptr;
	m_nodeSelectionState = GraphicalNode::SelectionState::SELECT_NONE;

	Refresh();
}

// Capture mouse movement for panning and moving graphical nodes
void Canvas::OnMotion(wxMouseEvent& event) {
	wxPoint2DDouble mousePosition = event.GetPosition();

	if (m_isPanning) {
		PanCamera(mousePosition);
	}
	else if (!m_selectedNode)
		return;

	// Defined before switch statement to avoid redefinition
	// Used to convert from window to world coordinates
	wxAffineMatrix2D inverse = GetCameraTransform();
	inverse.Invert();

	switch (m_nodeSelectionState) {
	case GraphicalNode::SelectionState::SELECT_NODE:
		MoveComponent(mousePosition);
		break;
	case GraphicalNode::SelectionState::SELECT_OUTPUT:
		m_incompleteEdge->SetDestinationPoint(inverse.TransformPoint(mousePosition));
		Refresh();
		break;
	case GraphicalNode::SelectionState::SELECT_INPUT:
		m_incompleteEdge->SetSourcePoint(inverse.TransformPoint(mousePosition));
		Refresh();
		break;
	}
}

// Capture mouse scrolling in order to zoom the camera
void Canvas::OnMouseWheel(wxMouseEvent& event) {
	double scaleFactor = pow(2, 0.1 * event.GetWheelRotation() / event.GetWheelDelta());
	m_cameraZoom.Scale(scaleFactor, scaleFactor);

	Refresh();
}

// Unable to track mouse position outside of window; therefore, panning and dragging is disabled when the mouse leaves
void Canvas::OnLeaveWindow(wxMouseEvent& event) {
	m_selectedNode = nullptr;
	m_nodeSelectionState = GraphicalNode::SelectionState::SELECT_NONE;
	m_isPanning = false;
}

// Custom graphical elements such as graphical nodes and connections are drawn here
void Canvas::OnPaint(wxPaintEvent& event) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (!gc)
		return;

	for (const GraphicalNode& node : m_nodes) {
		node.Draw(GetCameraTransform(), gc);

		// DEBUG code
		if (node.m_outputEdge) {
			wxPoint2DDouble point1 = node.m_outputEdge->GetSourcePoint();
			wxPoint2DDouble point2 = node.m_outputEdge->GetDestinationPoint();
			dc.DrawLine(wxPoint(point1.m_x, point1.m_y), wxPoint(point2.m_x, point2.m_y));
		}
	}

	for (const GraphicalEdge& edge : m_edges) {
		edge.Draw(GetCameraTransform(), gc);
	}

	delete gc;
}

// Refresh, i.e. redraw the window, upon resizing
void Canvas::OnSize(wxSizeEvent& event) {
	Refresh();
}