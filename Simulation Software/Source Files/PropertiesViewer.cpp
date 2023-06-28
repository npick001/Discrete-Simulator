#include "PropertiesViewer.h"

PropertiesViewer::PropertiesViewer(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	auto width = GetParent()->GetSize().x * 0.2;
	auto height = GetParent()->GetSize().y;
	wxSize panelSize(width, height);

	m_propGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, panelSize,
		wxPG_SPLITTER_AUTO_CENTER);

	// Event bindings
	this->Bind(wxEVT_SIZE, &PropertiesViewer::OnResize, this);
	this->Bind(wxEVT_PG_CHANGED, &PropertiesViewer::OnPropertyGridChange, this);
}

void PropertiesViewer::Reset()
{
	while (m_props.GetSize() > 0) {
		m_propGrid->RemoveProperty(m_props.GetFirst());
	}
}

void PropertiesViewer::Refresh()
{
	m_propGrid->Refresh();
	m_propGrid->Update();
}

void PropertiesViewer::SetSize(wxSize newSize)
{
	auto width = newSize.x;
	auto height = newSize.y;
	wxSize panelSize(width, height);

	m_propGrid->SetSize(panelSize);
	m_propGrid->Refresh();
}

void PropertiesViewer::AddProperty(wxPGProperty* propToAdd)
{
	// add property to the PropGrid
	m_propGrid->Append(propToAdd);

	// save property in Set for easy access
	m_props.Add(propToAdd);
}

void PropertiesViewer::EditProperty(wxPGProperty* propToEdit, wxVariant* newValue)
{
	// Throw error and exit if property to edit is not there
	// should always be in the Set for this operation
	if (!m_props.Has(propToEdit)) {
		wxLogError("Selected property to edit does not exist.");
	}
	else {
		// get property
		auto prop = m_props.Get(propToEdit);

		// change property values
		prop->SetValue(newValue);
		m_propGrid->RefreshProperty(prop);

		// add property back to Set
		m_props.Add(prop);
	}
}

void PropertiesViewer::RemoveProperty(wxPGProperty* propToRemove)
{
	// Throw error and exit if property to edit is not there
	// should always be in the Set for this operation
	if (!m_props.Has(propToRemove)) {
		wxLogError("Selected property to remove does not exist.");
	}
	else {
		// get prop to remove from Set
		auto prop = m_props.Get(propToRemove);
		m_propGrid->DeleteProperty(prop);
	}
}

void PropertiesViewer::ShowProperties()
{
	m_propGrid->Freeze();

	//auto* propArray[m_props.GetSize()];
	Set<wxPGProperty> unshownProps;

	// copy all data over first
	// showing them will delete the data from the Set
	unshownProps = m_props;

	// Go through and enable all properties
	for (int i = 0; i < unshownProps.GetSize(); i++) {
		m_propGrid->EnableProperty(unshownProps.GetFirst());
	}

	m_propGrid->Thaw();
}

void PropertiesViewer::HideProperties()
{
	m_propGrid->Freeze();

	//auto* propArray[m_props.GetSize()];
	Set<wxPGProperty> shownProps;

	// copy all data over first
	// disabling them will delete the data from the Set
	shownProps = m_props;

	// Go through and disable all properties
	for (int i = 0; i < shownProps.GetSize(); i++) {

		auto thisOne = shownProps.GetFirst();

		// make sure its actually enabled
		if (m_propGrid->IsPropertyEnabled(thisOne)) {
			
			m_propGrid->DisableProperty(thisOne);
		}
	}

	m_propGrid->Thaw();
}

void PropertiesViewer::OnResize(wxSizeEvent& event)
{
	auto width = GetSize().x;
	auto height = GetSize().y;
	wxSize* panelSize = new wxSize(width, height);

	m_propGrid->SetSize(*panelSize);
	m_propGrid->Refresh();

	event.Skip();
}

void PropertiesViewer::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	auto changedProp = event.GetProperty();
	if (!changedProp) return;

	auto value = event.GetValue();

	changedProp->SetValueInEvent(value);
}
