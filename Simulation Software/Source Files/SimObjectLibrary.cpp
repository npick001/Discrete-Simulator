#include "SimObjectLibrary.h"

SimObjectLibrary::SimObjectLibrary(wxWindow * parent) : wxPanel(parent, wxID_ANY) {
    m_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(m_sizer);

    this->Bind(wxEVT_LEFT_DOWN, &SimObjectLibrary::OnNodeBeginDrag, this, ID_NodeBeginDrag);
    
    m_size.y = this->GetClientSize().y;
    m_size.x = this->GetMinWidth();
}

SimObjectLibrary::~SimObjectLibrary() {
}

void SimObjectLibrary::AddNode(GraphicalNode* node) {

    _nodes.push_back(node);

    //wxImage _nodePrefabImage(node->GetImagePath(), wxBITMAP_TYPE_ANY);
    //int width = 100;
    //int height = 50;
    //wxImage _scaledImage = _nodePrefabImage.Scale(width, height, wxIMAGE_QUALITY_HIGH);

    //wxStaticBitmap* bitmapControl = new wxStaticBitmap(this, wxID_ANY, _scaledImage);
    //GetSizer()->Add(bitmapControl, 0, wxALL, 5);
    //Layout();

    //// mem leak not here
    //bitmapControl->Bind(wxEVT_LEFT_DOWN, [this, node](wxMouseEvent& event) {
    //    NodeDataObject dataObject(node);
    //    wxDropSource dragSource(this);
    //    dragSource.SetData(dataObject);
    //    dragSource.DoDragDrop(wxDrag_DefaultMove);
    //    });
}

void SimObjectLibrary::OnNodeBeginDrag(wxMouseEvent& event) {

    // mem leak not here
    int index = event.GetId();
    if (index >= 0 && index < _nodes.size()) {
        GraphicalNode* node = _nodes[index];

        wxCustomDataObject* dataObject = new wxCustomDataObject(wxDataFormat(wxT("GenericNode*")));
        dataObject->SetData(sizeof(GenericNode*), &node);

        wxDropSource dragSource(this);
        dragSource.SetData(*dataObject);
        dragSource.DoDragDrop(wxDrag_DefaultMove);
    }
}