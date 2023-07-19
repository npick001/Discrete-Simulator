#include "GraphicalEdge.h"

const wxColor GraphicalEdge::ms_labelColor = wxColor(64, 64, 64);

GraphicalEdge::GraphicalEdge() : GraphicalElement(), m_source(), m_destination() {}

GraphicalEdge::GraphicalEdge(ElementKey id)
	: GraphicalElement(id), m_sourcePoint(), m_destinationPoint() {

	m_label = "Edge " + std::to_string(m_id);

	m_source = nullptr;
	m_destination = nullptr;

	m_sourceID = -1;
	m_destinationID = -1;
}

GraphicalEdge::GraphicalEdge(ElementKey id, GraphicalNode* source, GraphicalNode* destination)
	: GraphicalEdge(id) {

	SetSource(source);
	SetDestination(destination);

	m_sourceID = source->GetID();
	m_destinationID = destination->GetID();
}

GraphicalEdge::GraphicalEdge(const GraphicalEdge& other) {
	
	GraphicalElement::operator=(other);

	m_source = other.m_source;
	m_destination = other.m_destination;

	m_sourceID = other.m_sourceID;
	m_destinationID = other.m_destinationID;

	m_sourcePoint = other.m_sourcePoint;
	m_destinationPoint = other.m_destinationPoint;
}

std::unique_ptr<GraphicalEdge> GraphicalEdge::Clone()
{
	return std::make_unique<GraphicalEdge>(*this);
}

GraphicalEdge& GraphicalEdge::operator=(const GraphicalEdge& other) {
	if (this == &other)
		return (*this);

	GraphicalElement::operator=(other);

	m_source = other.m_source;
	m_destination = other.m_destination;

	m_sourceID = other.m_sourceID;
	m_destinationID = other.m_destinationID;

	m_sourcePoint = other.m_sourcePoint;
	m_destinationPoint = other.m_destinationPoint;

	return (*this);
}

GraphicalEdge::~GraphicalEdge() {
	Disconnect();
}

void GraphicalEdge::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

void GraphicalEdge::SetSource(GraphicalNode* source) {
	if (!source)
		return;
	
	m_source = source;
	m_sourceID = source->GetID();
	m_sourcePoint = source->GetOutputPoint();
	m_source->m_outputs.push_back(this);

	if (m_destination)
		return;

	m_destinationPoint = m_sourcePoint;
}

GraphicalNode* GraphicalEdge::GetSource()
{
	return m_source;
}

ElementKey GraphicalEdge::GetSourceID()
{
	return m_sourceID;
}

void GraphicalEdge::SetSourceID(ElementKey id)
{
	m_sourceID = id;
}

void GraphicalEdge::SetDestination(GraphicalNode* destination) {
	if (!destination)
		return;
	
	m_destination = destination;
	m_destinationID = destination->GetID();
	m_destinationPoint = destination->GetInputPoint();
	m_destination->m_inputs.push_back(this);

	if (m_source)
		return;

	m_sourcePoint = m_destinationPoint;
}

GraphicalNode* GraphicalEdge::GetDestination()
{
	return m_destination;
}

ElementKey GraphicalEdge::GetDestinationID()
{
	return m_destinationID;
}

void GraphicalEdge::SetDestinationID(ElementKey id)
{
	m_destinationID = id;
}

void GraphicalEdge::Disconnect() {
	if (m_source) {
		m_source->m_outputs.remove(this);
		m_source = nullptr;
	}

	if (m_destination) {
		m_destination->m_inputs.remove(this);
		m_destination = nullptr;
	}
}

// Draws the edge to a wxGraphicsContext
void GraphicalEdge::Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) {

	// Transform from world to window coordinates
	gc->SetTransform(gc->CreateMatrix(camera));
	gc->SetPen(wxPen(*wxBLACK, 2));

	auto path = gc->CreatePath();
	path.MoveToPoint(m_sourcePoint);
	path.AddLineToPoint(m_destinationPoint);
	path.CloseSubpath();
	gc->StrokePath(path);

	gc->SetFont(*wxNORMAL_FONT, ms_labelColor);

	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);

	gc->DrawText(m_label, m_sourcePoint.m_x + (m_destinationPoint.m_x - m_sourcePoint.m_x) / 2 - textWidth / 2,
		m_sourcePoint.m_y + (m_destinationPoint.m_y - m_sourcePoint.m_y) / 2 - textHeight);
}

Selection GraphicalEdge::Select
	(const wxAffineMatrix2D& camera, wxPoint2DDouble clickPosition) {

	return { nullptr, Selection::State::NONE };
}