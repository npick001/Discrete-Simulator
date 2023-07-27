#include "HTMLControls.h"

HTMLPageViewer::HTMLPageViewer(wxWindow* parent)
{
	m_webViewer = wxWebView::New(this, wxID_ANY,
		"http://www.wxwidgets.org/",
		wxDefaultPosition, wxDefaultSize,
		wxWebViewBackendDefault);

	this->Bind(wxEVT_SIZE, &HTMLPageViewer::OnResize, this);
}

void HTMLPageViewer::SetSize(wxSize size)
{
	this->SetSize(size);
	m_webViewer->SetSize(GetSize());
	Refresh();
}

void HTMLPageViewer::OnResize(wxSizeEvent& event)
{
	m_webViewer->SetSize(GetSize());
	event.Skip();
}

HTMLDocumentBuilder::HTMLDocumentBuilder()
{
	// canvas
	m_size = wxSize(640, 480);

	// chart building
	m_chartTitle = "";
	m_chartType = "";
	m_lineWidth = "";
	m_lineColor = *wxBLACK;
	m_borderColor = *wxBLACK;
	m_backgroundColor = *wxBLACK;

	// charting values
	m_data = "";
	m_labels = "";

	// interpolation
	m_interpolationMode = "default";
	m_interpolationTension = 0.0;
}

void HTMLDocumentBuilder::SetChartType(wxString& type)
{
	m_chartType = type;
}

void HTMLDocumentBuilder::SetCanvasSize(wxSize& size)
{
	m_size = size;
}

void HTMLDocumentBuilder::SetLineWidth(int pixels)
{
	m_lineWidth = std::to_string(pixels);
}

void HTMLDocumentBuilder::SetLineColor(wxColour& color)
{
	m_lineColor = color;
}

void HTMLDocumentBuilder::SetBorderColor(wxColour& color)
{
	m_borderColor = color;
}

void HTMLDocumentBuilder::SetBackgroundColor(wxColour& color)
{
	m_backgroundColor = color;
}

void HTMLDocumentBuilder::SetChartLabel(wxString& label)
{
	m_chartTitle = label;
}

void HTMLDocumentBuilder::SetInterpolationMode(bool defaultMode)
{
	if (defaultMode) {
		m_interpolationMode = "default";
	}
	else {
		m_interpolationMode = "monotone";
	}
}

void HTMLDocumentBuilder::SetInterpolationTension(double tension)
{
	m_interpolationTension = tension;
}

wxString HTMLDocumentBuilder::BuildGraphingScript()
{
	// CHECK THAT ALL VALUES ARE POPULATED
	// FOR NOW WE WILL ASSUME THAT ARE

	wxString chartHTML = wxString::Format(
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
			"<title>" + m_chartTitle + "</title>\n"
			"<script src=" + m_chart_js_path + ">< / script>\n"
		"</head>\n"

		// create chart
		"<body>\n"
			"<canvas id='myChart' width='%d' height='%d'></canvas>\n"
			"<script>\n"
				"var ctx = document.getElementById('myChart').getContext('2d');\n"
				"var myChart = new Chart(ctx, {\n"
					"type: " + m_chartType +",\n"
					"data: {\n"
						"labels: [%s],\n"
						"datasets: [{\n"
							"label: 'whatever man',\n"
							"data: [%s],\n"
							"backgroundColor:" + m_backgroundColor.GetAsString() + ",\n"
							"borderColor: " + m_borderColor.GetAsString() + ",\n"
							"borderWidth: " + m_lineWidth + "\n"
						"}]\n"
					"},\n"

					// configure options
					"options: {\n"
						"responsive: false,\n"
						"scales: {\n"
							"y: {\n"
								"beginAtZero: true\n"
							"}\n"
						"}\n"
					"}\n"
				"})\n"
			"</script>\n"
		"</body>\n"
		"</html>\n",
		m_size.GetWidth(), m_size.GetHeight(), m_labels, m_data);

	return chartHTML;
}

wxString HTMLDocumentBuilder::BuildDocument()
{
	wxString suffix = ".html";


	return wxString();
}
