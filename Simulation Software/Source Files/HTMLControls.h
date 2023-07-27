#pragma once
#include <wx/wx.h>
#include <wx/webview.h>

namespace GraphTypes
{
	const wxString BAR_GRAPH = "bar";
	const wxString LINE_GRAPH = "line";
	const wxString SCATTER_PLOT = "scatter";
	const wxString DONUT_GRAPH = "doughnut";
	const wxString BUBBLE_CHART = "bubble";
	const wxString POLAR_AREA_CHART = "polarArea";
};

class HTMLDocumentBuilder
{
public:
	HTMLDocumentBuilder();

	void SetChartType(wxString& type);
	void SetCanvasSize(wxSize& size);
	void SetLineWidth(int pixels);
	void SetLineColor(wxColour& color);
	void SetBorderColor(wxColour& color);
	void SetBackgroundColor(wxColour& color);
	void SetChartLabel(wxString& label);

	// linear interpolation methods
	void SetInterpolationMode(bool defaultMode);
	void SetInterpolationTension(double tension);

	// bring all strings together in the correct order to build  
	// a script that will chart the data based on the values saved in the document
	wxString BuildGraphingScript();

	// bring all strings together in the correct order to build the 
	// current HTML document from the non-null data
	// this allows for building of different types of documents.
	wxString BuildDocument();

private:
	const wxString m_chart_js_path = "D:\\wxChart.js-master";
	const wxString m_output_path = ".\\Output Files\\Charts\\";

	// canvas
	wxSize m_size;

	// chart building
	wxString m_chartTitle;
	wxString m_chartType;
	wxString m_lineWidth;
	wxColour m_lineColor;
	wxColour m_borderColor;
	wxColour m_backgroundColor;

	// charting values
	wxString m_data;
	wxString m_labels;

	// Line Graph Interpolation 
	wxString m_interpolationMode;
	double m_interpolationTension;
};

class HTMLViewer : public wxPanel
{
public:
	HTMLViewer(wxWindow* parent);
	void SetSize(wxSize size);

private:
	wxWebView* m_webViewer;
	//HtmlDocument* m_pages;

	void OnResize(wxSizeEvent& event);

};