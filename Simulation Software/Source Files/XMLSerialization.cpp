#include "XMLSerialization.h"

void XMLSerializingVisitor::Visit(GraphicalSource& source)
{
	wxString label(source.GetLabel());

	// main object rectangle
	wxXmlNode* sourceNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	sourceNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::BaseNodeName);
	sourceNode->AddAttribute(XmlNodeKeys::NameAttribute, XmlNodeKeys::SourceNodeName);
	sourceNode->AddAttribute(XmlNodeKeys::LabelAttribute, label);
	sourceNode->AddAttribute(XmlNodeKeys::NodeIDAttribute, wxString::FromDouble(source.GetID()));
	sourceNode->AddAttribute(XmlNodeKeys::ColorAttribute, source.GetBodyColor().GetAsString());
	sourceNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(source.GetBodyShape().m_x));
	sourceNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(source.GetBodyShape().m_y));
	sourceNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(source.GetBodyShape().m_width));
	sourceNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(source.GetBodyShape().m_height));
	
	// Interarrival Distribution
	XMLSerializer toXML;
	wxXmlNode* iaDist = toXML.SerializeDistribution(source.GetIATime());

	// output rectangle
	wxXmlNode* outputNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::OutputRectAttribute);
	outputNode->AddAttribute(XmlNodeKeys::ColorAttribute, source.GetIOColor().GetAsString());
	outputNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(source.GetOutputRect().m_x));
	outputNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(source.GetOutputRect().m_y));
	outputNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(source.GetOutputRect().m_width));
	outputNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(source.GetOutputRect().m_height));

	// make links
	sourceNode->AddChild(iaDist);
	sourceNode->AddChild(outputNode);
	m_parentNode->AddChild(sourceNode);
}

void XMLSerializingVisitor::Visit(GraphicalServer& server)
{
	wxString label(server.GetLabel());

	// main object rectangle
	wxXmlNode* serverNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	serverNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::BaseNodeName);
	serverNode->AddAttribute(XmlNodeKeys::NameAttribute, XmlNodeKeys::ServerNodeName);
	serverNode->AddAttribute(XmlNodeKeys::LabelAttribute, label);
	serverNode->AddAttribute(XmlNodeKeys::NodeIDAttribute, wxString::FromDouble(server.GetID()));
	serverNode->AddAttribute(XmlNodeKeys::ColorAttribute, server.GetBodyColor().GetAsString());
	serverNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(server.GetBodyShape().m_x));
	serverNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(server.GetBodyShape().m_y));
	serverNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(server.GetBodyShape().m_width));
	serverNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(server.GetBodyShape().m_height));

	// Service Time Distribution
	XMLSerializer toXML;
	wxXmlNode* serviceTimeDist = toXML.SerializeDistribution(server.GetServiceTime());

	// input rectangle
	wxXmlNode* inputNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::InputRectAttribute);
	inputNode->AddAttribute(XmlNodeKeys::ColorAttribute, server.GetIOColor().GetAsString());
	inputNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(server.GetOutputRect().m_x));
	inputNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(server.GetOutputRect().m_y));
	inputNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(server.GetOutputRect().m_width));
	inputNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(server.GetOutputRect().m_height));

	// output rectangle
	wxXmlNode* outputNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::OutputRectAttribute);
	outputNode->AddAttribute(XmlNodeKeys::ColorAttribute, server.GetIOColor().GetAsString());
	outputNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(server.GetOutputRect().m_x));
	outputNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(server.GetOutputRect().m_y));
	outputNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(server.GetOutputRect().m_width));
	outputNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(server.GetOutputRect().m_height));

	// make links
	serverNode->AddChild(serviceTimeDist);
	serverNode->AddChild(inputNode);
	serverNode->AddChild(outputNode);
	m_parentNode->AddChild(serverNode);
}

