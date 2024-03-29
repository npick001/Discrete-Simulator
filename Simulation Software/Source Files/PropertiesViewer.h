#pragma once
#include "Utility.h"

#include "wx/log.h"
#include "wx/panel.h"
#include "wx/propgrid/propgrid.h"

#include "Set.h"

enum {

};

class PropertiesViewer : public wxPanel
{
public:
	PropertiesViewer(wxWindow* parent);

	void Reset();
	void Refresh();
	void SetSize(wxSize newSize);

	void AddProperty(wxPGProperty* propToAdd);
	void EditProperty(wxPGProperty* propToEdit, wxVariant* newValue);
	void RemoveProperty(wxPGProperty* propToRemove);

	void ShowProperties();
	void HideProperties();

private:
	wxPropertyGrid* m_propGrid;
	Set<wxPGProperty> m_props;

	void OnResize(wxSizeEvent& event);
	void OnPropertyGridChange(wxPropertyGridEvent& event);
};
