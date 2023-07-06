#include "Canvas.h"

#include "wx/graphics.h"
#include "wx/dcbuffer.h"

#include "SimProject.h"

Canvas::Canvas(wxWindow* parent, wxStatusBar* statusBar)
	: wxPanel(parent, wxID_ANY), m_nextID(0), m_elements(), m_nodes(&m_elements),
	m_edges(&m_elements), m_selection(), m_incompleteEdge()
{
	m_history = new History(100);

	m_gridSizes = { 1.0, 10.0, 100.0, 1000.0 };

	// UI
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	m_zoomLevel = 1;

	m_statusBarFields = 4;
	m_debugStatusBar = statusBar;
	m_debugStatusBar->SetFieldsCount(m_statusBarFields);

	m_nodeSubMenu = new wxMenu("Specific Nodes");
	m_nodeSubMenu->Append(ID_ADD_SOURCE, "Source", "Add a source node to the canvas");
	m_nodeSubMenu->Append(ID_ADD_SERVER, "Server", "Add a server node to the canvas");
	m_nodeSubMenu->Append(ID_ADD_SINK, "Sink", "Add a sink node to the canvas");
	m_nodeSubMenu->Bind(wxEVT_MENU, &Canvas::OnMenuAddSource, this, ID_ADD_SOURCE);
	m_nodeSubMenu->Bind(wxEVT_MENU, &Canvas::OnMenuAddServer, this, ID_ADD_SERVER);
	m_nodeSubMenu->Bind(wxEVT_MENU, &Canvas::OnMenuAddSink, this, ID_ADD_SINK);

	m_canvasMenu = new wxMenu("Canvas");
	m_canvasMenu->Append(ID_ADD_NODE, "Add node", "Add a node to the canvas");
	m_canvasMenu->Append(wxID_ANY, "Specific Nodes", m_nodeSubMenu);
	m_canvasMenu->Bind(wxEVT_MENU, &Canvas::OnMenuAddNode, this, ID_ADD_NODE);

	m_nodeMenu = new wxMenu("");
	m_nodeMenu->Append(ID_DELETE_NODE, "Delete", "Delete the currently selected node");
	m_nodeMenu->Bind(wxEVT_MENU, &Canvas::OnMenuDeleteNode, this, ID_DELETE_NODE);

	// Event bindings

	// Event for drawing to the canvas
	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

	// Refresh the canvas upon resizing
	this->Bind(wxEVT_SIZE, &Canvas::OnSize, this);

	// Capture mouse events
	this->Bind(wxEVT_MIDDLE_DOWN, &Canvas::OnMiddleDown, this);
	this->Bind(wxEVT_MIDDLE_UP, &Canvas::OnMiddleUp, this);
	this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnLeftDown, this);
	this->Bind(wxEVT_LEFT_UP, &Canvas::OnLeftUp, this);
	this->Bind(wxEVT_MOTION, &Canvas::OnMotion, this);
	this->Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouseWheel, this);
	this->Bind(wxEVT_RIGHT_UP, &Canvas::OnRightUp, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnLeaveWindow, this);
	this->Bind(wxEVT_ENTER_WINDOW, &Canvas::OnEnterWindow, this);

	// Capture key events
	this->Bind(wxEVT_CHAR_HOOK, &Canvas::OnCharHook, this);
	this->Bind(wxEVT_KEY_UP, &Canvas::OnDeleteKey, this);
}

Canvas::~Canvas() {
	m_canvasMenu->Destroy(ID_ADD_NODE);
	delete m_canvasMenu;

	m_nodeMenu->Destroy(ID_DELETE_NODE);
	delete m_nodeMenu;
}

void Canvas::AddNode(GraphicalNode* obj) {
	m_nodes.add_new(obj);
	m_gnodes.Add(obj);
}

