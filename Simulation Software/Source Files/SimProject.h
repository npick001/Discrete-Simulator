#pragma once
#include "Utility.h"

#include <vector>
#include <map>

#include "SimulationExecutive.h"
#include "SpecificNodes.h"
#include "GraphicalNode.h"
#include "Canvas.h"

class GenericNode;
class GraphicalNode;

class GraphicalSource;
class GraphicalServer;
class GraphicalSink;

class Canvas;

/***************************************

Define the MVC Controller class.

- The Model-View-Controller Design Pattern

- The key roles of a SimProject are:

	-- build the simulation code from user graphical input
		--- let user know if the graphical model is a valid configuration.
	
	-- execute the built simulation code
		--- UI will go through controller to control simulation execution

	-- maintain a Canvas
		--- which contains the model graphical information

	-- manage data flow
		--- obtaining current simulation state from the Model
		--- sending simulation state information to View (UI for User)

- Class members are broken up into Model and View sections

***************************************/
class SimProject {
public:
	SimProject(Canvas* canvas);

	void SetCanvas(Canvas* canvas);
	Canvas& ViewCanvas();

	/// <Build>
	/// Take graphical nodes and generate 
	/// simulation code
	void Build();
	bool CheckBuildViability();
	void Run();
	void WriteStatistics();

	void SetTimeUnit(TimeUnit newUnit);
	bool HasBeenBuilt();

	void RegisterNewConnection(GraphicalNode* from, GraphicalNode* to);
	void RegisterNodeDeletion(GraphicalNode* deleted);

protected:
	void RecursivelyBuildChildren(GraphicalNode* node, GraphicalNode* previous);

private:
	std::vector<std::unique_ptr<GenericNode::StatisticsWrapper>> stats;
	std::unordered_map<GraphicalNode*, GenericNode*> m_nodeMap;
	bool m_hasBeenBuilt;

	TimeUnit m_modelTimeUnit;

	// Model 
	std::vector<GenericNode*> m_instantiatedNodes;

	// View
	Canvas* m_canvas;
};

/*******************************************/
/* Node Factory:                           */
/* defines an node creation object         */
/* the factory's whole job is to generate  */
/* the specified node and return a pointer */
/*******************************************/
class NodeFactory
{
public:
	static GraphicalNode* CreateGraphicalNode(GenericNode::Type type);
	static GraphicalNode* CreateGraphicalNode(GenericNode::Type type, ElementKey id, wxWindow* window, wxPoint2DDouble center);

	static GenericNode* CreateSimObject(GenericNode::Type type);

private:

};