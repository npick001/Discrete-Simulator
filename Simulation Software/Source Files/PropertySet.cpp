#include "PropertySet.h"

void PropertySet::CreateParentChildLink(wxPGProperty* parent, wxPGProperty* child)
{

}

void PropertySet::Add(wxPGProperty* property)
{
	m_propSet.Add(property);
}

wxPGProperty* PropertySet::GetFirst()
{
	return m_propSet.GetFirst();
}

wxPGProperty* PropertySet::Get(wxPGProperty* toFind)
{
	return m_propSet.Get(toFind);
}

bool PropertySet::Has(wxPGProperty* toFind)
{
	return m_propSet.Has(toFind);
}

void PropertySet::Clear()
{
	m_propSet.Clear();
}

wxPGProperty* PropertySet::View()
{
	return m_propSet.View();
}

bool PropertySet::IsEmpty()
{
	return  m_propSet.IsEmpty();
}

int PropertySet::GetSize()
{
	return  m_propSet.GetSize();
}

// Do a deep copy of the passed Set to this one
PropertySet& PropertySet::operator=(const PropertySet& other)
{
	if (this != &other) {
		m_propSet = other.m_propSet;
	}
	return *this;
}