#include "GenericNode.h"

GenericNode::GenericNode(const wxString& name, const wxString& imagePath) {
    _name = name;
    _imagePath = imagePath;
}

// Copy constructor
GenericNode::GenericNode(const GenericNode& other) {
    _name = other._name;
    _imagePath = other._imagePath;
}

GenericNode::~GenericNode() {
    delete _prev;
    delete _next;
}

wxString GenericNode::GetName() {
    return _name;
}

wxString GenericNode::GetImagePath() {
    return _imagePath;
}

void GenericNode::SetNext(GenericNode* next) {
    _next = next;
}

void GenericNode::SetPrev(GenericNode* prev) {
    _prev = prev;
}

void GenericNode::Draw(wxGraphicsContext* gc) {
    // Load the image
    wxImage image(_imagePath);
    if (!image.IsOk()) {
        return;
    }

    // Convert the wxImage to wxBitmap
    wxBitmap bitmap(image);

    // Draw the image on the graphics context
    gc->DrawBitmap(bitmap, _position.x, _position.y, _size.GetWidth(), _size.GetHeight());
}

void GenericNode::InstantiateNode(int x, int y, wxSize size) {
    _position.x = x;
    _position.y = y;
    _size = size;
}

Connection::~Connection(){
    delete _startNode;
    delete _endNode;
}
