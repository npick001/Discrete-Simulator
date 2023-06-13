#pragma once
//#include <wx/wx.h>
//#include <wx/graphics.h>

#include "Directives.h"
#include "Entity.h"

class GenericNode
{
public:
    enum Type {
        SOURCE,
        SINK
    };

    void SetNext(GenericNode* next);
    void SetPrevious(GenericNode* prev);
    void SetImagePath(const std::string& imagePath);
    inline void SetNodeType(std::string nodetype) { m_nodeType = nodetype; }
    std::string GetName();
    std::string GetImagePath();
    inline int GetID() { return m_id; }
    inline std::string GetType() { return m_nodeType; }

    void Arrive(Entity* entity);

    class StatisticsWrapper {
    public:
        StatisticsWrapper(int id) : m_id(id) {}
        virtual void ReportStats() = 0;
        virtual void DeleteStats() = 0;

    protected:
        int m_id;
    };

    // Virtual function returning a wrapper
    virtual std::unique_ptr<StatisticsWrapper> GetStatistics() = 0;

protected:
    GenericNode(const std::string& name);
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

    class Statistics {
    public: 
        virtual void Report(std::string header) = 0;
    protected:
        inline Statistics() {};
    };

private:
    int m_id;
    std::string m_nodeType;
    static int m_nextID;
    std::string m_name;
    std::string m_imagePath;
    //wxPoint m_position;
    //wxSize m_size;

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

