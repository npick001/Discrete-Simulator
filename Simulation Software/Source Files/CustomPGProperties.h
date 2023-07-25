#pragma once
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/property.h>

#include "Distribution.h"

class ExponentialProperty : public wxPGProperty
{
public:

	// Constructor: This is where you initialize your property. You will need to call the base class constructor with the appropriate parameters.
	ExponentialProperty(const wxString& label, const wxString& name, Exponential& value);

	// Value Type: This method should return the type of your property. This is a string that should be unique to your property.
	const wxChar* GetValueTypeName() const;

	// Value to String: This method should convert your property value to a string. This is used for displaying the property.
	wxString ValueToString(wxVariant& value, int argFlags) const;

	// String to Value: This method should convert a string to your property value. This is used when the user edits the property.
	bool StringToValue(wxVariant& variant, const wxString& text, int argFlags) const;

	// Value to Int: This method should convert your property value to an integer. This is used for sorting properties.
	int ValueToInt(wxVariant& value, int argFlags) const;

	// Clone: This method should create a new instance of your property.
	wxPGProperty* Clone() const;

	// OnSetValue: This method is called after m_value has been set.
	void OnSetValue();

	// ValidateValue: This method is called from SetValue() and from property grid's event handling system (when Enter is pressed or editor closed).
	bool ValidateValue(wxVariant& value, wxPGValidationInfo& validationInfo) const;

private:
	Exponential m_myDist;
};