#include "GraphicalNode.h"

#include "wx/graphics.h"

#include "SimProject.h"

GraphicalNode::GraphicalNode() : GraphicalElement(), m_inputs(), m_outputs() 
{
}

GraphicalNode::GraphicalNode(ElementKey id) : GraphicalElement(id), m_inputs(), m_outputs()
{
	m_label = "Node " + std::to_string(m_id);

}

GraphicalNode::GraphicalNode(ElementKey id, wxWindow* parent, wxPoint2DDouble center)
	: GraphicalNode(id) {
	
	// Default component dimensions and colors
	// High pixel density displays are accounted for in the GraphicalNode constructor

	auto headerProp = new wxStringProperty("Property", wxPG_LABEL, "Value");
	m_properties.Add(headerProp);

	m_next = nullptr;
	m_previous = nullptr;

	/// graphical characteristics
	// size
	m_bodySize = parent->FromDIP(wxSize(100, 75));
	m_ioSize = parent->FromDIP(wxSize(15, 15));
	m_sizerSize = parent->FromDIP(wxSize(6, 6));

	// color
	m_bodyColor = *wxBLACK;
	m_labelColor = *wxWHITE;
	m_ioColor = *wxBLUE;
	m_sizerColor = *wxRED;

	// shape
	m_bodyShape = wxRect2DDouble(-m_bodySize.GetWidth() / 2, -m_bodySize.GetHeight() / 2, m_bodySize.GetWidth(), m_bodySize.GetHeight());

	// position
	m_position = center;

	// user sizing nodes
	m_sizers[0] = wxRect2DDouble(-m_bodyShape.m_width/2 -m_sizerSize.GetX()/2, -m_bodyShape.m_height/2 -m_sizerSize.GetY()/2, m_sizerSize.GetWidth(), m_sizerSize.GetHeight()); // top left
	m_sizers[1] = wxRect2DDouble(m_bodyShape.m_width/2 - m_sizerSize.GetX()/2, -m_bodyShape.m_height/2 -m_sizerSize.GetY()/2, m_sizerSize.GetWidth(), m_sizerSize.GetHeight()); // top right
	m_sizers[2] = wxRect2DDouble(-m_bodyShape.m_width/2 -m_sizerSize.GetX()/2, m_bodyShape.m_height - m_bodyShape.m_height / 2 -m_sizerSize.GetY()/2, m_sizerSize.GetWidth(), m_sizerSize.GetHeight()); // bottom left
	m_sizers[3] = wxRect2DDouble(m_bodyShape.m_width/2 - m_sizerSize.GetX()/2,
								 m_bodyShape.m_height - m_bodyShape.m_height / 2 - m_sizerSize.GetY()/2,
								 m_sizerSize.GetWidth(), m_sizerSize.GetHeight()); // bottom right

	// IO nodes
	m_inputRect = wxRect2DDouble(-m_bodyShape.m_width / 2 - m_ioSize.GetWidth() / 2, -m_ioSize.GetHeight() / 2, m_ioSize.GetWidth(), m_ioSize.GetHeight());
	m_outputRect = wxRect2DDouble(m_bodyShape.m_width / 2 - m_ioSize.GetWidth() / 2, -m_ioSize.GetHeight() / 2, m_ioSize.GetWidth(), m_ioSize.GetHeight());
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

	m_nodeType = other.m_nodeType;
	m_properties = other.m_properties;
	m_bodyColor = other.m_bodyColor;
	m_bodyShape = other.m_bodyShape;
	m_outputRect = other.m_outputRect;
	m_inputRect = other.m_inputRect;
	m_position = other.m_position;
	m_bodySize = other.m_bodySize;
	m_ioSize = other.m_ioSize;
	m_labelColor = other.m_labelColor;
	m_ioColor = other.m_ioColor;
	m_inputs = other.m_inputs;
	m_outputs = other.m_outputs;

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

wxPoint2DDouble GraphicalNode::GetCenter()
{
	auto x = m_bodyShape.m_x + (m_bodyShape.m_width / 2.0);
	auto y = m_bodyShape.m_y + (m_bodyShape.m_height / 2.0);
	wxPoint2DDouble center(x, y);

	return center;
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

Set<wxPGProperty> GraphicalNode::GetProperties()
{
	return m_properties;
}

void GraphicalNode::DisconnectOutputs() {
	for (auto output : m_outputs)
		output->Disconnect();
}

void GraphicalNode::DisconnectInputs() {
	for (auto input : m_inputs)
		input->Disconnect();
}

void GraphicalNode::SetNext(GraphicalNode* next)
{
	m_next = next;
}

void GraphicalNode::SetPrevious(GraphicalNode* previous)
{
	m_previous = previous;
}

GraphicalNode* GraphicalNode::GetNext()
{
	return m_next;
}

GraphicalNode* GraphicalNode::GetPrevious()
{
	return m_previous;
}

// Draws the node to a wxGraphicsContext
void GraphicalNode::Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) {
	MyDraw(camera, gc);
}

// Returns the selection state of the component given where the user clicked
Selection GraphicalNode::Select(const wxAffineMatrix2D& camera, wxPoint2DDouble clickPosition) {

	// Transform click position from window coordinates to node's local coordinates
	auto windowToLocal = camera;
	windowToLocal.Concat(GetTransform());
	windowToLocal.Invert();
	clickPosition = windowToLocal.TransformPoint(clickPosition);
	m_isSelected = false;

	// Return selection state according to what user clicked on
	for (int i = 0; i < 4; i++) {
		if (m_sizers[i].Contains(clickPosition)) {

			m_isSelected = true;
			return { this, Selection::State::NODE_SIZER };
		}
	}
	if (m_inputRect.Contains(clickPosition))
		return { this, Selection::State::NODE_INPUT };
	else if (m_outputRect.Contains(clickPosition))
		return { this, Selection::State::NODE_OUTPUT };
	else if (m_bodyShape.Contains(clickPosition)) {
		this->m_isSelected = true;
		return { this, Selection::State::NODE };
	}
	else
		return { nullptr, Selection::State::NONE };
}

void GraphicalNode::Move(wxPoint2DDouble displacement) {
	m_position += displacement;

	for (int i = 0; i < 4; i++) {
		//m_sizers[i].m_x += displacement.m_x;
		//m_sizers[i].m_y += displacement.m_y;
	}
	
	for (auto output : m_outputs)
		output->m_sourcePoint = GetOutputPoint();

	for (auto input : m_inputs)
		input->m_destinationPoint = GetInputPoint();
}

int GraphicalNode::GetSelectedSizerIndex(wxPoint2DDouble clickPosition)
{
	for (int i = 0; i < 4; i++) {

		auto x = m_sizers[i].m_x;
		auto y = m_sizers[i].m_y;
		auto transformedStartPos = GetTransformedPoint(wxPoint2DDouble(x, y));
		wxRect2DDouble rect(transformedStartPos.m_x, transformedStartPos.m_y, m_sizerSize.GetWidth(), m_sizerSize.GetHeight());

		if (rect.Contains(clickPosition)) {
			m_sizerSelected = i;
			return i;
		}
	}
	return -1;
}

int GraphicalNode::GetSelectedSizer()
{
	return m_sizerSelected;
}

wxColor GraphicalNode::GetBodyColor()
{
	return m_bodyColor;
}

void GraphicalNode::SetBodyColor(const wxColor& color)
{
	m_bodyColor = color;
}

wxSize GraphicalNode::GetSize()
{
	return m_bodySize;
}

void GraphicalNode::SetBodyShape(wxRect2DDouble newBody)
{
	m_bodyShape = newBody;
}

wxRect2DDouble GraphicalNode::GetBodyShape()
{
	return m_bodyShape;
}

void GraphicalNode::SetTransformationMatrix(wxAffineMatrix2D transform)
{
	m_transformation = transform;
}

wxAffineMatrix2D GraphicalNode::GetTransformationMatrix()
{
	return m_transformation;
}

wxPoint2DDouble GraphicalNode::GetTransformedPoint(wxPoint2DDouble toTransform)
{
	// transform drawing location to local
	wxAffineMatrix2D cTransform = m_transformation;
	cTransform.Invert();
	wxPoint2DDouble transformedPoint = cTransform.TransformPoint(toTransform);
	return transformedPoint;
}

void GraphicalNode::SetInputRect(wxRect2DDouble newInput)
{
	m_inputRect = newInput;
}

wxRect2DDouble GraphicalNode::GetInputRect()
{
	return m_inputRect;
}

void GraphicalNode::SetOutputRect(wxRect2DDouble newOutput)
{
	m_outputRect = newOutput;
}

wxRect2DDouble GraphicalNode::GetOutputRect()
{
	return m_outputRect;
}

void GraphicalNode::SetIOColor(wxColor newColor)
{
	m_ioColor = newColor;
}

wxColor GraphicalNode::GetIOColor()
{
	return m_ioColor;
}

void GraphicalNode::SetNodeType(GenericNode::Type type)
{
	m_nodeType = type;
}

/********************************************/
/* GraphicalSource:                         */
/* defines an entity generating object      */
/* provides basic entity creation			*/
/********************************************/
class GraphicalSource::MyProperties : public SimProperties {

public:
	MyProperties() {
		m_IA_Time = new Exponential(0.25);
		m_numToGen = 10;
	}

	MyProperties(Distribution* ia_time, int numGen) {
		m_IA_Time = ia_time;
		m_numToGen = numGen;
	}

private:
	Distribution* m_IA_Time;
	int m_numToGen;
};

class GraphicalSource::SourceProperties : public PropertiesWrapper {

public:
	SourceProperties(SimProperties* props, int id) : PropertiesWrapper(id) {
		m_props = props;
	}

	void ReportProperties() override {

	}

private:
	SimProperties* m_props;
};

GraphicalSource::GraphicalSource() : GraphicalNode()
{
	SetNodeType(GenericNode::SOURCE);
	m_iaTime = new Exponential(0.25);
}

GraphicalSource::GraphicalSource(const GraphicalSource& other)
{
	GraphicalNode::operator=(other);

	m_iaTime = other.m_iaTime;
	m_myProps = other.m_myProps;
}

GraphicalSource::GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center, "Source")
{
	SetNodeType(GenericNode::SOURCE);

	m_iaTime = new Exponential(0.25);
	ExponentialProperty* iaTime = new ExponentialProperty("Interarrival Time", wxPG_LABEL, (Exponential&)m_iaTime);

	m_properties.Add(iaTime);
}

