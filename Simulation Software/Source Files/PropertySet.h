
#include "wx/propgrid/propgrid.h"
#include "Set.h"

/*
PropertySet:
This class handles properties for a FIFO LL scheme.
Used for additional handling functions specified to property management.
*/
class PropertySet
{
private:
	Set<wxPGProperty> m_propSet;
public:
	void Add(wxPGProperty* property);
	void Empty();
	bool IsEmpty();
	int GetSize();
	bool Has(wxPGProperty* toFind);
	
	wxPGProperty* View();
	wxPGProperty* GetFirst();
	wxPGProperty* Get(wxPGProperty* toFind);

	void CreateParentChildLink(wxPGProperty* parent, wxPGProperty* child);
	
	// Do a deep copy of the passed Set to this one
	PropertySet& operator=(const PropertySet& other);
};