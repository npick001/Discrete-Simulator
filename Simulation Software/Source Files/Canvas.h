#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include "NodeDropTarget.h"

#ifdef __WXMSW__
	#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

enum {
	ID_LeftClickUp,
	ID_LeftClickDown
};

class Canvas : public wxScrolledWindow
{
public:
	Canvas(wxWindow* parent);
	~Canvas();

	void AddNode(GenericNode* node, int x, int y);
private:

	// Canvas Events
	void OnLeftClickDown(wxMouseEvent& event);
	void OnLeftClickUp(wxMouseEvent& event);

	NodeDropTarget* _dropTarget;
	std::vector<GenericNode*> _nodeInstances;
};
