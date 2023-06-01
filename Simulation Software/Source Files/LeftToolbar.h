#pragma once
#include <wx/dnd.h>
#include "GenericNode.h"
#include "NodeDataObject.h"

enum {
    ID_NodeBeginDrag
};

class LeftToolbar : public wxPanel {
public:
    LeftToolbar(wxWindow* parent);
    ~LeftToolbar();
    void AddNode(GenericNode* node);

    inline wxSize GetSize() { return m_size; }

private:
    wxBoxSizer* m_sizer;
    std::vector<GenericNode*> _nodes;

    wxSize m_size;

    // Toolbar events
    void OnNodeBeginDrag(wxMouseEvent& event);
};