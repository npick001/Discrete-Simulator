#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "Directives.h"
#include "Entity.h"

class GenericNode
{
public:
    void SetNext(GenericNode* next);
    void SetPrevious(GenericNode* prev);
    void SetImagePath(const wxString& imagePath);
    wxString GetName();
    wxString GetImagePath();    
    inline int GetID() { return m_id; }

    // At (x, y) instantiate a node of specified size
    void InstantiateNode(int x, int y, wxSize size);

    // Have entity begin arrival logic
    void Arrive(Entity* entity);

protected:
    GenericNode(const wxString& name);
    GenericNode(const GenericNode& other);
    ~GenericNode();

    GenericNode* m_next;

    /*
        Depart entity from this node
        This function sets the entity source and then sends 
        it to the next node
    */
    void Depart(Entity* entity);

    // Node process handler
    virtual void NodeProcess(Entity* entity) = 0;

private:
    int m_id;
    static int m_nextID;
    wxString m_name;
    wxString m_imagePath;
    wxPoint m_position;
    wxSize m_size;

    GenericNode* m_prev;
    //GraphicalNode* m_graphics;
};

class Connection : public GenericNode
{
public:
    Connection(GenericNode* startBlock, GenericNode* endBlock)
        : GenericNode("Connection") {};

   // void Draw(wxGraphicsContext* gc) override {
   ///*     wxPoint startPoint = _startNode->GetPosition() + wxSize(_startNode->GetSize().x, _startNode->GetSize().y / 2);
   //     wxPoint endPoint = _endNode->GetPosition() + wxSize(0, _endNode->GetSize().y / 2);

   //     gc->SetPen(wxPen(*wxBLACK, 2));
   //     gc->StrokeLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
   //*/ }

private:
};

