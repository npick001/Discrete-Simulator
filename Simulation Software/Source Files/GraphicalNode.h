#pragma once

#include <list>
#include <string>

#include "wx/wx.h"
#include "wx/graphics.h"
#include "wx/propgrid/propgrid.h"

#include "SimulationExecutive.h"
#include "Set.h"
#include "GenericNode.h"
#include "GraphicalElement.h"
#include "GraphicalEdge.h"
#include "Action.h"

class GraphicalEdge;
//class MoveNodeAction;
class NodeFactory;
class SimProperties;

class GraphicalNode : public GraphicalElement {
public:
	GenericNode::Type GetNodeType() const
		{ return m_nodeType; }

	GraphicalNode();
	GraphicalNode(ElementKey id);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(const GraphicalNode& other);

	// Also disconnects attached edges, preparing them for deletion
	~GraphicalNode();

	// all specific nodes should implement their own drawing for different pictures
	void Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc);
	virtual void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) = 0;

	GraphicalNode& operator=(const GraphicalNode& other);

	// position accessors
	wxPoint2DDouble GetPosition();
	void SetPosition(const wxPoint2DDouble& position);
	wxAffineMatrix2D GetTransform() const;

	// IO container getters
	std::list<GraphicalEdge*> GetOutputs() const;
	std::list<GraphicalEdge*> GetInputs() const;

	// for property viewer
	Set<wxPGProperty> GetProperties();

	void DisconnectOutputs();
	void DisconnectInputs();

	Selection Select(const wxAffineMatrix2D& camera,
		wxPoint2DDouble clickPosition) override;

	void Move(wxPoint2DDouble displacement);

	void SetBodyColor(const wxColor& color);
	void SetNodeType(GenericNode::Type type);

	// PROPERTY REPORTING DESIGN PATTERN
	class PropertiesWrapper {
	public:
		PropertiesWrapper(int id) : m_id(id) {}
		virtual void ReportProperties() = 0;

	private:
		int m_id;
	};

	virtual std::unique_ptr<PropertiesWrapper> GetSimProperties() = 0;

protected:
	friend class GraphicalEdge;
	GenericNode::Type m_nodeType;

	/// <SimProperties>
	/// This is the same design pattern as Statistics from the simulation code.
	/// Idea is that the parent class defines a properties object that is populated by the
	/// instances of this object. Any handlers using the parent class
	/// can thus access instantiated object properties
	class SimProperties
	{
	protected:
		inline SimProperties() {}
	};

	// list of wxProperties that can be used and displayed (hopefully)
	Set<wxPGProperty> m_properties;

	// graphical characteristics
	wxColor m_bodyColor;
	wxRect2DDouble m_bodyShape;
	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;
	wxPoint2DDouble m_position;
	wxSize m_bodySize;
	wxSize m_ioSize;
	wxColor m_labelColor;
	wxColor m_ioColor;

	// link to graphical edges
	std::list<GraphicalEdge*> m_inputs;
	std::list<GraphicalEdge*> m_outputs;

	// Returns the points at which an edge should be drawn between
	// Points are in world coordinates
	wxPoint2DDouble GetOutputPoint() const;
	wxPoint2DDouble GetInputPoint() const;

private:

};

typedef SpecificElementContainer<GraphicalNode> NodeMap;

/********************************************/
/* SourceNode:                              */
/* defines an entity generating object      */
/* provides basic entity creation			*/
/********************************************/
class GraphicalSource : public GraphicalNode {
public:
	GraphicalSource();
	GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center);

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	class SourceProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

protected:
	class MyProperties;

private:
	// will be replaced with a distribution later
	int m_arrivalTime;

	SimProperties* m_myProps;
};

/****************************************************************/
/* GraphicalServer:                                             */
/* defines a single server single queue object                  */
/* provides basic server functionality							*/
/* the server originates as a basic rectangle in the Canvas     */
/****************************************************************/
class GraphicalServer : public GraphicalNode {
public:
	GraphicalServer();
	GraphicalServer(ElementKey id, wxWindow* window, wxPoint2DDouble center);

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	class ServerProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

protected:
	class MyProperties;

private:
	// will be replaced with a distribution later
	double m_serviceTime;

	// will be replaced with a time unit later (second, minute, hour, day, year)
	TimeUnit m_timeUnit;
	int m_numResources;

	SimProperties* m_myProps;
};

/*****************************************/
/* Sink Node:                            */
/* defines an entity deletion object     */
/* the sink object provides statistics	 */
/*****************************************/
class GraphicalSink : public GraphicalNode {
public:
	GraphicalSink();
	GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center);

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	class SinkProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

protected:
	class MyProperties;

private:
	SimProperties* m_myProps;
};