void XMLSerializingVisitor::Visit(GraphicalSink& sink) 
{
	wxString label(sink.GetLabel());

	// main object rectangle
	wxXmlNode* sinkNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	sinkNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::BaseNodeName);
	sinkNode->AddAttribute(XmlNodeKeys::NameAttribute, XmlNodeKeys::SinkNodeName);
	sinkNode->AddAttribute(XmlNodeKeys::LabelAttribute, label);
	sinkNode->AddAttribute(XmlNodeKeys::NodeIDAttribute, wxString::FromDouble(sink.GetID()));
	sinkNode->AddAttribute(XmlNodeKeys::ColorAttribute, sink.GetBodyColor().GetAsString());
	sinkNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(sink.GetBodyShape().m_x));
	sinkNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(sink.GetBodyShape().m_y));
	sinkNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(sink.GetBodyShape().m_width));
	sinkNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(sink.GetBodyShape().m_height));

	// input rectangle
	wxXmlNode* inputNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::InputRectAttribute);
	inputNode->AddAttribute(XmlNodeKeys::ColorAttribute, sink.GetIOColor().GetAsString());
	inputNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(sink.GetOutputRect().m_x));
	inputNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(sink.GetOutputRect().m_y));
	inputNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(sink.GetOutputRect().m_width));
	inputNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(sink.GetOutputRect().m_height));

	// make links
	sinkNode->AddChild(inputNode);
	m_parentNode->AddChild(sinkNode);
}

void XMLSerializingVisitor::Visit(GraphicalEdge& edge)
{
	wxString label(edge.GetLabel());

	wxXmlNode* edgeNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::EdgeNodeName);
	edgeNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::EdgeNodeName);
	edgeNode->AddAttribute(XmlNodeKeys::LabelAttribute, label);
	edgeNode->AddAttribute(XmlNodeKeys::NodeIDAttribute, wxString::FromDouble(edge.GetID()));
	edgeNode->AddAttribute(XmlNodeKeys::EdgeSourceID, wxString::FromDouble(edge.GetSourceID()));
	edgeNode->AddAttribute(XmlNodeKeys::EdgeDestinationID, wxString::FromDouble(edge.GetDestinationID()));

	m_parentNode->AddChild(edgeNode);
}

void XMLSerializingVisitor::Visit(Exponential& expo)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ExponentialDist);

	auto mean = wxString::FromDouble(expo.GetMean());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::ExponentialDist);
	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Uniform& uni)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::UniformDist);

	auto min = wxString::FromDouble(uni.GetMin());
	auto max = wxString::FromDouble(uni.GetMax());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::UniformDist);
	distNode->AddAttribute(XmlNodeKeys::MinAttribute, min);
	distNode->AddAttribute(XmlNodeKeys::MaxAttribute, max);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Triangular& tri)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::TriangularDist);

	auto min = wxString::FromDouble(tri.GetMin());
	auto expected = wxString::FromDouble(tri.GetExpected());
	auto max = wxString::FromDouble(tri.GetMax());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::TriangularDist);
	distNode->AddAttribute(XmlNodeKeys::MinAttribute, min);
	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, expected);
	distNode->AddAttribute(XmlNodeKeys::MaxAttribute, max);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Normal& norm)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::NormalDist);

	auto mean = wxString::FromDouble(norm.GetMean());
	auto variance = wxString::FromDouble(norm.GetVariance());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::NormalDist);
	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);
	distNode->AddAttribute(XmlNodeKeys::VarianceAttribute, variance);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Poisson& poisson)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::PoissonDist);

	auto mean = wxString::FromDouble(poisson.GetMean());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::PoissonDist);
	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Constant& constant)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ConstantDist);

	auto mean = wxString::FromDouble(constant.GetMean());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::ConstantDist);
	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Weibull& weibull)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::WeibullDist);

	auto scale = wxString::FromDouble(weibull.GetScale());
	auto shape = wxString::FromDouble(weibull.GetShape());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::WeibullDist);
	distNode->AddAttribute(XmlNodeKeys::ScaleAttribute, scale);
	distNode->AddAttribute(XmlNodeKeys::ShapeAttribute, shape);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Erlang& erlang)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ErlangDist);

	auto scale = wxString::FromDouble(erlang.GetScale());
	auto shape = wxString::FromDouble(erlang.GetShape());

	distNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::ErlangDist);
	distNode->AddAttribute(XmlNodeKeys::ScaleAttribute, scale);
	distNode->AddAttribute(XmlNodeKeys::ShapeAttribute, shape);

	m_parentNode->AddChild(distNode);
}

wxXmlDocument XMLSerializer::SerializeSimObjects(const std::vector<std::unique_ptr<GraphicalNode>>& nodes,
												 const std::vector<std::unique_ptr<GraphicalEdge>>& edges)
{
	wxXmlDocument doc;

	wxXmlNode* docNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::DocumentNodeName);
	docNode->AddAttribute(XmlNodeKeys::VersionAttribute, XmlNodeKeys::VersionValue);

	XMLSerializingVisitor visitor{ docNode };

	for (const auto& node : nodes)
	{
		node->Accept(visitor);
	}
	for (const auto& edge : edges)
	{
		edge->Accept(visitor);
	}

	doc.SetRoot(docNode);

	return doc;
}

