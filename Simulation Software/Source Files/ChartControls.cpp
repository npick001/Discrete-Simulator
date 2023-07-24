#include "ChartControls.h"

#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

ChartControl::ChartControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	m_title = "Basic Chart";
	m_x_axis_textRotationAngle = 90;

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &ChartControl::OnPaint, this);
}

void ChartControl::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc) {

		wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2.0,
			wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

		// Set the font for the wxGraphicsContext
		gc->SetFont(titleFont, *wxBLACK);

		double tw, th;
		gc->GetTextExtent(m_title, &tw, &th);

		const double topBottomMinimumMargin = this->FromDIP(10);

		wxRect2DDouble fullArea(0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()));

		const double marginX = fullArea.GetSize().GetWidth() / 8.0;
		const double marginTop = std::max(fullArea.GetSize().GetHeight() / 8.0, topBottomMinimumMargin * 2.0 + th);
		const double marginBottom = fullArea.GetSize().GetHeight() / 8.0;
		double labelsToChartAreaMargin = FromDIP(10);

		wxRect2DDouble chartArea = fullArea;
		chartArea.Inset(marginX, marginTop, marginX, marginBottom);

		gc->DrawText(m_title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

		wxAffineMatrix2D normalizedToChartArea{};
		normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
		normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

		std::vector<wxPoint2DDouble> points;
		if (m_xvalues.size() != m_yvalues.size()) {
			throw std::runtime_error("X and Y vectors must be the same size for plotting");
		}
		else {
			for (int i = 0; i < m_xvalues.size(); i++) {
				points.push_back(wxPoint2DDouble(m_xvalues[i], m_yvalues[i]));
			}
		}

		double ylowValue = *std::min_element(m_yvalues.begin(), m_yvalues.end());
		double yhighValue = *std::max_element(m_yvalues.begin(), m_yvalues.end());
		double xlowValue = *std::min_element(m_xvalues.begin(), m_xvalues.end());
		double xhighValue = *std::max_element(m_xvalues.begin(), m_xvalues.end());
		double xValueSpan = xhighValue - xlowValue;
		double yValueSpan = yhighValue - ylowValue;

		wxAffineMatrix2D normalizedToValue{};
		normalizedToValue.Translate(0, yhighValue);
		normalizedToValue.Scale(1, -1);
		normalizedToValue.Scale(static_cast<double>(m_yvalues.size() - 1), yValueSpan);

		const std::tuple<int, double, double> ylines = CalculateChartSegmentCountAndRange(ylowValue, yhighValue);

		int segmentCount = std::get<0>(ylines);
		double rangeLow = std::get<1>(ylines);
		double rangeHigh = std::get<2>(ylines);

		ylowValue = rangeLow;
		yhighValue = rangeHigh;
		yValueSpan = yhighValue - ylowValue;

		double xLinesCount = m_x_axis_labels.size();
		double yLinesCount = segmentCount + 1;

		wxColour fadedBlack(0, 0, 0, 16);
		gc->SetPen(wxPen(fadedBlack, 3));
		gc->SetFont(*wxNORMAL_FONT, *wxBLACK);

		if (m_x_axis_labels.size() != xLinesCount) {
			wxLogError("Chart does not have adequate x-axis labels");
		}
		for (int i = 0; i < xLinesCount; i++) {

			double xvalue;
			m_x_axis_labels[i].ToDouble(&xvalue);

			double normalizedLineX = xvalue / xhighValue;

			auto lineStartPoint = normalizedToChartArea.TransformPoint({ normalizedLineX, 0 });
			auto lineEndPoint = normalizedToChartArea.TransformPoint({ normalizedLineX, 1 });

			wxPoint2DDouble linePoints[] = { lineStartPoint, lineEndPoint };

			gc->StrokeLines(2, linePoints);

			double valueAtLineX = normalizedToValue.TransformPoint({ normalizedLineX, 0 }).m_x;

			auto text = wxString::Format("%.2f", valueAtLineX);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetBottom() - labelsToChartAreaMargin);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);

			double x = lineEndPoint.m_x;
			double y = lineEndPoint.m_y;
			double textX = x + (tw / 2.0);
			double textY = y - (tw / 2.0);
			double rotationAngle = wxDegToRad(m_x_axis_textRotationAngle);

			gc->Translate(x, y);
			gc->Rotate(rotationAngle);
			gc->Translate(-x, -y);

			gc->DrawText(m_x_axis_labels[i], textX, textY);

			gc->Translate(x, y);
			gc->Rotate(-rotationAngle);
			gc->Translate(-x, -y);
		}

		for (int i = 0; i < yLinesCount; i++) {
			double normalizedLineY = static_cast<double>(i) / (yLinesCount - 1);

			auto lineStartPoint = normalizedToChartArea.TransformPoint({ 0, normalizedLineY });
			auto lineEndPoint = normalizedToChartArea.TransformPoint({ 1, normalizedLineY });

			wxPoint2DDouble linePoints[] = { lineStartPoint, lineEndPoint };

			gc->StrokeLines(2, linePoints);

			double valueAtLineY = normalizedToValue.TransformPoint({ 0,normalizedLineY }).m_y;

			auto text = wxString::Format("%.2f", valueAtLineY);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() - labelsToChartAreaMargin);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, chartArea.GetLeft() - labelsToChartAreaMargin - tw, lineStartPoint.m_y - th / 2.0);
		}

		wxPoint2DDouble leftHLinePoints[] = {
			normalizedToChartArea.TransformPoint({ 0, 0 }),
			normalizedToChartArea.TransformPoint({ 0, 1 }) 
		};

		wxPoint2DDouble rightHLinePoints[] = {
			normalizedToChartArea.TransformPoint({ 1, 0 }),
			normalizedToChartArea.TransformPoint({ 1, 1 })
		};

		gc->StrokeLines(2, leftHLinePoints);
		gc->StrokeLines(2, rightHLinePoints);

		wxPoint2DDouble* pointArray = new wxPoint2DDouble[points.size()];

		wxAffineMatrix2D valueToNormalized = normalizedToValue;
		valueToNormalized.Invert();
		wxAffineMatrix2D valueToChartArea = normalizedToChartArea;
		//valueToChartArea.Concat(valueToNormalized);

		for (int i = 0; i < points.size(); i++) {

			// transform [0, last state change time] to time weighted [0, 1] percent of axis
			/*int xValueLocation = ((i % 2) == 0) ? (2 * i) : ();*/

			double x_axis_percent = points[i].m_x / xhighValue;
			double x_location = xhighValue * x_axis_percent;

			pointArray[i] = valueToChartArea.TransformPoint(wxPoint2DDouble(x_axis_percent, m_yvalues[i]));
		}

		gc->SetPen(*wxRED_PEN);
		gc->StrokeLines(m_yvalues.size(), pointArray);

		delete[] pointArray;
		delete gc;
	}
}


std::tuple<int, double, double> ChartControl::CalculateChartSegmentCountAndRange(double origLow, double origHigh)
{
	constexpr double rangeMults[] = { 0.2, 0.25, 0.5, 1.0, 2.0, 2.5, 5.0 };
	constexpr int maxSegments = 6;

	double magnitude = std::floor(std::log10(origHigh - origLow));

	for (auto r : rangeMults)
	{
		double stepSize = r * std::pow(10.0, magnitude);
		double low = std::floor(origLow / stepSize) * stepSize;
		double high = std::ceil(origHigh / stepSize) * stepSize;

		int segments = round((high - low) / stepSize);

		if (segments <= maxSegments)
		{
			return std::make_tuple(segments, low, high);
		}
	}

	// return some defaults in case rangeMults and maxSegments are mismatched
	return std::make_tuple(10, origLow, origHigh);
}