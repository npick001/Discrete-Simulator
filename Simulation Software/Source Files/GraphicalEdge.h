#pragma once

#include "wx/dcbuffer.h"
#include "wx/wx.h"

#include "GraphicalElement.h"
#include "GraphicalNode.h"

class GraphicalNode;

class GraphicalEdge : public GraphicalElement {
private:
	friend class GraphicalNode;

	GraphicalNode* m_source;
	GraphicalNode* m_destination;

	wxPoint2DDouble m_sourcePoint;
	wxPoint2DDouble m_destinationPoint;

public:
	GraphicalEdge(ElementKey id);
	GraphicalEdge(ElementKey id, GraphicalNode* source, GraphicalNode* destination);

	~GraphicalEdge();
	
	// Used to connect a source or destination
	void ConnectSource(GraphicalNode* source);
	void ConnectDestination(GraphicalNode* destination);

	// Disconnect both the source and destination
	void Disconnect();

	// Used to set the point during incomplete connection state
	// Also used by GraphicalNode::Move to update edge points
	inline void SetSourcePoint(wxPoint2DDouble sourcePoint)
		{ m_sourcePoint = sourcePoint; }
	inline void SetDestinationPoint(wxPoint2DDouble destinationPoint)
		{ m_destinationPoint = destinationPoint; }

	void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const override;

	inline const wxPoint2DDouble& GetSourcePoint() const { return m_sourcePoint; }
	inline const wxPoint2DDouble& GetDestinationPoint() const { return m_destinationPoint; }

	inline bool operator==(const GraphicalEdge& other) const {
		return m_id == other.m_id;
	}

private:
	static const wxColor ms_labelColor;
};

