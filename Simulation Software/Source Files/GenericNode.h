#pragma once
#include <wx/wx.h>
#include <wx/log.h>

#include "Directives.h"
#include "Entity.h"

class GenericNode
{
public:
    enum Type {
        SOURCE,
        SERVER,
        SINK
    };

    void SetNext(GenericNode* next);
    void SetPrevious(GenericNode* prev);
    GenericNode* GetNext();
    GenericNode* GetPrevious();
    inline void SetNodeType(Type nodetype) { m_nodeType = nodetype; }
    std::string GetName();
    inline int GetID() { return m_id; }
    inline Type GetType() { return m_nodeType; }

    void Arrive(Entity* entity);

    class StatisticsWrapper {
    public:
        StatisticsWrapper(int id) : m_id(id) {}
        virtual void ReportStats() = 0;
        virtual void DeleteStats() = 0;

    protected:
        int m_id;
    };

    virtual std::unique_ptr<StatisticsWrapper> GetStatistics() = 0;

protected:
    friend class SimProject;
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
    Type m_nodeType;
    static int m_nextID;
    std::string m_name;

    GenericNode* m_previous;
};