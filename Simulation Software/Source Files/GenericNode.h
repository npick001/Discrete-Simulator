#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#ifdef __WXMSW__
    #include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

class GenericNode
{
public:
    GenericNode(const wxString& name, const wxString& imagePath);
    GenericNode(const GenericNode& other);
    ~GenericNode();

    wxString GetName();
    wxString GetImagePath();
    void SetNext(GenericNode* next);
    void SetPrev(GenericNode* prev);

    void Draw(wxGraphicsContext* gc);
    void InstantiateNode(int x, int y, wxSize size);

private:
    wxString _name;
    wxString _imagePath;
    wxPoint _position;
    wxSize _size;
    GenericNode* _next;
    GenericNode* _prev;
};

class Connection
{
public:
    Connection(GenericNode* startBlock, GenericNode* endBlock)
        : _startNode(startBlock), _endNode(endBlock) {}

    ~Connection();

    void Draw(wxGraphicsContext* gc) {
   /*     wxPoint startPoint = _startNode->GetPosition() + wxSize(_startNode->GetSize().x, _startNode->GetSize().y / 2);
        wxPoint endPoint = _endNode->GetPosition() + wxSize(0, _endNode->GetSize().y / 2);

        gc->SetPen(wxPen(*wxBLACK, 2));
        gc->StrokeLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
   */ }

private:
    GenericNode* _startNode;
    GenericNode* _endNode;
};

