#pragma once
#include <vector>
#include <wx/wx.h>

#include "SimulationExecutive.h"

class ChartControl : public wxWindow
{
public:
	ChartControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

	std::string m_title;
	std::vector<double> m_xvalues;
	std::vector<double> m_yvalues;
	std::vector<wxString> m_x_axis_labels;
	int m_x_axis_textRotationAngle;
	
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