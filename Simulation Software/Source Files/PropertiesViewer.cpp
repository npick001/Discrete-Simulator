#include "PropertiesViewer.h"

#include "GraphicalNode.h"
#include "CustomPGProperties.h"

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
	while (!m_props.IsEmpty()) {

		auto prop = m_props.GetFirst();
		prop->DeleteChildren();
		m_propGrid->RemoveProperty(prop);
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

void PropertiesViewer::SetSelectedObject(GraphicalNode* selectedNode)
{
	m_selectedNode = selectedNode;
}

void PropertiesViewer::AddProperty(wxPGProperty* propToAdd)
{
	// add property to the PropGrid
	m_propGrid->Append(propToAdd);

	// save property in Set for easy access
	m_props.Add(propToAdd);

	Refresh();
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

void PropertiesViewer::PopulateCorrectChildren(int choice)
{
	if (choice == 0) {
		// Exponential

		
	}
	else if (choice == 1) {
		// Uniform


	}
	else if (choice == 2) {
		// Triangular


	}
	else if (choice == 3) {
		// Normal


	}
	else if (choice == 4) {
		// Poisson


	}
	else if (choice == 5) {
		// Constant


	}
	else if (choice == 6) {
		// Weibull


	}
	else if (choice == 7) {
		// Erlang


	}
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
	Distribution* dist;

	if (changedProp->GetName() == "Interarrival Distribution" ||
		changedProp->GetName() == "Service Time Distribution") {

		// Get the value of the choice property
		int value = changedProp->GetValue().GetLong();

		changedProp->DeleteChildren();

		if (value == 1) {

			// Exponential
			double mean = 0.25;
			dist = new Exponential(mean);
			changedProp->AppendChild(new wxFloatProperty("Mean", wxPG_LABEL, mean));
		}
		else if (value == 2) {

			// Uniform
			double min = 0.0;
			double max = 1.0;
			dist = new Uniform(min, max);
			changedProp->AppendChild(new wxFloatProperty("Min", wxPG_LABEL, min));
			changedProp->AppendChild(new wxFloatProperty("Max", wxPG_LABEL, max));
		}
		else if (value == 3) {

			// Triangular
			double min = 1.0;
			double mean = 2.0;
			double max = 3.0;
			dist = new Triangular(min, mean, max);
			changedProp->AppendChild(new wxFloatProperty("Min", wxPG_LABEL, min));
			changedProp->AppendChild(new wxFloatProperty("Mean", wxPG_LABEL, mean));
			changedProp->AppendChild(new wxFloatProperty("Max", wxPG_LABEL, max));
		}
		else if (value == 4) {

			// Normal
			double mean = 1.0;
			double stdev = 0.25;
			dist = new Normal(mean, stdev);
			changedProp->AppendChild(new wxFloatProperty("Mean", wxPG_LABEL, mean));
			changedProp->AppendChild(new wxFloatProperty("Standard Deviation", wxPG_LABEL, stdev));
		}
		else if (value == 5) {

			// Poisson
			double mean = 0.25;
			dist = new Poisson(mean);
			changedProp->AppendChild(new wxFloatProperty("Mean", wxPG_LABEL, mean));
		}
		else if (value == 6) {

			// Constant
			double value = 1.0;
			dist = new Constant(value);
			changedProp->AppendChild(new wxFloatProperty("Value", wxPG_LABEL, value));
		}
		else if (value == 7) {

			// Weibull
			double shape = 1.0;
			double scale = 1.0;
			dist = new Weibull(shape, scale);
			changedProp->AppendChild(new wxFloatProperty("Shape", wxPG_LABEL, shape));
			changedProp->AppendChild(new wxFloatProperty("Scale", wxPG_LABEL, scale));
		}
		else if (value == 8) {

			// Erlang
			double shape = 1.0;
			double scale = 1.0;
			dist = new Erlang(shape, scale);
			changedProp->AppendChild(new wxFloatProperty("Shape", wxPG_LABEL, shape));
			changedProp->AppendChild(new wxFloatProperty("Scale", wxPG_LABEL, scale));
		}

		switch (m_selectedNode->GetNodeType()) {
		case GenericNode::SOURCE:



			break;
		case GenericNode::SERVER:



			break;
		case GenericNode::SINK:



			break;
		}

		wxArrayString timeUnits;
		timeUnits.push_back(wxString(TimeToString[TimeUnit::SECONDS]));
		timeUnits.push_back(wxString(TimeToString[TimeUnit::MINUTES]));
		timeUnits.push_back(wxString(TimeToString[TimeUnit::HOURS]));
		timeUnits.push_back(wxString(TimeToString[TimeUnit::YEARS]));

		wxEnumProperty* unitChoices = new wxEnumProperty("Time Unit", wxPG_LABEL, timeUnits);
		changedProp->AppendChild(unitChoices);
	}

	changedProp->SetValueInEvent(value);
}
