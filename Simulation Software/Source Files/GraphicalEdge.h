#pragma once
#include "Utility.h"

#include "wx/graphics.h"

#include "GraphicalElement.h"
#include "GraphicalNode.h"
#include "XMLSerialization.h"

class GraphicalNode;

class GraphicalEdge : public GraphicalElement {
private:
	friend class GraphicalNode;

	GraphicalNode* m_source;
	GraphicalNode* m_destination;

	ElementKey m_sourceID;
	ElementKey m_destinationID;

	wxPoint2DDouble m_sourcePoint;
	wxPoint2DDouble m_destinationPoint;

public:
	GraphicalEdge();
	GraphicalEdge(ElementKey id);
	GraphicalEdge(ElementKey id, GraphicalNode* source, GraphicalNode* destination);
	GraphicalEdge(const GraphicalEdge& other);
	std::unique_ptr<GraphicalEdge> Clone();

	GraphicalEdge& operator=(const GraphicalEdge& other);

	~GraphicalEdge();

	void Accept(Visitor& visitor);
	
	// Used to connect a source or destination
	void ConnectSource(GraphicalNode* source);
	ElementKey GetSourceID();
	void SetSourceID(ElementKey id);
	void ConnectDestination(GraphicalNode* destination);
	ElementKey GetDestinationID();
	void SetDestinationID(ElementKey id);


	// Disconnect both the source and destination
	void Disconnect();

	// Used to set the point during incomplete connection state
	// Also used by GraphicalNode::Move to update edge points
	inline void SetSourcePoint(wxPoint2DDouble sourcePoint)
		{ m_sourcePoint = sourcePoint; }
	inline void SetDestinationPoint(wxPoint2DDouble destinationPoint)
		{ m_destinationPoint = destinationPoint; }

	void Draw(const wxAffineMatrix2D& camera, wxGraphicsContext* gc) override;

	Selection Select(const wxAffineMatrix2D& camera,
		wxPoint2DDouble clickPosition) override;

	inline const wxPoint2DDouble& GetSourcePoint() const { return m_sourcePoint; }
	inline const wxPoint2DDouble& GetDestinationPoint() const { return m_destinationPoint; }

	inline bool operator==(const GraphicalEdge& other) const {
		return m_id == other.m_id;
	}

private:
	static const wxColor ms_labelColor;
};

typedef SpecificElementContainer<GraphicalEdge> EdgeMap;