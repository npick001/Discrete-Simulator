#include "ExponentialProperty.h"

ExponentialProperty::ExponentialProperty(const wxString& label, const wxString& name, Exponential& value)
    : wxPGProperty(label, name)
{
    m_myDist = Exponential(value.GetMean());
    m_value = wxVariant(m_myDist.GetMean());
}

const wxChar* ExponentialProperty::GetValueTypeName() const
{
    return wxT("Exponential");
}

wxString ExponentialProperty::ValueToString(wxVariant& value, int argFlags) const
{
    return wxString::Format(wxT("%f"), value.GetDouble());
}

bool ExponentialProperty::StringToValue(wxVariant& variant, const wxString& text, int argFlags) const
{
    double val;
    if (text.ToDouble(&val))
    {
        variant = val;
        return true;
    }
    return false;
}

int ExponentialProperty::ValueToInt(wxVariant& value, int argFlags) const
{
    return (int)value.GetDouble();
}

wxPGProperty* ExponentialProperty::Clone() const
{
    Exponential exp(m_value.GetDouble());
    return new ExponentialProperty(m_label, m_name, exp);
}

void ExponentialProperty::OnSetValue()
{
    m_myDist.SetMean(m_value);
}

bool ExponentialProperty::ValidateValue(wxVariant& value, wxPGValidationInfo& validationInfo) const
{
    // keep user values above 0, sim time is non decreasing
    if (value.GetDouble() < 0) {
        validationInfo.SetFailureMessage(wxT("The mean value must be greater than 0."));
        return false;
    }
    return true;
}
