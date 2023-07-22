#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/log.h>

#include "Statistics.h"

class InputAnalyzerPanel : public wxPanel
{
public:
	InputAnalyzerPanel(wxWindow* parent, StatisticsObject* statObj);


private:

	enum {
		ID_OPEN_FILE = 6999,
		ID_FILE_OPEN_BUTTON,
	};

	StatisticsObject* m_statObj;

	wxButton* m_fileOpenButton;

	void OnClickFileOpenButton(wxCommandEvent& event);
};

