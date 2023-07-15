#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>

class DraggableText : public wxPanel
{
public:
	DraggableText(wxPanel* parent, wxColor& color, wxSize size);

	void SetText(const wxString& value);
	void SetColor(wxColor& color);

private:
	wxColor* m_textcolor;
	wxTextCtrl* m_text;
	wxTextCompleter* m_autocompleter;
};