GraphicalSource::~GraphicalSource()
{
	delete m_iaTime;
}

std::unique_ptr<GraphicalNode> GraphicalSource::Clone()
{
	return std::make_unique<GraphicalSource>(*this);
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

	if (m_isSelected) {
		// draw all the scaling rects
		gc->SetBrush(wxBrush(m_sizerColor));
		for (int i = 0; i < 4; i++) {

			wxDouble x = m_sizers[i].m_x;
			wxDouble y = m_sizers[i].m_y;
			wxDouble width = m_sizers[i].m_width;
			wxDouble height = m_sizers[i].m_height;
			gc->DrawRectangle(x, y, width, height);
		}
	}

	// draw the text on the object
	gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);
	gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);
}

std::unique_ptr<GraphicalNode::PropertiesWrapper> GraphicalSource::GetSimProperties()
{
	return std::make_unique<SourceProperties>(m_myProps, GetID());
}

void GraphicalSource::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

void GraphicalSource::SetIATime(Distribution* iaTime)
{
	m_iaTime = iaTime;
}

Distribution* GraphicalSource::GetIATime()
{
	return m_iaTime;
}

/****************************************************************/
/* GraphicalServer:                                             */
/* defines a single server single queue object                  */
/* provides basic server functionality							*/
/****************************************************************/

