#include "SimProject.h"

SimProject::SimProject(Canvas* canvas)
	: m_canvas(canvas) 
{
	m_modelTimeUnit = MINUTES;
}

void SimProject::Build() {
	// Generate digraph describing what nodes connect to what other nodes
	// Use digraph to instantiate and set connections between specific nodes such as source, server, sink, etc.

	NodeMap gnodes = m_canvas->GetSimObjects();


}

void SimProject::Run()
{
	RunSimulation();
}

void SimProject::SetTimeUnit(TimeUnit newUnit)
{
	m_modelTimeUnit = newUnit;
}


GraphicalNode* NodeFactory::CreateGraphicalNode(GenericNode::Type type)
{
	GraphicalNode* createdNode = nullptr;

	switch (type) {
	case GenericNode::SOURCE:
		createdNode = new GraphicalSource();
		break;
	case GenericNode::SINK:
		createdNode = new GraphicalSink();
		break;
	default:
		createdNode = new GraphicalServer();
		break;
	}

	return createdNode;
}

GraphicalNode* NodeFactory::CreateGraphicalNode(GenericNode::Type type, ElementKey id, wxWindow* window, wxPoint2DDouble center)
{
	GraphicalNode* createdNode = nullptr;

	switch (type) {
	case GenericNode::SOURCE:
		createdNode = new GraphicalSource(id, window, center);
		break;
	case GenericNode::SINK:
		createdNode = new GraphicalSink(id, window, center);
		break;
	default:
		createdNode = new GraphicalServer(id, window, center);
		break;
	}

	return createdNode;
}

GenericNode* NodeFactory::CreateSimObject(GenericNode::Type type)
{
	GenericNode* createdNode = nullptr;

	switch (type) {
	case GenericNode::SOURCE:
		createdNode = new SourceNode();
		break;
	case GenericNode::SINK:
		createdNode = new SinkNode();
		break;
	default:
		createdNode = new SSSQ();
		break;
	}

	return createdNode;
}
