#include <iostream>
#include "GenericNode.h"

int GenericNode::m_nextID = 0;

void GenericNode::AddNext(GenericNode* next) {
    m_next.Add(next);
}

void GenericNode::AddPrevious(GenericNode* prev) {
    m_previous.Add(prev);
}

Set<GenericNode> GenericNode::GetNext()
{
    return m_next;
}

Set<GenericNode> GenericNode::GetPrevious()
{
    return m_previous;
}


std::string GenericNode::GetName() {
    return m_name;
}

void GenericNode::Arrive(Entity* entity) {
#if SIM_OUTPUT
    std::cout << "time = " << GetSimulationTime() << ", " << " " << m_name << ", arrive, entity: " << entity->GetID() << std::endl << std::flush;
#endif
    std::string message = "time = " + std::to_string(GetSimulationTime()) + "\t" + m_name + "\tArrive\tEntity: " + std::to_string(entity->GetID());
    wxLogMessage("%s", message.c_str());

    NodeProcess(entity);
}

void GenericNode::Depart(Entity* entity) {
#if SIM_OUTPUT
    std::cout << "time = " << GetSimulationTime() << ", " << " " << m_name << ", depart, entity: " << entity->GetID() << std::endl << std::flush;
#endif
    std::string message = "time = " + std::to_string(GetSimulationTime()) + "\t" + m_name + "\tDepart\tEntity: " + std::to_string(entity->GetID());
    wxLogMessage("%s", message.c_str());
    
    entity->SetSource(m_id);
    m_next.GetRandom()->Arrive(entity);
}

GenericNode::GenericNode(const std::string& name) {
    m_name = name;
    m_id = m_nextID++;
}

// Copy constructor
GenericNode::GenericNode(const GenericNode& other) {
    m_name = other.m_name;
}

GenericNode::~GenericNode() {
}

void GenericNode::StatisticsWrapper::DeleteStats()
{
}
