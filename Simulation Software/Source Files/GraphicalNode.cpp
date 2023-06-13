#include "GraphicalNode.h"

#include "wx/graphics.h"

// Default component dimensions and colors
// High pixel density displays are accounted for in the GraphicalNode constructor
const wxSize GraphicalNode::ms_bodySize = wxSize(100, 75);

const wxSize GraphicalNode::ms_ioSize = wxSize(15, 15);
const wxColor GraphicalNode::ms_ioColor = wxColor(128, 128, 128);

const wxColor GraphicalNode::ms_labelColor = *wxWHITE;

GraphicalNode::GraphicalNode() : GraphicalElement(), m_inputs(), m_outputs() {}

GraphicalNode::GraphicalNode(ElementKey id) : GraphicalElement(id), m_inputs(), m_outputs() {
	m_label = "Node " + std::to_string(m_id);
}

GraphicalNode::GraphicalNode(ElementKey id, wxWindow* parent, wxPoint2DDouble center)
	: GraphicalNode(id) {
	
	wxSize bodySize = parent->FromDIP(ms_bodySize);
	wxSize ioSize = parent->FromDIP(ms_ioSize);

	m_rect = wxRect2DDouble(-bodySize.GetWidth() / 2, -bodySize.GetHeight() / 2, bodySize.GetWidth(), bodySize.GetHeight());
	m_position = center;

	m_inputRect = wxRect2DDouble(-m_rect.m_width / 2 - ioSize.GetWidth() / 2, -ioSize.GetHeight() / 2, ioSize.GetWidth(), ioSize.GetHeight());
	m_outputRect = wxRect2DDouble(m_rect.m_width / 2 - ioSize.GetWidth() / 2, -ioSize.GetHeight() / 2, ioSize.GetWidth(), ioSize.GetHeight());
}

GraphicalNode::GraphicalNode(ElementKey id, wxWindow* parent, wxPoint2DDouble center, const std::string& label)
	: GraphicalNode(id, parent, center) {

	m_label = label;
}

GraphicalNode::GraphicalNode(const GraphicalNode& other) : GraphicalElement(other) {
	(*this) = other;
}

GraphicalNode& GraphicalNode::operator=(const GraphicalNode& other) {
	if (this == &other)
		return (*this);

	GraphicalElement::operator=(other);

	m_rect = other.m_rect;
	m_outputRect = other.m_outputRect;
	m_inputRect = other.m_inputRect;

	m_position = other.m_position;
	m_outputs = other.m_outputs;
	m_inputs = other.m_inputs;

	return (*this);
}

GraphicalNode::~GraphicalNode() {
	DisconnectInputs();
	DisconnectOutputs();
}

wxAffineMatrix2D GraphicalNode::GetTransform() const {
	wxAffineMatrix2D transform;
	transform.Translate(m_position.m_x, m_position.m_y);
	return transform;
}

wxPoint2DDouble GraphicalNode::GetOutputPoint() const {
	wxPoint2DDouble outputPoint = {
		m_outputRect.m_x + m_outputRect.m_width / 2,
		m_outputRect.m_y + m_outputRect.m_height / 2
	};

	return GetTransform().TransformPoint(outputPoint);
}

wxPoint2DDouble GraphicalNode::GetInputPoint() const {
	wxPoint2DDouble inputPoint = {
		m_inputRect.m_x + m_inputRect.m_width / 2,
		m_inputRect.m_y + m_inputRect.m_height / 2
	};

	return GetTransform().TransformPoint(inputPoint);
}

void GraphicalNode::DisconnectOutputs() {
	for (auto output : m_outputs)
		output->Disconnect();
}

void GraphicalNode::DisconnectInputs() {
	for (auto input : m_inputs)
		input->Disconnect();
}

// Draws the node to a wxGraphicsContext
void GraphicalNode::Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) const {

	// Transform coordinates according to camera and node transforms
	wxAffineMatrix2D localToWindow = camera;
	localToWindow.Concat(GetTransform());
	gc->SetTransform(gc->CreateMatrix(localToWindow));

	gc->SetPen(*wxTRANSPARENT_PEN);

	gc->SetBrush(wxBrush(m_bodyColor));
	gc->DrawRectangle(m_rect.m_x, m_rect.m_y, m_rect.m_width, m_rect.m_height);

	gc->SetBrush(wxBrush(ms_ioColor));
	gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	gc->SetFont(*wxNORMAL_FONT, ms_labelColor);

	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);

	gc->DrawText(m_label, m_rect.m_x + m_rect.m_width / 2 - textWidth / 2, m_rect.m_y + m_rect.m_height / 2 - textHeight);
}

// Returns the selection state of the component given where the user clicked
Selection GraphicalNode::Select(const wxAffineMatrix2D& camera, wxPoint2DDouble clickPosition) {

	// Transform click position from window coordinates to node's local coordinates
	auto windowToLocal = camera;
	windowToLocal.Concat(GetTransform());
	windowToLocal.Invert();
	clickPosition = windowToLocal.TransformPoint(clickPosition);

	// Return selection state according to what user clicked on
	if (m_inputRect.Contains(clickPosition))
		return { this, Selection::State::NODE_INPUT };
	else if (m_outputRect.Contains(clickPosition))
		return { this, Selection::State::NODE_OUTPUT };
	else if (m_rect.Contains(clickPosition))
		return { this, Selection::State::NODE };
	else
		return { nullptr, Selection::State::NONE };
}

void GraphicalNode::Move(wxPoint2DDouble displacement) {
	m_position += displacement;
	
	for (auto output : m_outputs)
		output->m_sourcePoint = GetOutputPoint();

	for (auto input : m_inputs)
		input->m_destinationPoint = GetInputPoint();
}

// GraphicalSource

const wxColor GraphicalSource::ms_bodyColor = wxColor(200, 100, 100);

GraphicalSource::GraphicalSource() : GraphicalNode() {}

GraphicalSource::GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center)
{
	m_bodyColor = ms_bodyColor;
}

// GraphicalSink

const wxColor GraphicalSink::ms_bodyColor = wxColor(100, 100, 200);

GraphicalSink::GraphicalSink() : GraphicalNode() {}

GraphicalSink::GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center)
{
	m_bodyColor = ms_bodyColor;
}