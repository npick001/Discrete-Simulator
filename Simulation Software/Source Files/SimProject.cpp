#include "SimProject.h"

SimProject::SimProject(Canvas* canvas)
	: m_canvas(canvas) 
{
	m_modelTimeUnit = MINUTES;
	m_canvas->SetSimulationProject(this);
	m_hasBeenBuilt = false;
}

void SimProject::SetCanvas(Canvas* canvas)
{
	m_canvas = canvas;
	m_canvas->SetSimulationProject(this);
}

Canvas& SimProject::ViewCanvas()
{
	Canvas& canvas_reference = *m_canvas;
	return canvas_reference;
}

void SimProject::Build() {
	// Generate digraph describing what nodes connect to what other nodes
	// Use digraph to instantiate and set connections between specific nodes such as source, server, sink, etc.

	// CURRENTLY USES A LINEAR BUILD
	// GRAPHICS OBJECTS MUST HAVE 1 INPUT AND 1 OUTPUT ONLY
	m_instantiatedNodes.clear();
	m_nodeMap.clear();

	Set<GraphicalNode> gnodes = m_canvas->GetSimObjects();
	GraphicalNode* currentNode = gnodes.GetFirst();
	GenericNode* previousNode = nullptr;

	while (currentNode != nullptr) {

		// create a simobj node of that type 
		GenericNode* simObj = NodeFactory::CreateSimObject(currentNode->GetNodeType());
		m_instantiatedNodes.push_back(simObj);
		m_nodeMap.insert({ currentNode, simObj });

		// make connections
		if (previousNode != nullptr) {
			previousNode->SetNext(m_nodeMap[currentNode]);
			m_nodeMap[currentNode]->SetPrevious(previousNode);
		}

		// keep track of last one for linking
		previousNode = simObj;
		
		// move on
		currentNode = currentNode->GetNext();
	}
	m_hasBeenBuilt = true;
}

bool SimProject::CheckBuildViability()
{
	// linear build 
	// basically traversing linked list
	// make sure all connections are there
	for (int i = 0; i < m_instantiatedNodes.size(); i++) {

		auto currentNode = m_instantiatedNodes[i];

		switch (currentNode->GetType()) {
		case GenericNode::SOURCE:

			if (currentNode->GetNext() == nullptr) {
				wxLogError("SOURCE NODE NEEDS A DESTINATION FOR GENERATED ENTITIES.\n Consider checking connections.");
				m_hasBeenBuilt = false;
				m_instantiatedNodes.clear();
				m_nodeMap.clear();

				return false;
			}

			break;
		case GenericNode::SINK:

			if (currentNode->GetPrevious() == nullptr) {
				wxLogWarning("SINK NODE DOES NOT HAVE AN INPUT CONNECTION.\n Consider checking connections.");
			}

			break;
		default:

			if ((currentNode->GetNext() == nullptr)) {
				wxLogError("SERVER NODE NEEDS AN OUTPUT CONNECTION.\n Consider checking connections.");
				m_hasBeenBuilt = false;
				m_instantiatedNodes.clear();
				m_nodeMap.clear();

				return false;
			}
			else if ((currentNode->GetPrevious() == nullptr)) {
				wxLogMessage("A SERVER NODE HAS NO INPUT CONNECTION.\n Consider checking connections.");
			}
			else if ((currentNode->GetNext() == nullptr) && (currentNode->GetPrevious() == nullptr)) {
				wxLogError("SERVER NODE NEEDS BOTH INPUT AND OUTPUT CONNECTIONS.\n Consider checking connections.");
				m_hasBeenBuilt = false;
				m_instantiatedNodes.clear();
				m_nodeMap.clear();

				return false;
			}
			break;
		}
	}
	return true;
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

bool SimProject::HasBeenBuilt()
{
	return m_hasBeenBuilt;
}

void SimProject::RegisterNewConnection(GraphicalNode* from, GraphicalNode* to)
{
	m_nodeMap[from]->SetNext(m_nodeMap[to]);
	m_nodeMap[to]->SetPrevious(m_nodeMap[from]);
}

void SimProject::RegisterNodeDeletion(GraphicalNode* deleted)
{
	auto simNode = m_nodeMap[deleted];
	delete simNode;
	m_nodeMap.erase(deleted);
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
