#include "HTMLControls.h"

HTMLViewer::HTMLViewer(wxWindow* parent)
{
	m_webViewer = wxWebView::New(this, wxID_ANY,
		"http://www.wxwidgets.org/",
		wxDefaultPosition, wxDefaultSize,
		wxWebViewBackendDefault);

	this->Bind(wxEVT_SIZE, &HTMLViewer::OnResize, this);
}

void HTMLViewer::SetSize(wxSize size)
{
	this->SetSize(size);
	m_webViewer->SetSize(GetSize());
	Refresh();
}

void HTMLViewer::OnResize(wxSizeEvent& event)
{
	m_webViewer->SetSize(GetSize());
	event.Skip();
}

HTMLDocumentBuilder::HTMLDocumentBuilder()
{
}

void HTMLDocumentBuilder::SetChartType(wxString& type)
{
}

void HTMLDocumentBuilder::SetCanvasSize(wxSize& size)
{
}

void HTMLDocumentBuilder::SetLineWidth(int pixels)
{
}

void HTMLDocumentBuilder::SetLineColor(wxColour& color)
{
}

void HTMLDocumentBuilder::SetBorderColor(wxColour& color)
{
}

void HTMLDocumentBuilder::SetBackgroundColor(wxColour& color)
{
}

void HTMLDocumentBuilder::SetChartLabel(wxString& label)
{
}

wxString HTMLDocumentBuilder::BuildGraphingScript()
{
	return wxString();
}

wxString HTMLDocumentBuilder::BuildDocument()
{
	wxString suffix = ".html";




	return wxString();
}