wxXmlNode* XMLSerializer::SerializeDistribution(Distribution* dist)
{
	wxXmlNode* docNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Distribution");
	XMLSerializingVisitor visitor{ docNode };
	dist->Accept(visitor);
	return docNode;
}

SimulationObjects XMLSerializer::DeserializeXMLDocument(const wxXmlDocument& doc)
{
	wxXmlNode* root = doc.GetRoot();

	Set<GraphicalNode> nodes;
	Set<GraphicalEdge> edges;

	for (wxXmlNode* node = root->GetChildren(); node; node = node->GetNext()) {

		auto type = node->GetAttribute(XmlNodeKeys::TypeAttribute);

		XMLDeserializationFactory factory{ };

		if (type == XmlNodeKeys::BaseNodeName) {
			GraphicalNode* gnode = factory.DeserializeNode(node).release();
			nodes.Add(gnode);
		}
		else if(type == XmlNodeKeys::EdgeNodeName) {
			GraphicalEdge* gEdge = factory.DeserializeEdge(node).release();
			edges.Add(gEdge);
		}
	}

	SimulationObjects unlinkedObjs(nodes, edges);
	SimulationObjects linkedSimObjs = LinkSimObjects(unlinkedObjs);
	return linkedSimObjs;
}

SimulationObjects XMLSerializer::LinkSimObjects(SimulationObjects& simObjects)
{
	Set<GraphicalNode> nodes = simObjects.GetNodes();
	Set<GraphicalEdge> edges = simObjects.GetEdges();

	// populate all nodes with their next
	for (int i = 0; i < edges.GetSize(); i++) {
		for (int j = 0; j < nodes.GetSize(); j++) {

			if (edges[i]->GetSourceID() == nodes[j]->GetID()){
				edges[i]->SetSource(nodes[j]);
			}
			else if (edges[i]->GetDestinationID() == nodes[j]->GetID()) {
				edges[i]->SetDestination(nodes[j]);

				// if both are set already
				if ((edges[i]->GetSource() != nullptr) && (edges[i]->GetDestination() != nullptr))
					break;
			}
		}

		// if both are set already
		if ((edges[i]->GetSource() != nullptr) && (edges[i]->GetDestination() != nullptr)) {

			GraphicalNode* from = edges[i]->GetSource();
			GraphicalNode* to = edges[i]->GetDestination();

			from->AddNext(to);
			to->AddPrevious(from);

			edges[i]->SetSource(from);
			edges[i]->SetDestination(to);
		}
	}

	SimulationObjects linkedSimObjs(nodes, edges);
	return linkedSimObjs;
}

std::unique_ptr<GraphicalNode> XMLDeserializationFactory::DeserializeNode(const wxXmlNode* node)
{
	auto type = node->GetAttribute(XmlNodeKeys::NameAttribute);

	if (type == XmlNodeKeys::SourceNodeName) {
		return std::make_unique<GraphicalSource>(DeserializeSource(node));
	}
	else if (type == XmlNodeKeys::ServerNodeName) {
		return std::make_unique<GraphicalServer>(DeserializeServer(node));
	}
	else if (type == XmlNodeKeys::SinkNodeName) {
		return std::make_unique<GraphicalSink>(DeserializeSink(node));
	}

	// should never make it here
	throw std::runtime_error("Unknown SimObject type " + type);
}

std::unique_ptr<GraphicalEdge> XMLDeserializationFactory::DeserializeEdge(const wxXmlNode* node)
{
	GraphicalEdge edge;

	auto id = std::stoi(node->GetAttribute(XmlNodeKeys::NodeIDAttribute).ToStdString());
	auto sourceID = std::stoi(node->GetAttribute(XmlNodeKeys::EdgeSourceID).ToStdString());
	auto destinationID = std::stoi(node->GetAttribute(XmlNodeKeys::EdgeDestinationID).ToStdString());

	edge.SetID(id);
	edge.SetSourceID(sourceID);
	edge.SetDestinationID(destinationID);

	return std::make_unique<GraphicalEdge>(edge);
}

