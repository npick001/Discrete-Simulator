#pragma once
#include "Utility.h"

#include <memory>
#include <list>
#include <string>

#include "wx/wx.h"
#include "wx/graphics.h"
#include "wx/propgrid/propgrid.h"

#include "SimulationExecutive.h"
#include "Distribution.h"
#include "Set.h"
#include "GenericNode.h"
#include "GraphicalElement.h"
#include "GraphicalEdge.h"
#include "XMLSerialization.h"

class GraphicalEdge;
class NodeFactory;
class SimProperties;

struct Transformation
{
	double translationX = 0.0;
	double translationY = 0.0;
	double rotationAngle = 0.0;
	double scaleX = 1.0;
	double scaleY = 1.0;
};

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

	void SetNext(GraphicalNode* next);
	void SetPrevious(GraphicalNode* previous);
	GraphicalNode* GetNext();
	GraphicalNode* GetPrevious();

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

	// PROPERTY REPORTING DESIGN PATTERN
	class PropertiesWrapper {
	public:
		PropertiesWrapper(int id) : m_id(id) {}
		virtual void ReportProperties() = 0;

	private:
		int m_id;
	};

	virtual std::unique_ptr<PropertiesWrapper> GetSimProperties() = 0;

	// XML Serialization
	virtual void Accept(Visitor& visitor) = 0;

	wxSize GetSize();
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

	GraphicalNode* m_next;
	GraphicalNode* m_previous;

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

	class SourceProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

	void SetIATime(Distribution* iaTime);
	Distribution* GetIATime();

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	class MyProperties;

private:
	// will be replaced with a distribution later
	int m_arrivalTime;
	Distribution* m_iaTime;

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
	GraphicalServer(const GraphicalServer& other);
	GraphicalServer(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	~GraphicalServer();

	std::unique_ptr<GraphicalNode> Clone() override;

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	class ServerProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

	void SetServiceTime(Distribution* serviceTime);
	Distribution* GetServiceTime();

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	class MyProperties;

private:
	// will be replaced with a distribution later
	//double m_serviceTime;
	Distribution* m_serviceTime;

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
	GraphicalSink(const GraphicalSink& other);
	GraphicalSink(ElementKey id, wxWindow* window, wxPoint2DDouble center);
	std::unique_ptr<GraphicalNode> Clone() override;

	void MyDraw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	class SinkProperties;
	std::unique_ptr<PropertiesWrapper> GetSimProperties() override;

	// XML Serialization
	void Accept(Visitor& visitor) override;

protected:
	class MyProperties;

private:
	SimProperties* m_myProps;
};