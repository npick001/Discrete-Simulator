#include "GraphicalEdge.h"

#include "Canvas.h"

unsigned int GraphicalEdge::ms_nextID = 1;

GraphicalEdge::GraphicalEdge() {
	m_id = ms_nextID;
	ms_nextID++;

	m_source = nullptr;
	m_destination = nullptr;

	m_sourcePoint = wxPoint2DDouble();
	m_destinationPoint = wxPoint2DDouble();
}

GraphicalEdge::GraphicalEdge(GraphicalNode* source, GraphicalNode* destination) {
	m_id = ms_nextID;
	ms_nextID++;

	ConnectSource(source);
	ConnectDestination(destination);
}

GraphicalEdge::~GraphicalEdge() {
	Disconnect();
}

void GraphicalEdge::ConnectSource(GraphicalNode* source) {
	if (!source)
		return;
	
	m_source = source;
	m_sourcePoint = source->GetOutputPoint();
	m_source->SetOutputEdge(this);

	if (m_destination)
		return;

	m_destinationPoint = m_sourcePoint;
}

void GraphicalEdge::ConnectDestination(GraphicalNode* destination) {
	if (!destination)
		return;
	
	m_destination = destination;
	m_destinationPoint = destination->GetInputPoint();
	m_destination->SetInputEdge(this);

	if (m_source)
		return;

	m_sourcePoint = m_destinationPoint;
}

void GraphicalEdge::Disconnect() {
	if (m_source) {
		m_source->m_outputEdge = nullptr;
		m_source = nullptr;
	}

	if (m_destination) {
		m_destination->m_inputEdge = nullptr;
		m_destination = nullptr;
	}
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