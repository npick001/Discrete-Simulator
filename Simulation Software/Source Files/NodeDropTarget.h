#pragma once

#include <wx/wx.h>
#include <wx/dnd.h>
#include "GenericNode.h"
//#include "Canvas.h"

class Canvas;

class NodeDropTarget : public wxDropTarget {
public:
    NodeDropTarget(Canvas* canvas);
    ~NodeDropTarget();
    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def) override;

private:
    Canvas* _canvas;
};