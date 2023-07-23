#pragma once
#include <vector>
#include <wx/wx.h>

class ChartControl : public wxWindow
{
public:
	ChartControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

	std::string m_title;
	std::vector<double> m_values;
	std::vector<wxString> m_x_axis_labels;
	
private:
	void OnPaint(wxPaintEvent& event);
	std::tuple<int, double, double>  CalculateChartSegmentCountAndRange(double origLow, double origHigh);
};

//class StepCtrl : public ChartControl
//{
//
//};
//
//class HistogramCtrl : public ChartControl
//{
//
//};