#pragma once
#include "Utility.h"

#include <memory>
#include <list>
#include <string>

#include "wx/wx.h"
#include "wx/graphics.h"
#include "wx/propgrid/propgrid.h"

#include "Set.h"
#include "SimulationExecutive.h"
#include "GenericNode.h"
#include "Distribution.h"
#include "GraphicalElement.h"
#include "GraphicalEdge.h"
#include "XMLSerialization.h"

class GraphicalEdge;
class NodeFactory;
class SimProperties;
class SimProject;

class GraphicalNode : public GraphicalElement {
public:
	void SetNodeType(GenericNode::Type type);
	GenericNode::Type GetNodeType() const
		{ return m_nodeType; }

	GraphicalNode();
	GraphicalNode(ElementKey id);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	GraphicalNode(ElementKey id, wxWindow* window, wxPoint2DDouble center, const std::string& _text);
	GraphicalNode(const GraphicalNode& other);
	virtual std::unique_ptr<GraphicalNode> Clone() = 0;

	// Also disconnects attached edges, preparing them for deletion
	~GraphicalNode();

	void AddNext(GraphicalNode* next);
	void AddPrevious(GraphicalNode* previous);
	Set<GraphicalNode> GetNext();
	Set<GraphicalNode> GetPrevious();

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
	void ShiftSizerPositions(int selectedSizer, wxPoint2DDouble displacement);
	int GetSelectedSizerIndex(wxPoint2DDouble clickPosition);
	int GetSelectedSizer();

	wxColor GetBodyColor();
	void SetBodyColor(const wxColor& color);

	// XML Serialization
	virtual void Accept(Visitor& visitor) = 0;

	wxSize GetSize();
	wxPoint2DDouble GetTopLeft();
	wxPoint2DDouble GetCenter();
	void SetBodyShape(wxRect2DDouble newBody);
	wxRect2DDouble GetBodyShape();
	void SetTransformationMatrix(wxAffineMatrix2D transform);
	wxAffineMatrix2D GetTransformationMatrix();
	wxPoint2DDouble GetTransformedPoint(wxPoint2DDouble toTransform);
	void SetInputRect(wxRect2DDouble newInput);
	wxRect2DDouble GetInputRect();
	void SetOutputRect(wxRect2DDouble newOutput);
	wxRect2DDouble GetOutputRect();
	void SetIOColor(wxColor newColor);
	wxColor GetIOColor();

protected:
	friend class GraphicalEdge;
	GenericNode::Type m_nodeType;

	Set<GraphicalNode> m_next;
	Set<GraphicalNode> m_previous;

	// list of wxProperties that can be used and displayed
	Set<wxPGProperty> m_properties;

	// graphical characteristics
	wxColor m_bodyColor;
	wxColor m_labelColor;
	wxColor m_ioColor;
	wxColor m_sizerColor;
	wxRect2DDouble m_bodyShape;
	wxAffineMatrix2D m_transformation;
	wxRect2DDouble m_inputRect;
	wxRect2DDouble m_outputRect;
	wxPoint2DDouble m_position;
	wxRect2DDouble m_sizers[4];
	int m_sizerSelected;
	wxSize m_bodySize;
	wxSize m_ioSize;
	wxSize m_sizerSize;

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
	GraphicalSource(const GraphicalSource& other);
	GraphicalSource(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	~GraphicalSource();
	std::unique_ptr<GraphicalNode> Clone() override;

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	// Simulation values
	void SetEntity(Entity* e);
	void SetIATime(Distribution* iaTime);
	void SetNumberToGenerate(int numgen);
	Entity* GetEntity();
	Distribution* GetIATime();
	int GetNumberToGenerate();

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	friend class SimProject;

private:

	// Entity generation members
	Distribution* m_iaTime;
	Entity* m_entity;
	int m_numberToGenerate;
	bool m_infiniteGeneration;
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
	GraphicalServer(const GraphicalServer& other);
	GraphicalServer(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	~GraphicalServer();

	std::unique_ptr<GraphicalNode> Clone() override;

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	// Simulation values
	void SetServiceTime(Distribution* serviceTime);
	void SetNumResources(int resources);
	Distribution* GetServiceTime();
	int GetNumResources();

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	friend class SimProject;

private:
	// will be replaced with a distribution later
	//double m_serviceTime;
	Distribution* m_serviceTime;

	// time unit (second, minute, hour, day, year)
	TimeUnit m_timeUnit;
	int m_numResources;
};

/*****************************************/
/* Sink Node:                            */
/* defines an entity deletion object     */
/* the sink object provides statistics	 */
/*****************************************/
class GraphicalSink : public GraphicalNode {
public:
	GraphicalSink();
	GraphicalSink(const GraphicalSink& other);
	GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	std::unique_ptr<GraphicalNode> Clone() override;

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	friend class SimProject;

	class MyProperties;

private:
};