class GraphicalServer::MyProperties : public SimProperties {

public:
	MyProperties() {
		m_serviceTime = 1.0;
		m_st_timeUnit = MINUTES;
		m_numResources = 1;
	}

	// getting lazy, this works
	MyProperties(double st, TimeUnit tu, int nr) {
		m_serviceTime = st;
		m_st_timeUnit = tu;
		m_numResources = nr;		
	}

private:
	double m_serviceTime;
	TimeUnit m_st_timeUnit;
	int m_numResources;
};

class GraphicalServer::ServerProperties : public PropertiesWrapper {

public:
	ServerProperties(SimProperties* props, int id) : PropertiesWrapper(id) {
		m_props = props;
	}

	void ReportProperties() override {
			
	}

private:
	SimProperties* m_props;
};

GraphicalServer::GraphicalServer()
{
	SetNodeType(GenericNode::SERVER);
}

GraphicalServer::GraphicalServer(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center, "Server")
{
	SetNodeType(GenericNode::SERVER);

	m_serviceTime = new Triangular(1.0, 2.0, 3.0);
	m_timeUnit = MINUTES;
	m_numResources = 1;

	//auto stProp = new wxFloatProperty("Service Time", wxPG_LABEL, m_serviceTime);
	auto timeUnitProp = new wxStringProperty("Time Unit", wxPG_LABEL, TimeToString.at(m_timeUnit));
	auto resourceNumProp = new wxUIntProperty("Number of Resources", wxPG_LABEL, m_numResources);

	//m_properties.Add(stProp);
	m_properties.Add(timeUnitProp);
	m_properties.Add(resourceNumProp);
}

GraphicalServer::~GraphicalServer()
{
	delete m_serviceTime;
}