// Adds a graphical node to the canvas
void Canvas::AddNode(GenericNode::Type type, wxPoint2DDouble center, const std::string& label) {
	
	// generate the specified graphical node based on the type given
	GraphicalNode* obj = NodeFactory::CreateGraphicalNode(type, m_nextID, this, center);
	m_nextID++;
	
	AddNode(obj);

	Refresh();
}

// Adds a graphical node to the canvas, name is auto populated with id number
void Canvas::AddNode(GenericNode::Type type, wxPoint2DDouble center) {

	// generate the specified graphical node based on the type given
	GraphicalNode* obj = NodeFactory::CreateGraphicalNode(type, m_nextID, this, center);
	m_nextID++;
	
	AddNode(obj);

	Refresh();
}

// Deletes the currently selected node
void Canvas::DeleteNode() {
	if (m_selection.state != Selection::State::NODE)
		return;

	// Edges are disconnected and deleted
	for (auto output : m_nodes[m_selection]->GetOutputs())
		m_edges.erase(output->GetID());

	for (auto input : m_nodes[m_selection]->GetInputs())
		m_edges.erase(input->GetID());

	// Node is then deleted
	m_nodes.erase(m_selection);
	m_myProject->RegisterNodeDeletion(m_nodes[m_selection]);

	Refresh();
}

// Returns the camera transform which is the scaling matrix multiplied by the translation matrix
wxAffineMatrix2D Canvas::GetCameraTransform() const {
	wxAffineMatrix2D cameraTransform = m_cameraZoom;
	cameraTransform.Concat(m_cameraPan);
	return cameraTransform;
}

wxPoint2DDouble Canvas::GetTransformedPoint(wxPoint2DDouble pointToTransform)
{
	// transform drawing location to local
	wxAffineMatrix2D cTransform = GetCameraTransform();
	cTransform.Invert();
	wxPoint2DDouble transformedPoint = cTransform.TransformPoint(pointToTransform);
	return transformedPoint;
}

void Canvas::DrawGrid(wxGraphicsContext* gc)
{
}

void Canvas::TransformOriginLocation(wxSize canvasSize)
{
	m_canvasSize = canvasSize;

	// set canvas coordinate system origin
	int width, height;
	GetClientSize(&width, &height);
	m_origin = wxPoint(width / 2, height / 2);
	m_cameraPan.Translate(m_origin.x, m_origin.y);
	m_zoomLevel = m_zoomLevel * 2.3;
	m_cameraZoom.Scale(m_zoomLevel, m_zoomLevel);

	// transform drawing location to local
	wxPoint2DDouble originPosition = GetTransformedPoint(m_origin);

	// draw a few basic nodes
	AddNode(GenericNode::SOURCE, wxPoint2DDouble(originPosition.m_x - 150, originPosition.m_y));
	AddNode(GenericNode::SERVER, wxPoint2DDouble(originPosition.m_x, originPosition.m_y));
	AddNode(GenericNode::SINK, wxPoint2DDouble(originPosition.m_x + 150, originPosition.m_y));
}

Set<GraphicalNode> Canvas::GetSimObjects()
{
	return m_gnodes;
}

std::vector<std::unique_ptr<GraphicalNode>> Canvas::GetUniqueNodes()
{
	std::vector<std::unique_ptr<GraphicalNode>> nodes;
	Set<GraphicalNode> canvasNodes = m_gnodes;

	while (!canvasNodes.IsEmpty()) {
		GraphicalNode* currentNode = canvasNodes.GetFirst();
		nodes.push_back(currentNode->Clone());
	}

	return nodes;
}

ElementKey Canvas::GetNextID()
{
	return m_nextID;
}

void Canvas::SetSimulationProject(SimProject* parentProject)
{
	m_myProject = parentProject;
}

