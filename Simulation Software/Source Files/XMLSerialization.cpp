#include "XMLSerialization.h"

void XMLSerializingVisitor::Visit(GraphicalSource& source)
{
	// main object rectangle
	wxXmlNode* sourceNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	sourceNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::SourceNodeName);
	sourceNode->AddAttribute(XmlNodeKeys::ColorAttribute, source.GetBodyColor().GetAsString());
	sourceNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(source.GetBodyShape().m_x));
	sourceNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(source.GetBodyShape().m_y));
	sourceNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(source.GetBodyShape().m_width));
	sourceNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(source.GetBodyShape().m_height));
	
	// Interarrival Distribution
	wxXmlNode iaDist = XMLSerializer::SerializeDistribution(source.GetIATime());

	// output rectangle
	wxXmlNode* outputNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::OutputRectAttribute);
	outputNode->AddAttribute(XmlNodeKeys::ColorAttribute, source.GetIOColor().GetAsString());
	outputNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(source.GetOutputRect().m_x));
	outputNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(source.GetOutputRect().m_y));
	outputNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(source.GetOutputRect().m_width));
	outputNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(source.GetOutputRect().m_height));

	// make links
	sourceNode->AddChild(&iaDist);
	sourceNode->AddChild(outputNode);
	m_parentNode->AddChild(sourceNode);
}

void XMLSerializingVisitor::Visit(GraphicalServer& server)
{
	// main object rectangle
	wxXmlNode* serverNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	serverNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::ServerNodeName);
	serverNode->AddAttribute(XmlNodeKeys::ColorAttribute, server.GetBodyColor().GetAsString());
	serverNode->AddAttribute(XmlNodeKeys::XAttribute, wxString::FromDouble(server.GetBodyShape().m_x));
	serverNode->AddAttribute(XmlNodeKeys::YAttribute, wxString::FromDouble(server.GetBodyShape().m_y));
	serverNode->AddAttribute(XmlNodeKeys::WidthAttribute, wxString::FromDouble(server.GetBodyShape().m_width));
	serverNode->AddAttribute(XmlNodeKeys::HeightAttribute, wxString::FromDouble(server.GetBodyShape().m_height));

	// Service Time Distribution
	wxXmlNode serviceTimeDist = XMLSerializer::SerializeDistribution(server.GetServiceTime());

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
	serverNode->AddChild(outputNode);
	serverNode->AddChild(inputNode);
	serverNode->AddChild(&serviceTimeDist);
	m_parentNode->AddChild(serverNode);
}

void XMLSerializingVisitor::Visit(GraphicalSink& sink) 
{
	// main object rectangle
	wxXmlNode* sinkNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::BaseNodeName);
	sinkNode->AddAttribute(XmlNodeKeys::TypeAttribute, XmlNodeKeys::SinkNodeName);
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

void XMLSerializingVisitor::Visit(Exponential& expo)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ExponentialDist);

	auto mean = wxString::FromDouble(expo.GetMean());

	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Uniform& uni)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::UniformDist);

	auto min = wxString::FromDouble(uni.GetMin());
	auto max = wxString::FromDouble(uni.GetMax());

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

	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);
	distNode->AddAttribute(XmlNodeKeys::VarianceAttribute, variance);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Poisson& poisson)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::PoissonDist);

	auto mean = wxString::FromDouble(poisson.GetMean());

	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Constant& constant)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ConstantDist);

	auto mean = wxString::FromDouble(constant.GetMean());

	distNode->AddAttribute(XmlNodeKeys::MeanAttribute, mean);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Weibull& weibull)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::WeibullDist);

	auto scale = wxString::FromDouble(weibull.GetScale());
	auto shape = wxString::FromDouble(weibull.GetShape());

	distNode->AddAttribute(XmlNodeKeys::ScaleAttribute, scale);
	distNode->AddAttribute(XmlNodeKeys::ShapeAttribute, shape);

	m_parentNode->AddChild(distNode);
}

