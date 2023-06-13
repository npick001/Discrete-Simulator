#include "GraphicalNode.h"

#include "wx/graphics.h"

<<<<<<< Updated upstream
GraphicalElement::Type GraphicalNode::ms_type = GraphicalElement::NODE;

// Default component dimensions and colors
// High pixel density displays are accounted for in the GraphicalNode constructor
const wxSize GraphicalNode::ms_bodySize = wxSize(100, 75);
const wxColor GraphicalNode::ms_bodyColor = wxColor(64, 64, 64);

const wxSize GraphicalNode::ms_ioSize = wxSize(15, 15);
const wxColor GraphicalNode::ms_ioColor = wxColor(128, 128, 128);

const wxColor GraphicalNode::ms_labelColor = *wxWHITE;

=======
>>>>>>> Stashed changes
GraphicalNode::GraphicalNode() : GraphicalElement(), m_inputs(), m_outputs() {}

GraphicalNode::GraphicalNode(ElementKey id) : GraphicalElement(id), m_inputs(), m_outputs() {
	m_label = "Node " + std::to_string(m_id);
}

GraphicalNode::GraphicalNode(ElementKey id, wxWindow* parent, wxPoint2DDouble center)
	: GraphicalNode(id) {
	
	// Default component dimensions and colors
	// High pixel density displays are accounted for in the GraphicalNode constructor

	// graphical characteristics
	m_bodyShape = wxRect2DDouble(-m_bodySize.GetWidth() / 2, -m_bodySize.GetHeight() / 2, m_bodySize.GetWidth(), m_bodySize.GetHeight());
	m_position = center;
	m_inputRect = wxRect2DDouble(-m_bodyShape.m_width / 2 - m_ioSize.GetWidth() / 2, -m_ioSize.GetHeight() / 2, m_ioSize.GetWidth(), m_ioSize.GetHeight());
	m_outputRect = wxRect2DDouble(m_bodyShape.m_width / 2 - m_ioSize.GetWidth() / 2, -m_ioSize.GetHeight() / 2, m_ioSize.GetWidth(), m_ioSize.GetHeight());
	m_bodySize = parent->FromDIP(wxSize(100, 75));
	m_ioSize = parent->FromDIP(wxSize(15, 15));
	m_ioColor = *wxBLUE;
	m_labelColor = *wxWHITE;
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

	m_bodyShape = other.m_bodyShape;
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

wxPoint2DDouble GraphicalNode::GetPosition()
{
	return m_position;
}

void GraphicalNode::SetPosition(const wxPoint2DDouble& position)
{
	m_position = position;
}

wxAffineMatrix2D GraphicalNode::GetTransform() const {
	wxAffineMatrix2D transform;
	transform.Translate(m_position.m_x, m_position.m_y);
	return transform;
}

std::list<GraphicalEdge*> GraphicalNode::GetOutputs() const
{
	return m_outputs;
}

std::list<GraphicalEdge*> GraphicalNode::GetInputs() const
{
	return m_inputs;
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
// this will become the Server drawing
void GraphicalNode::Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) {

	//// Transform coordinates according to camera and node transforms
	//wxAffineMatrix2D localToWindow = camera;
	//localToWindow.Concat(GetTransform());
	//gc->SetTransform(gc->CreateMatrix(localToWindow));

	//gc->SetPen(*wxTRANSPARENT_PEN);

<<<<<<< Updated upstream
	gc->SetBrush(wxBrush(ms_bodyColor));
	gc->DrawRectangle(m_rect.m_x, m_rect.m_y, m_rect.m_width, m_rect.m_height);
=======
	//// draw the rectangle 
	//gc->SetBrush(wxBrush(m_bodyColor));
	//gc->DrawRectangle(m_bodyShape.m_x, m_bodyShape.m_y, m_bodyShape.m_width, m_bodyShape.m_height);
>>>>>>> Stashed changes

	//// draw input and output rectangles
	//gc->SetBrush(wxBrush(m_ioColor));
	//gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	//gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	//// draw the text on the object
	//gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	//double textWidth, textHeight;
	//gc->GetTextExtent(m_label, &textWidth, &textHeight);
	//gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);
	MyDraw(camera, gc);
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
	else if (m_bodyShape.Contains(clickPosition))
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
<<<<<<< Updated upstream
}
=======
}

void GraphicalNode::SetBodyColor(const wxColor& color)
{
	m_bodyColor = color;
}

// GraphicalSource
GraphicalSource::GraphicalSource() : GraphicalNode() {}

GraphicalSource::GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center)
{
	
}

void GraphicalSource::MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc)
{
	// Transform coordinates according to camera and node transforms
	wxAffineMatrix2D localToWindow = camera;
	localToWindow.Concat(GetTransform());
	gc->SetTransform(gc->CreateMatrix(localToWindow));

	gc->SetPen(*wxTRANSPARENT_PEN);

	// draw the rectangle 
	gc->SetBrush(wxBrush(m_bodyColor));
	gc->DrawRectangle(m_bodyShape.m_x, m_bodyShape.m_y, m_bodyShape.m_width, m_bodyShape.m_height);

	// draw input and output rectangles
	gc->SetBrush(wxBrush(m_ioColor));
	//gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	// draw the text on the object
	gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);
	gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);

}

// GraphicalSink
GraphicalSink::GraphicalSink() : GraphicalNode() 
{
	m_bodyColor = *wxLIGHT_GREY;
}

GraphicalSink::GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center)
{
	
}

void GraphicalSink::MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc)
{
	// Transform coordinates according to camera and node transforms
	wxAffineMatrix2D localToWindow = camera;
	localToWindow.Concat(GetTransform());
	gc->SetTransform(gc->CreateMatrix(localToWindow));

	gc->SetPen(*wxTRANSPARENT_PEN);

	// draw the rectangle 
	gc->SetBrush(wxBrush(m_bodyColor));
	gc->DrawRectangle(m_bodyShape.m_x, m_bodyShape.m_y, m_bodyShape.m_width, m_bodyShape.m_height);

	// draw input and output rectangles
	gc->SetBrush(wxBrush(m_ioColor));
	gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	//gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	// draw the text on the object
	gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);
	gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);

}

GraphicalNode* NodeFactory::CreateNodeOfType(GenericNode::Type type)
{
	GraphicalNode* createdNode = new GraphicalServer();

	switch (type) {

	case GenericNode::SOURCE:

		delete createdNode;
		createdNode = new GraphicalSource();
		break;
	case GenericNode::SINK:

		delete createdNode;
		createdNode = new GraphicalSink();
		break;
	}

	return createdNode;
}

GraphicalNode* NodeFactory::CreateNodeOfType(GenericNode::Type type, ElementKey id, wxWindow* window, wxPoint2DDouble center)
{
	GraphicalNode* createdNode = new GraphicalServer(id, window, center);

	switch (type) {

	case GenericNode::SOURCE:

		delete createdNode;
		createdNode = new GraphicalSource(id, window, center);
		break;
	case GenericNode::SINK:

		delete createdNode;
		createdNode = new GraphicalSink(id, window, center);
		break;
	}

	return createdNode;
}

GraphicalServer::GraphicalServer()
{
}

GraphicalServer::GraphicalServer(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center)
{
}

void GraphicalServer::MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc)
{
}
>>>>>>> Stashed changes
