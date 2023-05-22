#include "GraphicalEdge.h"

GraphicalEdge::GraphicalEdge() {
	m_sourcePoint = wxPoint2DDouble();
	m_destinationPoint = wxPoint2DDouble();
}

GraphicalEdge::GraphicalEdge(GraphicalNode* source, GraphicalNode* destination) {
	SetSource(source);
	SetDestination(destination);
}

void GraphicalEdge::SetSource(GraphicalNode* source) {
	m_source = source;
	m_sourcePoint = source->GetOutputPoint();

	source->SetOutputEdge(this);
}

void GraphicalEdge::SetDestination(GraphicalNode* destination) {
	m_destination = destination;
	m_destinationPoint = destination->GetInputPoint();

	destination->SetInputEdge(this);
}

// Draws the edge to a wxGraphicsContext
void GraphicalEdge::Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const {

	// Transform from world to window coordinates
	gc->SetTransform(gc->CreateMatrix(camera));
	gc->SetPen(wxPen(*wxBLACK));

	auto path = gc->CreatePath();
	path.MoveToPoint(m_sourcePoint);
	path.AddLineToPoint(m_destinationPoint);
	path.CloseSubpath();
	gc->StrokePath(path);
}