#include "DraggableText.h"

DraggableText::DraggableText(wxPanel* parent, wxColor& color, wxSize size) : wxPanel(parent)
{
	m_textcolor = &color;
	m_text->SetValue("Generic\nNode");
}

void DraggableText::SetText(const wxString& value)
{
	m_text->SetValue(value);
}

void DraggableText::SetColor(wxColor& color)
{
}
