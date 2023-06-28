#pragma once
#include <wx/wx.h>
#include <wx/dnd.h>
#include "GraphicalNode.h"
#include "NodeDataObject.h"

enum {
    ID_NodeBeginDrag
};

class SimObjectLibrary : public wxPanel {
public:
    SimObjectLibrary(wxWindow* parent);
    ~SimObjectLibrary();
    void AddNode(GraphicalNode* node);

    inline wxSize GetSize() { return m_size; }

private:
    wxBoxSizer* m_sizer;
    std::vector<GraphicalNode*> _nodes;

    wxSize m_size;

    // Toolbar events
    void OnNodeBeginDrag(wxMouseEvent& event);
};