// Return selection information containing which graphical node, if any, was selected and the state of the selection
Selection Canvas::Select(wxPoint2DDouble clickPosition) {

	if (m_elements.empty())
		return Selection();

	Selection selection;

	for (GraphicalElement* const& element : m_elements) {
		if (selection = element->Select(GetCameraTransform(), clickPosition)) 
			break;
	}

	m_debugStatusBar->SetStatusText("Selection State: " + GraphicalElement::ms_selectionStateNames[selection.state],
		DebugField::SELECTION_STATE);

	if (!selection) {
		m_debugStatusBar->SetStatusText("No object selected", DebugField::COMPONENT_SELECTED);
		return selection;
	}

	m_debugStatusBar->SetStatusText("Object Selected: " + selection->GetLabel(),
		DebugField::COMPONENT_SELECTED);

	return selection;
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
	inv.Concat(m_nodes[m_selection]->GetTransform());
	inv.Invert();
	dragVector = inv.TransformDistance(dragVector);

	m_nodes[m_selection]->Move(dragVector);

	m_previousMousePosition = clickPosition;

	Refresh();
}

// Event Handlers

// Called upon user selecting add node in popup canvas menu
// deprecated
void Canvas::OnMenuAddNode(wxCommandEvent& event) {
	auto inverse = GetCameraTransform();
	inverse.Invert();

	//AddNode(inverse.TransformPoint(m_previousMousePosition));
}

void Canvas::OnMenuAddSource(wxCommandEvent& event)
{
	AddNode(GenericNode::SOURCE, GetTransformedPoint(m_previousMousePosition));
}

void Canvas::OnMenuAddServer(wxCommandEvent& event)
{
	AddNode(GenericNode::SERVER, GetTransformedPoint(m_previousMousePosition));
}

void Canvas::OnMenuAddSink(wxCommandEvent& event)
{
	AddNode(GenericNode::SINK, GetTransformedPoint(m_previousMousePosition));
}

