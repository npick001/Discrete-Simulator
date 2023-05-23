#include "GraphicalNode.h"

#include "wx/graphics.h"
#include "wx/dcbuffer.h"

#include "Canvas.h"

// Used for outputting debug information to the debug status bar
const std::string GraphicalNode::ms_selectionStateNames[GraphicalNode::SelectionState::STATES_MAX] = {
	"NONE",
	"COMPONENT",
	"OUTPUT",
	"INPUT",
};

// Default component dimensions and colors
// High pixel density displays are accounted for in the GraphicalNode constructor
const wxSize GraphicalNode::ms_bodySize = wxSize(100, 75);
const wxColor GraphicalNode::ms_bodyColor = wxColor(64, 64, 64);

const wxSize GraphicalNode::ms_ioSize = wxSize(15, 15);
const wxColor GraphicalNode::ms_ioColor = wxColor(128, 128, 128);

const wxColor GraphicalNode::ms_textColor = *wxWHITE;

unsigned int GraphicalNode::ms_nextID = 1;

GraphicalNode::GraphicalNode(wxWindow* parent, wxPoint2DDouble center, const std::string& text) {

	m_id = ms_nextID;
	ms_nextID++;

	wxSize bodySize = parent->FromDIP(ms_bodySize);
	wxSize ioSize = parent->FromDIP(ms_ioSize);

	m_rect = wxRect2DDouble(-bodySize.GetWidth() / 2, -bodySize.GetHeight() / 2, bodySize.GetWidth(), bodySize.GetHeight());
	m_text = text;
	m_transform.Translate(center.m_x, center.m_y);

	m_inputRect = wxRect2DDouble(-m_rect.m_width / 2 - ioSize.GetWidth() / 2, -ioSize.GetHeight() / 2, ioSize.GetWidth(), ioSize.GetHeight());
	m_outputRect = wxRect2DDouble(m_rect.m_width / 2 - ioSize.GetWidth() / 2, -ioSize.GetHeight() / 2, ioSize.GetWidth(), ioSize.GetHeight());
}

GraphicalNode::GraphicalNode(wxWindow* parent, wxPoint2DDouble center)
	: GraphicalNode(parent, center, "Node " + std::to_string(ms_nextID)) {}

GraphicalNode::~GraphicalNode() {
	DisconnectInput();
	DisconnectOutput();
}

wxPoint2DDouble GraphicalNode::GetOutputPoint() const {
	wxPoint2DDouble outputPoint = {
		m_outputRect.m_x + m_outputRect.m_width / 2,
		m_outputRect.m_y + m_outputRect.m_height / 2
	};

	return m_transform.TransformPoint(outputPoint);
}

wxPoint2DDouble GraphicalNode::GetInputPoint() const {
	wxPoint2DDouble inputPoint = {
		m_inputRect.m_x + m_inputRect.m_width / 2,
		m_inputRect.m_y + m_inputRect.m_height / 2
	};

	return m_transform.TransformPoint(inputPoint);
}

void GraphicalNode::SetOutputEdge(GraphicalEdge* outputEdge) {
	m_outputEdge = outputEdge;
}

void GraphicalNode::SetInputEdge(GraphicalEdge* inputEdge) {
	m_inputEdge = inputEdge;
}

void GraphicalNode::DisconnectOutput() {
	if (m_outputEdge)
		m_outputEdge->Disconnect();
}

void GraphicalNode::DisconnectInput() {
	if (m_inputEdge)
		m_inputEdge->Disconnect();
}

// Draws the node to a wxGraphicsContext
void GraphicalNode::Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const {

	// Transform coordinates according to camera and node transforms
	wxAffineMatrix2D nodeCameraTransform = camera;
	nodeCameraTransform.Concat(m_transform);
	gc->SetTransform(gc->CreateMatrix(nodeCameraTransform));

	// Draw component, input, output, and text
	gc->SetBrush(wxBrush(ms_bodyColor));
	gc->DrawRectangle(m_rect.m_x, m_rect.m_y, m_rect.m_width, m_rect.m_height);

	gc->SetBrush(wxBrush(ms_ioColor));
	gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	gc->SetFont(*wxNORMAL_FONT, ms_textColor);

	double textWidth, textHeight;
	gc->GetTextExtent(m_text, &textWidth, &textHeight);

	gc->DrawText(m_text, m_rect.m_x + m_rect.m_width / 2 - textWidth / 2, m_rect.m_y + m_rect.m_height / 2 - textHeight);
}

// Returns the selection state of the component given where the user clicked
GraphicalNode::SelectionState GraphicalNode::GetSelectionState(wxAffineMatrix2D cameraTransform, wxPoint2DDouble clickPosition) const {

	// Transform click position from window coordinates to node's local coordinates
	auto inverse = cameraTransform;
	inverse.Concat(m_transform);
	inverse.Invert();
	clickPosition = inverse.TransformPoint(clickPosition);

	// Return selection state according to what user clicked on
	if (m_inputRect.Contains(clickPosition))
		return SelectionState::INPUT;
	else if (m_outputRect.Contains(clickPosition))
		return SelectionState::OUTPUT;
	else if (m_rect.Contains(clickPosition))
		return SelectionState::NODE;
	else
		return SelectionState::NONE;
}

void GraphicalNode::Move(wxPoint2DDouble displacement) {
	m_transform.Translate(displacement.m_x, displacement.m_y);
	
	if (m_inputEdge)
		m_inputEdge->m_destinationPoint = GetInputPoint();

	if (m_outputEdge)
		m_outputEdge->m_sourcePoint = GetOutputPoint();
}