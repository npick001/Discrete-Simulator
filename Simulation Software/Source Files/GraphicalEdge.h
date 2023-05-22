#pragma once

#include "wx/dcbuffer.h"
#include "wx/wx.h"

#include "GraphicalNode.h"

class GraphicalNode;

class GraphicalEdge {
public:
	GraphicalEdge();
	GraphicalEdge(GraphicalNode* source, GraphicalNode* destination);

	void SetSource(GraphicalNode* source);
	void SetDestination(GraphicalNode* destination);

	void RemoveSource();
	void RemoveDestination();

	// Used to set the point during incomplete connection state
	// Also used by GraphicalNode::Move to update edge points
	inline void SetSourcePoint(wxPoint2DDouble sourcePoint)
		{ m_sourcePoint = sourcePoint; }
	inline void SetDestinationPoint(wxPoint2DDouble destinationPoint)
		{ m_destinationPoint = destinationPoint; }

	void Draw(wxAffineMatrix2D camera, wxGraphicsContext* gc) const;

	// DEBUG code
	inline wxPoint2DDouble GetSourcePoint() const { return m_sourcePoint; }
	inline wxPoint2DDouble GetDestinationPoint() const { return m_destinationPoint; }

private:
	GraphicalNode* m_source;
	GraphicalNode* m_destination;

	wxPoint2DDouble m_sourcePoint;
	wxPoint2DDouble m_destinationPoint;
};