// Called upon user selecting delete node in popup node menu
void Canvas::OnMenuDeleteNode(wxCommandEvent& event) {
	DeleteNode();
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
// Panning also occurs when selection state is none
void Canvas::OnLeftDown(wxMouseEvent& event) {

	// selections and position
	m_selection = Select(event.GetPosition());
	m_previousMousePosition = event.GetPosition();

	// world to local coord transform
	wxPoint2DDouble transformedPos = GetTransformedPoint(m_previousMousePosition);

	m_debugStatusBar->SetStatusText("Zoom Level: " + std::to_string(m_zoomLevel), DebugField::ZOOM_LEVEL);
	m_debugStatusBar->SetStatusText("Mouse Position (" + std::to_string((int)transformedPos.m_x) + "," +
		std::to_string((int)transformedPos.m_y) + ")", DebugField::MOUSE_POSITION);

	switch (m_selection.state) {

	// Prepare to drag selected node
	case Selection::State::NODE:

		m_moveNodeAction = new MoveNodeAction(m_selection->GetID(), &m_nodes);
		m_moveNodeAction->SetPreviousPosition(m_nodes[m_selection]->GetPosition());
		break;

	// Instatiate an edge and connect source to node's output
	case Selection::State::NODE_OUTPUT:
		m_edges.add_new(new GraphicalEdge(m_nextID));
		m_nextID++;

		// Get pointer to edge that was just added
		m_incompleteEdge = m_edges.recent();
		m_incompleteEdge->ConnectSource(m_nodes[m_selection]);
		break;

	// Instatiate an edge and connect destination to node's input
	case Selection::State::NODE_INPUT:
		m_edges.add_new(new GraphicalEdge(m_nextID));
		m_nextID++;

		// Get pointer to edge that was just added
		m_incompleteEdge = m_edges.recent();
		m_incompleteEdge->ConnectDestination(m_nodes[m_selection]);
		break;

	// Panning also works with left click
	case Selection::State::NONE:
		m_isPanning = true;
		break;
	}

	Refresh();
}

// Capture release of left mouse button in order to stop dragging or finalize a connection
void Canvas::OnLeftUp(wxMouseEvent& event) {

	// Holds which node and selection state occurred upon user's left button up
	auto endSelection = Select(event.GetPosition());

	switch (m_selection.state) {

	// Finish move action
	case Selection::State::NODE:
		m_moveNodeAction->SetNextPosition(m_nodes[m_selection]->GetPosition());
		m_history->LogAction(m_moveNodeAction);

		// for showing properties window
		MainFrame::GetInstance()->RegisterNewSelection((GraphicalNode*)endSelection.element);
		break;

	// Check that user selected an input to pair with the output and then connect
	case Selection::State::NODE_OUTPUT:
		if (endSelection.state == Selection::State::NODE_INPUT
			&& m_nodes[endSelection] != m_nodes[m_selection]) {

			m_incompleteEdge->ConnectDestination(m_nodes[endSelection]);
			m_nodes[m_selection]->SetNext(m_nodes[endSelection]);
			m_nodes[endSelection]->SetPrevious(m_nodes[m_selection]);
			if (m_myProject->HasBeenBuilt()) {
				m_myProject->RegisterNewConnection(m_nodes[m_selection], m_nodes[endSelection]);
			}
			m_debugStatusBar->SetStatusText("Connected " + m_nodes[m_selection]->GetLabel() + " to "
				+ m_nodes[endSelection]->GetLabel(), DebugField::COMPONENTS_CONNECTED);
		}
		else
			m_edges.erase(m_incompleteEdge->GetID());

		break;

	// Check that user selected an output to pair with the input and then connect
	case Selection::State::NODE_INPUT:
		if (endSelection.state == Selection::State::NODE_OUTPUT
			&& m_nodes[endSelection] != m_nodes[m_selection]) {

			m_incompleteEdge->ConnectSource(m_nodes[endSelection]);
			m_nodes[m_selection]->SetNext(m_nodes[endSelection]);
			m_nodes[endSelection]->SetPrevious(m_nodes[m_selection]);
			if (m_myProject->HasBeenBuilt()) {
				m_myProject->RegisterNewConnection(m_nodes[m_selection], m_nodes[endSelection]);
			}

			m_debugStatusBar->SetStatusText("Connected " + m_nodes[endSelection]->GetLabel() + " to "
				+ m_nodes[m_selection]->GetLabel(), DebugField::COMPONENTS_CONNECTED);
		}
		else
			m_edges.erase(m_incompleteEdge->GetID());

		break;

	// User is no longer panning camera
	case Selection::State::NONE:
		m_isPanning = false;
	}

	m_selection.reset();
	m_incompleteEdge = nullptr;

	Refresh();
}

// Capture mouse movement for panning and moving graphical nodes
void Canvas::OnMotion(wxMouseEvent& event) {
	wxPoint2DDouble mousePosition = event.GetPosition();

	if (m_isPanning && (event.ButtonIsDown(wxMOUSE_BTN_LEFT)
		|| event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))) {

		PanCamera(mousePosition);
	}
	else if (!m_selection)
		return;

	switch (m_selection.state) {

	// Move node
	case Selection::State::NODE:
		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			MoveNode(mousePosition);
		break;

	// Move destination point of edge
	case Selection::State::NODE_OUTPUT:

		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			m_incompleteEdge->SetDestinationPoint(GetTransformedPoint(mousePosition));
		else if (m_incompleteEdge) {
			m_incompleteEdge->Disconnect();
			m_edges.erase(m_incompleteEdge->GetID());
			m_incompleteEdge = nullptr;
		}

		Refresh();
		break;

	// Move source point of edge
	case Selection::State::NODE_INPUT:

		if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			m_incompleteEdge->SetSourcePoint(GetTransformedPoint(mousePosition));
		else if (m_incompleteEdge) {
			m_incompleteEdge->Disconnect();
			m_edges.erase(m_incompleteEdge->GetID());
			m_incompleteEdge = nullptr;
		}

		Refresh();
		break;
	}
}

