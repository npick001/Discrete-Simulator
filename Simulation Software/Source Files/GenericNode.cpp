#include "GenericNode.h"

int GenericNode::m_nextID = 0;

void GenericNode::SetNext(GenericNode* next) {
    m_next = next;
}

void GenericNode::SetPrevious(GenericNode* prev) {
    m_prev = prev;
}

void GenericNode::SetImagePath(const wxString& imagePath) {
    m_imagePath = imagePath;
}

wxString GenericNode::GetName() {
    return m_name;
}

wxString GenericNode::GetImagePath() {
    return m_imagePath;
}

//void GenericNode::InstantiateNode(int x, int y, wxSize size) {
//    m_position.x = x;
//    m_position.y = y;
//    m_size = size;
//}

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

GenericNode::GenericNode(const wxString& name) {
    m_name = name;
    m_imagePath = "";
    m_id = m_nextID++;

    m_next = 0;
    m_prev = 0;

    m_size = wxSize(100, 50);
}

// Copy constructor
GenericNode::GenericNode(const GenericNode& other) {
    m_name = other.m_name;
    m_imagePath = other.m_imagePath;
}

GenericNode::~GenericNode() {
    //delete m_prev;
    //delete m_next;
}

void GenericNode::StatisticsWrapper::DeleteStats()
{
}
