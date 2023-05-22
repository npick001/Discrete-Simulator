#pragma once

#include <wx/dataobj.h>
#include "GenericNode.h"

#ifdef __WXMSW__
    #include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

class NodeDataObject : public wxCustomDataObject {
public:

    NodeDataObject(GenericNode* node)
        : wxCustomDataObject(wxDataFormat("GenericNode")) {
        SetData(sizeof(GenericNode*), &node);
    }

    GenericNode* GetNode() const {
        GenericNode* node;
        memcpy(&node, GetData(), sizeof(GenericNode*));
        return node;
    }
};