std::unique_ptr<Distribution> XMLDeserializationFactory::DeserializeDistribution(const wxXmlNode* node)
{
	auto distType  = node->GetAttribute(XmlNodeKeys::TypeAttribute);

	if (distType == XmlNodeKeys::ExponentialDist) {
		return std::make_unique<Exponential>(DeserializeExponential(node));
	}
	else if (distType == XmlNodeKeys::UniformDist) {
		return std::make_unique<Uniform>(DeserializeUniform(node));

	}
	else if (distType == XmlNodeKeys::TriangularDist) {
		return std::make_unique<Triangular>(DeserializeTriangular(node));

	}
	else if (distType == XmlNodeKeys::NormalDist) {
		return std::make_unique<Normal>(DeserializeNormal(node));

	}
	else if (distType == XmlNodeKeys::PoissonDist) {
		return std::make_unique<Poisson>(DeserializePoisson(node));

	}
	else if (distType == XmlNodeKeys::ConstantDist) {
		return std::make_unique<Constant>(DeserializeConstant(node));

	}
	else if (distType == XmlNodeKeys::WeibullDist) {
		return std::make_unique<Weibull>(DeserializeWeibull(node));

	}
	else if (distType == XmlNodeKeys::ErlangDist) {
		return std::make_unique<Erlang>(DeserializeErlang(node));

	}

	// should never make it here
	throw std::runtime_error("Unknown Distribution type " + distType);
}