// Capture mouse scrolling in order to zoom the camera
void Canvas::OnMouseWheel(wxMouseEvent& event) {

	// Convert mouse position from screen to local coordinates
	wxPoint2DDouble mousePosition = event.GetPosition();
	wxPoint2DDouble localMousePosition = GetTransformedPoint(mousePosition);

	// Determine the zoom scale factor
	double scaleFactor = pow(2, 0.1 * event.GetWheelRotation() / event.GetWheelDelta());

	// Adjust the zoom and translation of the camera
	m_cameraZoom.Scale(scaleFactor, scaleFactor);
	m_zoomLevel = m_zoomLevel * scaleFactor;
	m_cameraPan.Translate((1 - scaleFactor) * (localMousePosition.m_x), (1 - scaleFactor) * (localMousePosition.m_y));

	Refresh();
}

// Capture release of right mouse button in order to present user with a popup menu
void Canvas::OnRightUp(wxMouseEvent& event) {
	m_selection = Select(event.GetPosition());

	if (m_incompleteEdge) {
		m_edges.erase(m_incompleteEdge->GetID());
		m_incompleteEdge = nullptr;
	}

	switch (m_selection.state) {

	// Popup node menu options
	case Selection::State::NODE:
		m_nodeMenu->SetTitle(m_selection->GetLabel());
		PopupMenu(m_nodeMenu);
		break;

	// Popup I/O menu options and bind output edge disconnection handler
	case Selection::State::NODE_OUTPUT:
		wxLogMessage("Right clicked output of %s", m_selection->GetLabel());
		break;

	// Popup I/O menu options and bind input edge disconnection handler
	case Selection::State::NODE_INPUT:
		wxLogMessage("Right clicked input of %s", m_selection->GetLabel());
		break;

	// Popup canvas menu options
	case Selection::State::NONE:
		m_previousMousePosition = event.GetPosition();
		PopupMenu(m_canvasMenu);
		break;
	}
}

// Unable to track mouse position outside of window; therefore, panning and dragging is disabled when the mouse leaves
void Canvas::OnLeaveWindow(wxMouseEvent& event) {
	if (m_selection.state == Selection::State::NODE) {
		m_moveNodeAction->SetNextPosition(m_nodes[m_selection]->GetPosition());
		m_history->LogAction(m_moveNodeAction);
	}

	Refresh();
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

	//int gridSize = m_canvasSize.GetHeight() / 50;
	//int width = m_canvasSize.GetWidth() / 2;
	//int height = m_canvasSize.GetHeight() / 2;

	//// x grid lines
	//for (wxPoint x = wxPoint(-width, 0); x.x < m_origin.x + width; x.x += gridSize) {
	//	//auto transformedX = cTransform.TransformPoint(x);
	//	dc.DrawLine(x.x, height + m_origin.y, x.x, -height + m_origin.y);
	//}
	//
	//// y grid lines
	//for (wxPoint y = wxPoint(-height, 0); y.y < m_origin.x + height; y.y += gridSize) {
	//	dc.DrawLine(width + m_origin.x, y.y, -width  + m_origin.x, y.y);
	//}

	//for (auto gridSize = m_gridSizes.rbegin(); gridSize != m_gridSizes.rend(); gridSize++) {
	//	double distanceOnScreen = (*gridSize) * m_zoomLevel;
	//	if (distanceOnScreen < 10.0) // Minimum pixel distance between grid lines
	//		break;
	//}

	for (GraphicalElement* const& element : m_elements)
		element->Draw(GetCameraTransform(), gc);

	delete gc;
}

// Refresh, i.e. redraw the window, upon resizing
void Canvas::OnSize(wxSizeEvent& event) {
	Refresh();
}

void Canvas::OnCharHook(wxKeyEvent& event) {

	if (event.ControlDown()) {
		switch (event.GetKeyCode()) {
		case 'Y':
			m_history->Redo();
			break;
		case 'Z':
			m_history->Undo();
			break;
		}
	}

	Refresh();
}

void Canvas::OnDeleteKey(wxKeyEvent& event)
{
	switch (event.GetKeyCode()) {
	case WXK_DELETE:

		DeleteNode();
		break;
	}
}
