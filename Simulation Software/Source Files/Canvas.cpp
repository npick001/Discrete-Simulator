#include "Canvas.h"

Canvas::Canvas(wxWindow* parent) : wxScrolledWindow(parent, wxID_ANY) {
	SetBackgroundColour(*wxWHITE);
	SetScrollRate(10, 10);
	// Create and associate a drop target with the canvas
	//wxDropTarget* dropTarget = new wxTextDropTarget(this);
	//SetDropTarget(dropTarget);

	//// Handle drop events
	//Bind(wxEVT_TEXT_DROP, &NodeCanvas::OnDrop, this);

	//_dropTarget = new NodeDropTarget(this);
	//SetDropTarget(_dropTarget);

	// bind the events
	this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnLeftClickDown, this, ID_LeftClickDown);
	this->Bind(wxEVT_LEFT_UP, &Canvas::OnLeftClickUp, this, ID_LeftClickUp);
}

Canvas::~Canvas() {
	_nodeInstances.~vector();
}

void Canvas::AddNode(GenericNode* node, int x, int y) {
	
	_nodeInstances.push_back(node);
	Refresh();
}


void Canvas::OnLeftClickDown(wxMouseEvent& event) {
	wxLogMessage("Left mouse button down.");
	//std::cout << "Canvas left click down." << std::endl;
}

void Canvas::OnLeftClickUp(wxMouseEvent& event) {
	wxLogMessage("Left mouse button up.");
}
