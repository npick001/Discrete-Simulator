#pragma once
#include <wx/xml/xml.h>

#include "Distribution.h"
#include "GraphicalNode.h"
#include "SimProject.h"

namespace XmlNodeKeys {

	// Simulation Objects
	constexpr auto BaseNodeName = "SimObject";
	constexpr auto SourceNodeName = "Source";
	constexpr auto ServerNodeName = "Server";
	constexpr auto SinkNodeName = "Sink";

	// Simulation Object attributes
	constexpr auto TypeAttribute = "Type";
	constexpr auto ColorAttribute = "Color";
	constexpr auto XAttribute = "x";
	constexpr auto YAttribute = "y";
	constexpr auto WidthAttribute = "width";
	constexpr auto HeightAttribute = "height";
	constexpr auto InputRectAttribute = "Input Rectangle";
	constexpr auto OutputRectAttribute = "Output Rectangle";

	// Distributions
	constexpr auto DistributionAttribute = "Distribution Attribute";
	constexpr auto DistributionBaseName = "Distribution";
	constexpr auto ExponentialDist = "Exponential";
	constexpr auto UniformDist = "Uniform";
	constexpr auto TriangularDist = "Triangular";
	constexpr auto NormalDist = "Normal";
	constexpr auto PoissonDist = "Poisson";
	constexpr auto ConstantDist = "Constant";
	constexpr auto WeibullDist = "Weibull";
	constexpr auto ErlangDist = "Erlang";

	// Distribution Attributes
	constexpr auto MinAttribute = "Min";
	constexpr auto MeanAttribute = "Mean";
	constexpr auto MaxAttribute = "Max";
	constexpr auto VarianceAttribute = "Variance";
	constexpr auto ScaleAttribute = "Scale";
	constexpr auto ShapeAttribute = "Shape";

	// Versioning
	constexpr auto DocumentNodeName = "SimObjectDocument";
	constexpr auto VersionAttribute = "version";
	constexpr auto VersionValue = "0.0.2";
}

class GraphicalSource;
class GraphicalServer;
class GraphicalSink;
class Exponential;
class Uniform;
class Triangular;
class Normal;
class Poisson;
class Constant;
class Weibull;
class Erlang;

class Visitor
{
public:
	// Simulation Objects
	virtual void Visit(GraphicalSource& source) = 0;
	virtual void Visit(GraphicalServer& server) = 0;
	virtual void Visit(GraphicalSink& sink) = 0;

	// Distributions
	virtual void Visit(Exponential& expo) = 0;
	virtual void Visit(Uniform& uni) = 0;
	virtual void Visit(Triangular& tri) = 0;
	virtual void Visit(Normal& norm) = 0;
	virtual void Visit(Poisson& poisson) = 0;
	virtual void Visit(Constant& constant) = 0;
	virtual void Visit(Weibull& weibull) = 0;
	virtual void Visit(Erlang& erlang) = 0;
};

class XMLSerializingVisitor : public Visitor {

public:
	XMLSerializingVisitor(wxXmlNode* parentNode) : m_parentNode(parentNode) {}

	// Simulation Objects
	void Visit(GraphicalSource& source) override;
	void Visit(GraphicalServer& server) override;
	void Visit(GraphicalSink& sink) override;

	// Distributions
	void Visit(Exponential& expo) override;
	void Visit(Uniform& uni) override;
	void Visit(Triangular& tri) override;
	void Visit(Normal& norm) override;
	void Visit(Poisson& poisson) override;
	void Visit(Constant& constant) override;
	void Visit(Weibull& weibull) override;
	void Visit(Erlang& erlang) override;

	wxXmlNode* m_parentNode;
};

struct XMLSerializer
{
	static wxXmlDocument SerializeNodes(const std::vector<std::unique_ptr<GraphicalNode>>& nodes);
	static wxXmlNode SerializeDistribution(Distribution* dist);
};

struct XMLDeserializationFactory
{
	std::unique_ptr<GraphicalNode> DeserializeNode(const wxXmlNode* node);
	std::unique_ptr<Distribution> DeserializeDistribution(const wxXmlNode* node);

private:
	// Simulation Objects
	GraphicalSource DeserializeSource(const wxXmlNode* node);
	GraphicalServer DeserializeServer(const wxXmlNode* node);
	GraphicalSink DeserializeSink(const wxXmlNode* node);

	// Distributions
	Exponential DeserializeExponential(const wxXmlNode* node);
	Uniform DeserializeUniform(const wxXmlNode* node);
	Triangular DeserializeTriangular(const wxXmlNode* node);
	Normal DeserializeNormal(const wxXmlNode* node);
	Poisson DeserializePoisson(const wxXmlNode* node);
	Constant DeserializeConstant(const wxXmlNode* node);
	Weibull DeserializeWeibull(const wxXmlNode* node);
	Erlang DeserializeErlang(const wxXmlNode* node);
};