#pragma once
#include <wx/wx.h>
#include <wx/dnd.h>
#include "GenericNode.h"
#include "NodeDataObject.h"

enum {
    ID_NodeBeginDrag
};

class SimObjectLibrary : public wxPanel {
public:
    SimObjectLibrary(wxWindow* parent);
    ~SimObjectLibrary();
    void AddNode(GenericNode* node);

    inline wxSize GetSize() { return m_size; }

private:
    wxBoxSizer* m_sizer;
    std::vector<GenericNode*> _nodes;

    wxSize m_size;

    // Toolbar events
    void OnNodeBeginDrag(wxMouseEvent& event);
};