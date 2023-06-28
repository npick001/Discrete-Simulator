#pragma once

#include "Canvas.h"
#include "SimulationExecutive.h"
#include "SpecificNodes.h"

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

	/// <Build>
	/// Take graphical nodes and generate 
	/// simulation code
	void Build();
	void Run();

	void SetTimeUnit(TimeUnit newUnit);

private:

	TimeUnit m_modelTimeUnit;

	// Model 
	Set<GenericNode*> m_instantiatedNodes;

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