void XMLSerializingVisitor::Visit(Erlang& erlang)
{
	wxXmlNode* distNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::ErlangDist);

	auto scale = wxString::FromDouble(erlang.GetScale());
	auto shape = wxString::FromDouble(erlang.GetShape());

	distNode->AddAttribute(XmlNodeKeys::ScaleAttribute, scale);
	distNode->AddAttribute(XmlNodeKeys::ShapeAttribute, shape);

	m_parentNode->AddChild(distNode);
}

wxXmlDocument XMLSerializer::SerializeNodes(const std::vector<std::unique_ptr<GraphicalNode>>& nodes)
{
	wxXmlDocument doc;

	wxXmlNode* docNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::DocumentNodeName);
	docNode->AddAttribute(XmlNodeKeys::VersionAttribute, XmlNodeKeys::VersionValue);

	XMLSerializingVisitor visitor{ docNode };

	for (const auto& node : nodes)
	{
		node->Accept(visitor);
	}

	doc.SetRoot(docNode);

	return doc;
}

wxXmlNode XMLSerializer::SerializeDistribution(Distribution* dist)
{
	wxXmlNode* docNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodeKeys::DocumentNodeName);
	docNode->AddAttribute(XmlNodeKeys::VersionAttribute, XmlNodeKeys::VersionValue);

	XMLSerializingVisitor visitor{ docNode };

	dist->Accept(visitor);
	return *docNode;
}

std::unique_ptr<GraphicalNode> XMLDeserializationFactory::DeserializeNode(const wxXmlNode* node)
{
	auto type = node->GetAttribute(XmlNodeKeys::TypeAttribute);

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

std::unique_ptr<Distribution> XMLDeserializationFactory::DeserializeDistribution(const wxXmlNode* node)
{
	auto distType = node->GetAttribute(XmlNodeKeys::DistributionAttribute);

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

	throw std::runtime_error("Unknown Distribution type " + distType);
}

GraphicalSource XMLDeserializationFactory::DeserializeSource(const wxXmlNode* node)
{
	GraphicalSource* sourceNode = (GraphicalSource*)NodeFactory::CreateGraphicalNode(GenericNode::SOURCE);

	// main rectangle properties
	auto bodyColor = node->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto x = std::stod(node->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto y = std::stod(node->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto width = std::stod(node->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto height = std::stod(node->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble bodyShape(x, y, width, height);
	sourceNode->SetBodyShape(bodyShape);
	sourceNode->SetBodyColor(bodyColor);
	
	// interarrival distribution
	auto iaDistXml = node->GetChildren();
	auto iaDist = DeserializeDistribution(iaDistXml);
	// ADD DISTRIBUTION TO SOURCE NODE => DONT KNOW HOW RN TOO TIRED
	
	// output rectangle properties
	auto outputRect = node->GetChildren()->GetNext();
	auto ibodyColor = outputRect->GetAttribute(XmlNodeKeys::ColorAttribute);
	auto ix = std::stod(outputRect->GetAttribute(XmlNodeKeys::XAttribute).ToStdString());
	auto iy = std::stod(outputRect->GetAttribute(XmlNodeKeys::YAttribute).ToStdString());
	auto iwidth = std::stod(outputRect->GetAttribute(XmlNodeKeys::WidthAttribute).ToStdString());
	auto iheight = std::stod(outputRect->GetAttribute(XmlNodeKeys::HeightAttribute).ToStdString());
	wxRect2DDouble outputRectangle(ix, iy, iwidth, iheight);
	sourceNode->SetOutputRect(outputRectangle);
	sourceNode->SetIOColor(ibodyColor);

	return *sourceNode;
}

GraphicalServer XMLDeserializationFactory::DeserializeServer(const wxXmlNode* node)
{
	return GraphicalServer();
}

GraphicalSink XMLDeserializationFactory::DeserializeSink(const wxXmlNode* node)
{
	return GraphicalSink();
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
