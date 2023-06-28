#include <iostream>
#include "GenericNode.h"

int GenericNode::m_nextID = 0;

void GenericNode::SetNext(GenericNode* next) {
    m_next = next;
}

void GenericNode::SetPrevious(GenericNode* prev) {
    m_prev = prev;
}


std::string GenericNode::GetName() {
    return m_name;
}

void GenericNode::Arrive(Entity* entity) {
#if SIM_OUTPUT
    std::cout << "time = " << GetSimulationTime() << ", " << " " << m_name << ", arrive, entity: " << entity->GetID() << std::endl << std::flush;
#endif

    NodeProcess(entity);
}

void GenericNode::Depart(Entity* entity) {
#if SIM_OUTPUT
    std::cout << "time = " << GetSimulationTime() << ", " << " " << m_name << ", depart, entity: " << entity->GetID() << std::endl << std::flush;
#endif
    
    entity->SetSource(m_id);
    m_next->Arrive(entity);
}

GenericNode::GenericNode(const std::string& name) {
    m_name = name;
    m_id = m_nextID++;

    m_next = 0;
    m_prev = 0;

    //m_size = wxSize(100, 50);
}

// Copy constructor
GenericNode::GenericNode(const GenericNode& other) {
    m_name = other.m_name;
}

GenericNode::~GenericNode() {
    //delete m_prev;
    //delete m_next;
}

void GenericNode::StatisticsWrapper::DeleteStats()
{
}
