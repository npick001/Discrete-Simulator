#include "GraphicalEdge.h"

GraphicalEdge::GraphicalEdge() {
	m_source = nullptr;
	m_destination = nullptr;

	m_sourcePoint = wxPoint2DDouble();
	m_destinationPoint = wxPoint2DDouble();
}

GraphicalEdge::GraphicalEdge(GraphicalNode* source, GraphicalNode* destination) {
	SetSource(source);
	SetDestination(destination);
}

void GraphicalEdge::SetSource(GraphicalNode* source) {
	m_source = source;

	if (!m_source)
		return;

	m_sourcePoint = source->GetOutputPoint();
	m_source->SetOutputEdge(this);
}

void GraphicalEdge::SetDestination(GraphicalNode* destination) {
	m_destination = destination;

	if (!m_destination)
		return;
	
	m_destinationPoint = destination->GetInputPoint();
	m_destination->SetInputEdge(this);
}

void GraphicalEdge::RemoveSource() {
	m_source->SetOutputEdge(nullptr);
	m_source = nullptr;
}

void GraphicalEdge::RemoveDestination() {
	m_destination->SetInputEdge(nullptr);
	m_destination = nullptr;
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