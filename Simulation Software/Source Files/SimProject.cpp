#include "SimProject.h"

SimProject::SimProject(Canvas* canvas)
	: m_canvas(canvas) 
{
	m_modelTimeUnit = MINUTES;
}

void SimProject::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
}

const Canvas& SimProject::ViewCanvas()
{
	const Canvas& canvas_reference = *m_canvas;
	return canvas_reference;
}

void SimProject::Build() {
	// Generate digraph describing what nodes connect to what other nodes
	// Use digraph to instantiate and set connections between specific nodes such as source, server, sink, etc.

	// CURRENTLY USES A LINEAR BUILD
	// GRAPHICS OBJECTS MUST HAVE 1 INPUT AND 1 OUTPUT ONLY

	Set<GraphicalNode> gnodes = m_canvas->GetSimObjects();
	GenericNode* previousNode = nullptr;

	while (!gnodes.IsEmpty()) {

		// get a graphical node
		GraphicalNode* graphicsObj = gnodes.GetFirst();

		// create a simobj node of that type
		GenericNode* simObj = NodeFactory::CreateSimObject(graphicsObj->GetNodeType());
		m_instantiatedNodes.push_back(simObj);

		// make connections
		if (previousNode != nullptr) {
			previousNode->SetNext(simObj);
		}

		// keep track of last one for linking
		previousNode = simObj;
	}
}

void SimProject::Run()
{
	RunSimulation();
}

void SimProject::WriteStatistics()
{
	for (int i = 0; i < m_instantiatedNodes.size(); i++) {
		stats.push_back(m_instantiatedNodes[i]->GetStatistics());
		stats[i]->ReportStats();
	}
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