GraphicalServer::GraphicalServer(const GraphicalServer& other)
{
	GraphicalNode::operator=(other);

	m_serviceTime = other.m_serviceTime;
	m_timeUnit = other.m_timeUnit;
	m_numResources = other.m_numResources;
	m_myProps = other.m_myProps;
}

std::unique_ptr<GraphicalNode> GraphicalServer::Clone()
{
	return std::make_unique<GraphicalServer>(*this);
}

void GraphicalServer::MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc)
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
	gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	if (m_isSelected) {
		// draw all the scaling rects
		gc->SetBrush(wxBrush(m_sizerColor));
		for (int i = 0; i < 4; i++) {

			wxDouble x = m_sizers[i].m_x;
			wxDouble y = m_sizers[i].m_y;
			wxDouble width = m_sizers[i].m_width;
			wxDouble height = m_sizers[i].m_height;
			gc->DrawRectangle(x, y, width, height);
		}
	}

	// draw the text on the object
	gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);
	gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);

}

std::unique_ptr<GraphicalNode::PropertiesWrapper> GraphicalServer::GetSimProperties()
{
	return std::make_unique<ServerProperties>(m_myProps, GetID());
}

void GraphicalServer::SetServiceTime(Distribution* serviceTime)
{
	m_serviceTime = serviceTime;
}

Distribution* GraphicalServer::GetServiceTime()
{
	return m_serviceTime;
}

void GraphicalServer::Accept(Visitor& visitor) 
{
	return visitor.Visit(*this);
}

/****************************************/
/* GraphicalSink                        */  
/* This is essentially useless          */
/* for sink specifically.				*/
/* this is here for the design pattern. */
/****************************************/

class GraphicalSink::MyProperties : public SimProperties {

public:
	MyProperties() {

	}

private:

};

class GraphicalSink::SinkProperties : public PropertiesWrapper {

public:
	SinkProperties(SimProperties* props, int id) : PropertiesWrapper(id) {
		m_props = props;
	}

	void ReportProperties() override {

	}

private:
	SimProperties* m_props;
};

GraphicalSink::GraphicalSink() : GraphicalNode() 
{
	SetNodeType(GenericNode::SINK);
}

GraphicalSink::GraphicalSink(const GraphicalSink& other)
{
	GraphicalNode::operator=(other);
}

GraphicalSink::GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center)
	: GraphicalNode(id, window, center, "Sink")
{
	SetNodeType(GenericNode::SINK);
}


std::unique_ptr<GraphicalNode> GraphicalSink::Clone()
{
	return std::make_unique<GraphicalSink>(*this);
}

void GraphicalSink::MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc)
{
	// Transform coordinates according to camera and node transforms
	wxAffineMatrix2D localToWindow = camera;
	localToWindow.Concat(GetTransform());
	gc->SetTransform(gc->CreateMatrix(localToWindow));

	gc->SetPen(*wxTRANSPARENT_PEN);

	m_bodyColor = *wxBLACK;

	// draw the rectangle 
	gc->SetBrush(wxBrush(m_bodyColor));
	gc->DrawRectangle(m_bodyShape.m_x, m_bodyShape.m_y, m_bodyShape.m_width, m_bodyShape.m_height);

	// draw input and output rectangles
	gc->SetBrush(wxBrush(m_ioColor));
	gc->DrawRectangle(m_inputRect.m_x, m_inputRect.m_y, m_inputRect.m_width, m_inputRect.m_height);
	//gc->DrawRectangle(m_outputRect.m_x, m_outputRect.m_y, m_outputRect.m_width, m_outputRect.m_height);

	if (m_isSelected) {
		// draw all the scaling rects
		gc->SetBrush(wxBrush(m_sizerColor));
		for (int i = 0; i < 4; i++) {

			wxDouble x = m_sizers[i].m_x;
			wxDouble y = m_sizers[i].m_y;
			wxDouble width = m_sizers[i].m_width;
			wxDouble height = m_sizers[i].m_height;
			gc->DrawRectangle(x, y, width, height);
		}
	}

	// draw the text on the object
	gc->SetFont(*wxNORMAL_FONT, m_labelColor);
	double textWidth, textHeight;
	gc->GetTextExtent(m_label, &textWidth, &textHeight);
	gc->DrawText(m_label, m_bodyShape.m_x + m_bodyShape.m_width / 2 - textWidth / 2, m_bodyShape.m_y + m_bodyShape.m_height / 2 - textHeight);
}

std::unique_ptr<GraphicalNode::PropertiesWrapper> GraphicalSink::GetSimProperties()
{
	return std::make_unique<SinkProperties>(m_myProps, GetID());
}

void GraphicalSink::Accept(Visitor& visitor) 
{
	return visitor.Visit(*this);
}