GraphicalSource XMLDeserializationFactory::DeserializeSource(const wxXmlNode* node)
{
	GraphicalSource* sourceNode = (GraphicalSource*)NodeFactory::CreateGraphicalNode(GenericNode::SOURCE);

	// main rectangle properties
	auto id = std::stoi(node->GetAttribute(XmlNodeKeys::NodeIDAttribute).ToStdString());
	auto label = node->GetAttribute(XmlNodeKeys::LabelAttribute).ToStdString();
	auto bodyColor = node->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto x = std::stod(node->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto y = std::stod(node->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto width = std::stod(node->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto height = std::stod(node->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble bodyShape(x, y, width, height);

	// interarrival distribution
	wxXmlNode* iaDistXml = node->GetChildren()->GetChildren();
	Distribution* iaDist = DeserializeDistribution(iaDistXml).release();
	
	// output rectangle properties
	auto outputRect = node->GetChildren()->GetNext();
	auto obodyColor = outputRect->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto ox = std::stod(outputRect->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto oy = std::stod(outputRect->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto owidth = std::stod(outputRect->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto oheight = std::stod(outputRect->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble outputRectangle(ox, oy, owidth, oheight);

	sourceNode->SetLabel(label);
	sourceNode->SetID(id);
	sourceNode->SetBodyColor(bodyColor);
	sourceNode->SetBodyShape(bodyShape);
	sourceNode->SetPosition(bodyShape.GetCentre());
	sourceNode->SetIATime(iaDist);
	sourceNode->SetOutputRect(outputRectangle);
	sourceNode->SetIOColor(obodyColor);

	return *sourceNode;
}

GraphicalServer XMLDeserializationFactory::DeserializeServer(const wxXmlNode* node)
{
	GraphicalServer* serverNode = (GraphicalServer*)NodeFactory::CreateGraphicalNode(GenericNode::SERVER);

	// main rectangle properties
	auto id = std::stoi(node->GetAttribute(XmlNodeKeys::NodeIDAttribute).ToStdString());
	auto label = node->GetAttribute(XmlNodeKeys::LabelAttribute).ToStdString();
	auto bodyColor = node->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto x = std::stod(node->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto y = std::stod(node->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto width = std::stod(node->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto height = std::stod(node->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble bodyShape(x, y, width, height);

	// interarrival distribution
	wxXmlNode* serviceTimeDistXml = node->GetChildren()->GetChildren();
	Distribution* serviceTimeDist = DeserializeDistribution(serviceTimeDistXml).release();

	// input rectangle properties
	auto inputRect = node->GetChildren()->GetNext();
	auto iobodyColor = inputRect->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto ix = std::stod(inputRect->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto iy = std::stod(inputRect->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto iwidth = std::stod(inputRect->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto iheight = std::stod(inputRect->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble inputRectangle(ix, iy, iwidth, iheight);

	// output rectangle properties
	auto outputRect = inputRect->GetNext();
	auto ox = std::stod(outputRect->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto oy = std::stod(outputRect->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto owidth = std::stod(outputRect->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto oheight = std::stod(outputRect->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble outputRectangle(ox, oy, owidth, oheight);

	serverNode->SetLabel(label);
	serverNode->SetID(id);
	serverNode->SetBodyColor(bodyColor);
	serverNode->SetBodyShape(bodyShape);
	serverNode->SetServiceTime(serviceTimeDist);
	serverNode->SetInputRect(inputRectangle);
	serverNode->SetOutputRect(outputRectangle);
	serverNode->SetIOColor(iobodyColor);

	return *serverNode;
}

GraphicalSink XMLDeserializationFactory::DeserializeSink(const wxXmlNode* node)
{
	GraphicalSink* sinkNode = (GraphicalSink*)NodeFactory::CreateGraphicalNode(GenericNode::SINK);

	// main rectangle properties
	auto id = std::stoi(node->GetAttribute(XmlNodeKeys::NodeIDAttribute).ToStdString());
	auto label = node->GetAttribute(XmlNodeKeys::LabelAttribute).ToStdString();
	auto bodyColor = node->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto x = std::stod(node->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto y = std::stod(node->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto width = std::stod(node->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto height = std::stod(node->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble bodyShape(x, y, width, height);

	// input rectangle properties
	auto inputRect = node->GetChildren();
	auto ibodyColor = inputRect->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto ix = std::stod(inputRect->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto iy = std::stod(inputRect->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto iwidth = std::stod(inputRect->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto iheight = std::stod(inputRect->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble inputRectangle(ix, iy, iwidth, iheight);

	sinkNode->SetLabel(label);
	sinkNode->SetID(id);
	sinkNode->SetBodyColor(bodyColor);
	sinkNode->SetBodyShape(bodyShape);
	sinkNode->SetInputRect(inputRectangle);
	sinkNode->SetIOColor(ibodyColor);

	return *sinkNode;
}

Exponential XMLDeserializationFactory::DeserializeExponential(const wxXmlNode* node)
{
	auto mean = std::stod(node->GetAttribute(XmlNodeKeys::MeanAttribute).ToStdString());
	Exponential exp(mean);
	return exp;
}

Uniform XMLDeserializationFactory::DeserializeUniform(const wxXmlNode* node)
{
	auto min = std::stod(node->GetAttribute(XmlNodeKeys::MinAttribute).ToStdString());
	auto max = std::stod(node->GetAttribute(XmlNodeKeys::MaxAttribute).ToStdString());
	Uniform uni(min, max);
	return uni;
}

Triangular XMLDeserializationFactory::DeserializeTriangular(const wxXmlNode* node)
{
	auto min = std::stod(node->GetAttribute(XmlNodeKeys::MinAttribute).ToStdString());
	auto expected = std::stod(node->GetAttribute(XmlNodeKeys::MeanAttribute).ToStdString());
	auto max = std::stod(node->GetAttribute(XmlNodeKeys::MaxAttribute).ToStdString());
	Triangular tri(min, expected, max);
	return tri;
}

Normal XMLDeserializationFactory::DeserializeNormal(const wxXmlNode* node)
{
	auto mean = std::stod(node->GetAttribute(XmlNodeKeys::MeanAttribute).ToStdString());
	auto variance = std::stod(node->GetAttribute(XmlNodeKeys::VarianceAttribute).ToStdString());
	Normal norm(mean, variance);
	return norm;
}

Poisson XMLDeserializationFactory::DeserializePoisson(const wxXmlNode* node)
{
	auto mean = std::stod(node->GetAttribute(XmlNodeKeys::MeanAttribute).ToStdString());
	Poisson	poisson(mean);
	return poisson;
}

Constant XMLDeserializationFactory::DeserializeConstant(const wxXmlNode* node)
{
	auto mean = std::stod(node->GetAttribute(XmlNodeKeys::MeanAttribute).ToStdString());
	Constant constant(mean);
	return constant;
}

Weibull XMLDeserializationFactory::DeserializeWeibull(const wxXmlNode* node)
{
	auto scale = std::stod(node->GetAttribute(XmlNodeKeys::ScaleAttribute).ToStdString());
	auto shape = std::stod(node->GetAttribute(XmlNodeKeys::ShapeAttribute).ToStdString());
	Weibull weibull(scale, shape);
	return weibull;
}

Erlang XMLDeserializationFactory::DeserializeErlang(const wxXmlNode* node)
{
	auto scale = std::stod(node->GetAttribute(XmlNodeKeys::ScaleAttribute).ToStdString());
	auto shape = std::stod(node->GetAttribute(XmlNodeKeys::ShapeAttribute).ToStdString());
	Erlang erlang(scale, shape);
	return erlang;
}

SimulationObjects::SimulationObjects(Set<GraphicalNode> nodes, Set<GraphicalEdge> edges)
{
	m_nodes = nodes;
	m_edges = edges;
}

Set<GraphicalNode> SimulationObjects::GetNodes()
{
	return m_nodes;
}

Set<GraphicalEdge> SimulationObjects::GetEdges()
{
	return m_edges;
}
