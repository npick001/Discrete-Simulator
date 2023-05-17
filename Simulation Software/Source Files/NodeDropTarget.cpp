#include "NodeDropTarget.h"
#include "Canvas.h"

NodeDropTarget::NodeDropTarget(Canvas* canvas) : wxDropTarget(new wxCustomDataObject(wxDataFormat(wxT("GenericNode*")))) {
    _canvas = canvas;
}

NodeDropTarget::~NodeDropTarget() {}

wxDragResult NodeDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def) {
    if (!GetData()) {
        return wxDragNone;
    }

    //GenericNode* node = *((GenericNode**)((wxCustomDataObject*)m_dataObject)->GetData());

    //// Create a new instance of GenericNode using the copy constructor
    //GenericNode* newNode = new GenericNode(*node);

    //// Instantiate the node at the drop position with the desired size
    //wxSize desiredSize(50, 50); // Set the desired size here
    //newNode->InstantiateNode(x, y, desiredSize);

    //// Add the node to the canvas at the drop position
    //_canvas->AddNode(newNode, x, y);

    //delete node;

    return def;
}