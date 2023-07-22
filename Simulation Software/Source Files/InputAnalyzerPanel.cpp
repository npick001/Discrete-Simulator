#include "InputAnalyzerPanel.h"

InputAnalyzerPanel::InputAnalyzerPanel(wxWindow* parent, StatisticsObject* statObj)
	: wxPanel(parent)
{
	m_statObj = statObj;

	wxPoint buttonTopLeft(GetSize().GetWidth() / 2, GetSize().GetHeight() / 2);
	m_fileOpenButton = new wxButton(this, ID_FILE_OPEN_BUTTON, "Open File", buttonTopLeft, FromDIP(wxSize(75, 40)));

	this->Bind(wxEVT_BUTTON, &InputAnalyzerPanel::OnClickFileOpenButton, this, ID_OPEN_FILE);
}

void InputAnalyzerPanel::OnClickFileOpenButton(wxCommandEvent& event)
{
	// handle the file opening for .txt files
	wxLogError("Button Clicked");
}
