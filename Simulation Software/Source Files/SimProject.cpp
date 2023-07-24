#include "SimProject.h"
#include "GraphicalNode.h"

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

	m_instantiatedNodes.clear();
	m_nodeMap.clear();

	Set<GraphicalNode> roots;
	Set<GraphicalNode> gnodes = m_canvas->GetSimObjects();
	GenericNode* previousNode = nullptr;

	// find the roots of the graph
	while (!gnodes.IsEmpty()){

		GraphicalNode* currentNode = gnodes.GetFirst();

		// sources will be the roots
		// so, the only nodes with no previous nodes
		if (currentNode->GetPrevious().IsEmpty()) {
			roots.Add(currentNode);
		}
	}

	Set<GraphicalNode> rootsForLinking = roots;

	// build all the roots children
	while (!roots.IsEmpty()) {
		BuildChildren(roots.GetFirst(), nullptr);
	}

	LinkChildren(rootsForLinking);

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

			if (currentNode->GetNext().IsEmpty()) {
				wxLogError("SOURCE NODE NEEDS A DESTINATION FOR GENERATED ENTITIES.\n Consider checking connections.");
				m_hasBeenBuilt = false;
				m_instantiatedNodes.clear();
				m_nodeMap.clear();

				return false;
			}

			break;
		case GenericNode::SINK:

			if (currentNode->GetPrevious().IsEmpty()) {
				wxLogWarning("SINK NODE DOES NOT HAVE AN INPUT CONNECTION.\n Consider checking connections.");
			}

			break;
		default:

			if ((currentNode->GetNext().IsEmpty())) {
				wxLogError("SERVER NODE NEEDS AN OUTPUT CONNECTION.\n Consider checking connections.");
				m_hasBeenBuilt = false;
				m_instantiatedNodes.clear();
				m_nodeMap.clear();

				return false;
			}
			else if ((currentNode->GetPrevious().IsEmpty())) {
				wxLogMessage("A SERVER NODE HAS NO INPUT CONNECTION.\n Consider checking connections.");
			}
			else if ((currentNode->GetNext().IsEmpty()) && (currentNode->GetPrevious().IsEmpty())) {
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

wxAuiNotebook* SimProject::CreateStatisticsGraphs(wxWindow* parent)
{
	wxAuiNotebook* graphs = new wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	for (int i = 0; i < m_instantiatedNodes.size(); i++) {
		
		std::vector<wxString> x_labels;

		std::vector<Time> eventTimes;
		std::vector<double> states;

		ServerNode* server;
		ChartControl* statesOverTimeChart;
		std::map<double, bool> eventTimeSkimmingMap;
		std::vector<double> uniqueEventTimes;

		switch (m_instantiatedNodes[i]->GetType())
		{
		case GenericNode::SOURCE:

			break;

		case GenericNode::SERVER:
			server = (ServerNode*)m_instantiatedNodes[i];

			eventTimes = server->GetStateChangeTimes();

			// create all the statistics graphs
			states = server->GetStatesOverTime();

			// skim non unique times from list
			for (int j = 0; j < eventTimes.size(); j++) {
				eventTimeSkimmingMap.insert({ eventTimes[j], true });
			}

			// create x-axis labels for states over time chart
			for (auto const& event : eventTimeSkimmingMap) {
				x_labels.push_back(wxString::FromDouble(event.first));
			}

			statesOverTimeChart = new ChartControl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);
			statesOverTimeChart->m_title = "Server " + std::to_string(server->GetID()) + " states over time";
			statesOverTimeChart->m_x_axis_labels = x_labels;
			statesOverTimeChart->m_xvalues = eventTimes;
			statesOverTimeChart->m_yvalues = states;

			graphs->AddPage(statesOverTimeChart, "ServerStates", true);

			break;

		case GenericNode::SINK:

			break;
		}

	}

	return graphs;
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
	// make sure theyve both been built, if not then return
	if (m_nodeMap[from] == nullptr || m_nodeMap[to] == nullptr) {
		m_hasBeenBuilt = false;
		return;
	}

	m_nodeMap[from]->AddNext(m_nodeMap[to]);
	m_nodeMap[to]->AddPrevious(m_nodeMap[from]);
}

void SimProject::RegisterNodeDeletion(GraphicalNode* deleted)
{
	auto simNode = m_nodeMap[deleted];
	delete simNode;
	m_nodeMap.erase(deleted);
}

void SimProject::BuildChildren(GraphicalNode* node, GraphicalNode* previous)
{
	// if node is in map, just make connections
	// we just want a unique list of SimObjs
	if (m_nodeMap[node] != nullptr) {
		return;
	}
	else {
		// create sim obj
		GenericNode* simObj = NodeFactory::CreateSimObject(node->GetNodeType());

		auto previousNodes = node->GetPrevious();

		SourceNode* src;
		ServerNode* server;
		SinkNode* sink;

		GraphicalSource* gsource;
		GraphicalServer* gserver;
		GraphicalSink* gsink;

		switch (node->GetNodeType())
		{
		case GenericNode::SOURCE:

			gsource = (GraphicalSource*)node;
			src = (SourceNode*)simObj;

			// Link data
			src->SetEntityToGenerate(gsource->GetEntity());
			src->SetIATime(gsource->GetIATime());
			src->SetNumberToGenerate(gsource->GetNumberToGenerate());

			m_nodeMap[node] = src;
			m_instantiatedNodes.push_back(src);

			break;

		case GenericNode::SERVER:

			gserver = (GraphicalServer*)node;
			server = (ServerNode*)simObj;

			server->SetServiceTime(gserver->GetServiceTime());
			server->SetNumResources(gserver->GetNumResources());

			m_nodeMap[node] = server;
			m_instantiatedNodes.push_back(server);

			break;

		case GenericNode::SINK:

			gsink = (GraphicalSink*)node;
			sink = (SinkNode*)simObj;

			m_nodeMap[node] = sink;
			m_instantiatedNodes.push_back(sink);

			break;
		}
	}

	// check if GetNext() is empty, if so then break
	if (node->GetNext().IsEmpty()) {
		return;
	}
	else {
		Set<GraphicalNode> children = node->GetNext();

		// loop thru all nodes in the children and recursively call this fn
		// is top heavy and need to balance more for breadth
		while (!children.IsEmpty()){
			BuildChildren(children.GetFirst(), node);
		}
	}
}

void SimProject::LinkChildren(Set<GraphicalNode> roots)
{
	Set<GraphicalNode> rootz = roots;
	while (!rootz.IsEmpty()) {

		GraphicalNode* thisNode = rootz.GetFirst();
		Set<GraphicalNode> children = thisNode->GetNext();
		Set<GraphicalNode> nextRoots = thisNode->GetNext();

		while (!children.IsEmpty()) {

			GraphicalNode* thisChild = children.GetFirst();
			m_nodeMap[thisNode]->AddNext(m_nodeMap[thisChild]);
			m_nodeMap[thisChild]->AddPrevious(m_nodeMap[thisNode]);
		}

		if (!nextRoots.IsEmpty()) {
			LinkChildren(nextRoots);
		}
	}
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
		createdNode = new ServerNode();
		break;
	}

	return createdNode;
}
