
#include "gcem.hpp"
#include "stats.hpp"
#include "Statistics.h"
#include "Entity.h"
#include "FIFO_Queue.h"

StatisticsObject::StatisticsObject(int histogramIntervals)
{
	// Input data
	m_numDataPoints = 0;
	m_dataRange = 0;
	m_binSize = 0;
	m_currentFile = "";
	m_xMinimum = 0, m_xMaximum = 0;
	m_yMinimum = 0, m_yMaximum = 0;

	// Histogram
	m_numHistogramIntervals = histogramIntervals;

	// Theoretical distributions
	m_numCurvePoints = 100;

	// Newtons Method
	// default tolerance 10^-6
	m_iterationTolerance = pow(10, -6);
	m_iterations = 500;
}

void StatisticsObject::ReadStatFile(std::string filename)
{
	ResetStatistics();

	std::ifstream iFile;
	iFile.open(filename);
	std::string currentFile = filename;

	// make sure the file can open correctly
	// if opened correctly, then save the first value as number of points
	if (iFile.is_open()) {
		iFile >> m_numDataPoints;
	}

	// allocate memory for all the data points in file
#if DEBUG
	std::cout << "Data set size: " << m_numDataPoints << '\n';
#endif // DEBUG
	m_dataset.clear();
	while (m_dataset.size() < m_numDataPoints) {
		m_dataset.push_back(0.0);
	}
	m_xMinimum = FLT_MAX, m_xMaximum = FLT_MIN;
	m_yMinimum = FLT_MAX, m_yMaximum = FLT_MIN;

	int currentLine = 0;
	float dataSum = 0;

	// get all lines in file
	while (!iFile.eof() && currentLine < m_numDataPoints) {
		iFile >> m_dataset[currentLine];

		// compute data min and max
		if (m_dataset[currentLine] < m_xMinimum)
			m_xMinimum = m_dataset[currentLine];
		if (m_dataset[currentLine] > m_xMaximum)
			m_xMaximum = m_dataset[currentLine];

		dataSum += m_dataset[currentLine];
		currentLine++;
	}
	float dataAvg = dataSum / m_numDataPoints;

	// make sure to close file when done
	iFile.close();
}

std::vector<double> StatisticsObject::CreateDummyData(int dataSize, Distribution* distribution)
{
	Distribution* dist = distribution;
	std::vector<double> distData;

#if WRITETOFILE
	WriteToFile(".\\Output Files\\MLEDistributionData.txt", dataSize, 1);
#endif // WRITETOFILE

	for (int i = 0; i < dataSize; i++) {
		distData.push_back(dist->GetRV());

#if WRITETOFILE
		WriteToFile(".\\Output Files\\MLEDistributionData.txt", distData[i], 0);
#endif // WRITETOFILE
	}

	delete dist;
	return distData;
}

void StatisticsObject::ResetStatistics()
{
	m_dataset.clear();
	m_intervalDensity.clear();
	m_numDataPoints = 0;
	m_dataRange = 0;
	m_binSize = 0;
	m_currentFile = "";
	m_numCurvePoints = 100;
#if DEBUG
	std::cout << "Resetting Statistics.\n" << std::endl;
#endif // DEBUG
}

void StatisticsObject::ComputeFilePDF()
{
	// go through and check every value for if its in current bin
	// if so, increment count to current bin

	m_intervalDensity.clear();
	while (m_intervalDensity.size() < m_numHistogramIntervals) {
		m_intervalDensity.push_back(0.0);
	}

	m_dataRange = m_xMaximum - m_xMinimum;
	m_binSize = m_dataRange / (float)m_numHistogramIntervals;

	// define the intervals for the bings
	std::unique_ptr<float[]> ranges(new float[m_numHistogramIntervals + 1]);
	ranges[0] = m_xMinimum;
	for (int i = 1; i <= m_numHistogramIntervals; i++) {
		ranges[i] = ranges[i - 1] + m_binSize;
#if DEBUG
		std::cout << "[" << ranges[i - 1] << ", " << ranges[i] << "]" << '\n';
#endif // DEBUG
	}
#if DEBUG
	std::cout << "Data range: " << m_dataRange << ", Bin Size: " << m_binSize << '\n';
	std::cout << "X max: " << m_xMaximum << ", X min: " << m_xMinimum << '\n';
#endif // DEBUG

	// put number of values into respective bin
	for (int i = 0; i < m_numDataPoints; i++) {

		for (int j = 0; j < m_numHistogramIntervals; j++) {
			if (m_dataset[i] < ranges[j + 1]) {
				//std::cout << "Increasing bin " << j << '\n';
				m_intervalDensity[j]++;
				break;
			}
		}

		//std::cout << "Data Point: " << dataset[i] << ", Bin Index: " << binIndex << '\n';
	}

#if WRITETOFILE
	WriteToFile(".\\Output Files\\HistogramDensity.txt", "Resetting file.", 1);
#endif // WRITETOFILE
	double total = 0.0;
	for (int j = 0; j < m_numHistogramIntervals; j++) {

		// get the density of points in bin / total number of points
		m_intervalDensity[j] /= m_numDataPoints;
		//intervalDensity[j] /= binSize;
		//intervalDensity[j] /= numHistogramIntervals;

		// compute pdf min and max
		if (m_intervalDensity[j] < m_yMinimum)
			m_yMinimum = m_intervalDensity[j];
		if (m_intervalDensity[j] > m_yMaximum)
			m_yMaximum = m_intervalDensity[j];
#if DEBUG
		std::cout << "Bin: " << j << ", " << m_intervalDensity[j] << '\n';
#endif // DEBUG

#if WRITETOFILE
		WriteToFile(".\\Output Files\\HistogramDensity.txt", m_intervalDensity[j], 0);
#endif // WRITETOFILE
		total += m_intervalDensity[j];
	}
	std::cout << "\nTOTAL: " << total << '\n';

	m_dataRange = m_yMaximum - m_yMinimum;
}

double StatisticsObject::ChiSquareTest(std::vector<double> data, std::vector<double> probabilities, bool iWrote)
{
	// make sure they are the same size.
	if (!(data.size() == probabilities.size())) {
		std::cout << "Data and probability sizes are not the same." << std::endl;
		return -1.0;
	}

	int size = data.size();
	std::vector<double> observed = data;
	std::vector<double> expected = probabilities;
	double dataSum = 0.0;
	double testStat = 0.0;

	// calculate test statistic
	for (int i = 0; i < size; i++) {
		double numerator = pow((observed[i] - expected[i]), 2);
		double denominator = expected[i];

		double relativeError = (numerator / denominator);
		testStat += relativeError;
	}	

	double dof = (double)(size - 1);
	int numPointsToGen = 100000;
	double p = 1.0 - ChiSquareCDF(dof, testStat, numPointsToGen);

	//double testStatistic = stats::pchisq 

	std::cout << "Test statistic: " << testStat << std::endl;
	std::cout << "Degrees of Freedom: " << dof << std::endl;
	std::cout << "P value: " << p << std::endl;

	return p;
}

double StatisticsObject::ChiSquareTest(std::vector<double> observed, std::vector<double> expected) {

	for (int i = 0; i < observed.size(); i++) {
		
	}

	return -1;
}

double StatisticsObject::NewtonsMethod(double alpha, double initialGuess)
{
	/*
		Need to find the quantile of a chi-squared distribution given a given alpha level. 
		So, we are looking for a value x such that CDF(x) = alpha. 
		This can be framed as a root-finding problem: find the root of the function f(x) = CDF(x) - alpha.
	*/

	/*
	The Newton-Raphson method is a root-finding algorithm that requires a function and its derivative.
	In your case, you want to find the critical value x for which the CDF of the chi-square distribution equals 1 - alpha.
	This means that the function you're looking for the root of is F(x) = CDF(x) - (1 - alpha).
	*/

	/*
	
		x_(n+1) = x_n - f(x_n) / f'(x_n)

	*/
	return 0;
}

double StatisticsObject::ChiSquareCriticalValue(double alpha, double dof) {

	// Use newton iteration to approximate the critical value

	double x = 2.0 * dof;  // A reasonable initial guess
	int max_iters = 250000;

	for (int i = 0; i < max_iters; i++) {
		double cdf = ChiSquareCDF(dof, x, 1000);
		double pdf_value = PDF_Chi_Square(x, dof);
		double f = cdf - (1 - alpha);
		double new_x = x - f / pdf_value;

		// If the change is small enough, we've converged
		if (abs(new_x - x) < m_iterationTolerance) {
			return new_x;
		}

		x = new_x;
	}

	// If we reach here, we did not converge in the given number of iterations
	return x;
}

void StatisticsObject::KolmogorovSmirnovTest()
{
}

void StatisticsObject::LeastSquaresEstimate()
{
}

void StatisticsObject::StatTesting()
{
#if DEBUG
	Triangular* tri = new Triangular(1, 2, 3);
	Normal* norm = new Normal(5, 1);
	Exponential* exp = new Exponential(5);
	Uniform* uni = new Uniform(-1, 1);
	int dataPoints = 500;

	// UNIFORM
	std::cout << "\nUNIFORM DISTRIBUTION (-1, 1)\n";
	CreateDummyData(dataPoints, uni);
	ReadStatFile("test.txt");
	ComputeFilePDF();

	// NORMAL
	std::cout << "\nNORMAL DISTRIBUTION (5, 1)\n";
	CreateDummyData(dataPoints, norm);
	ReadStatFile("test.txt");
	ComputeFilePDF();

	std::vector<double> normParams = MLE_Normal(m_dataset);
	Normal* norm_MLE = new Normal(normParams[0], normParams[1]);

	std::cout << "\n MLE FROM NORMAL DISTRIBUTION (5, 1)\n";
	CreateDummyData(dataPoints, norm_MLE);
	ReadStatFile("test.txt");
	ComputeFilePDF();

	// EXPONENTIAL
	std::cout << "\nEXPONENTIAL DISTRIBUTION (5)\n";
	CreateDummyData(dataPoints, exp);
	ReadStatFile("test.txt");
	ComputeFilePDF();

	// TRIANGULAR
	std::cout << "\nTRIANGULAR DISTRIBUTION (1, 2, 3)\n";
	CreateDummyData(dataPoints, tri);
	ReadStatFile("test.txt");
	ComputeFilePDF();

	ResetStatistics();
#endif // DEBUG
}

void StatisticsObject::GenerateTestDistributions(std::vector<double> data)
{
	// Generate Exponential Test distribution
	std::vector<double> expParams = MLE_Exponential(data);
	m_testDistributions.push_back(GenerateExponentialFromParams(expParams));

	// Generate Normal Test distribution
	std::vector<double> normParams = MLE_Normal(data);
	m_testDistributions.push_back(GenerateNormalFromParams(normParams));
}

std::vector<double> StatisticsObject::ComputePDF(std::vector<double> data, int numBins)
{
	std::vector<double> intervalDensity;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double dataRange = 0.0;

	// determine data extrema
	for (int i = 0; i < data.size(); i++){
		if (data[i] < xMin) {
			xMin = data[i];
		}
		else if (data[i] > xMax) {
			xMax = data[i];
		}
	}

	// go through and check every value for if its in current bin
	// if so, increment count to current bin
	while (intervalDensity.size() < numBins) {
		intervalDensity.push_back(0.0);
	}

	dataRange = xMax - xMin;
	double binSize = dataRange / (float)numBins;

	// define the intervals for the bings
	std::unique_ptr<float[]> ranges(new float[numBins + 1]);
	ranges[0] = xMin;
	for (int i = 1; i <= numBins; i++) {
		ranges[i] = ranges[i - 1] + binSize;
#if DEBUG
		std::cout << "[" << ranges[i - 1] << ", " << ranges[i] << "]" << '\n';
#endif // DEBUG
	}
#if DEBUG
	std::cout << "Data range: " << dataRange << ", Bin Size: " << binSize << '\n';
	std::cout << "X max: " << xMax << ", X min: " << xMin << '\n';
#endif // DEBUG

	// put number of values into respective bin
	for (int i = 0; i < data.size(); i++) {

		for (int j = 0; j < numBins; j++) {
			if (data[i] < ranges[j + 1]) {
				//std::cout << "Increasing bin " << j << '\n';
				intervalDensity[j]++;
				break;
			}
		}

		//std::cout << "Data Point: " << dataset[i] << ", Bin Index: " << binIndex << '\n';
	}

#if WRITETOFILE
	WriteToFile(".\\Output Files\\ComputedDataPDF.txt", "Resetting file.", 1);
#endif // WRITETOFILE
	double total = 0.0;
	double yMin = DBL_MAX;
	double yMax = DBL_MIN;

	for (int j = 0; j < numBins; j++) {

		// get the density of points in bin / total number of points
		intervalDensity[j] /= data.size();
		//intervalDensity[j] /= binSize;
		//intervalDensity[j] /= numHistogramIntervals;

		// compute pdf min and max
		if (intervalDensity[j] < yMin)
			yMin = intervalDensity[j];
		if (intervalDensity[j] > yMax)
			yMax = intervalDensity[j];
#if DEBUG
		std::cout << "Bin: " << j << ", " << intervalDensity[j] << '\n';
#endif // DEBUG

#if WRITETOFILE
		WriteToFile(".\\Output Files\\ComputedDataPDF.txt", intervalDensity[j], 0);
#endif // WRITETOFILE
		total += intervalDensity[j];
	}
	std::cout << "\nTOTAL: " << total << '\n';

	dataRange = yMax - yMin;
	return intervalDensity;
}

Exponential* StatisticsObject::GenerateExponentialFromParams(std::vector<double> params)
{
	Exponential* exp = new Exponential(params[0]);
	return exp;
}

Normal* StatisticsObject::GenerateNormalFromParams(std::vector<double> params)
{
	Normal* norm = new Normal(params[0], params[1]);
	return norm;
}

Distribution* StatisticsObject::DetermineBestFit()
{
	Distribution* smallest = new Uniform(0, 1);
	double smallest_p_value = DBL_MAX;
	for (int i = 0; i < m_testDistributions.size(); i++) {
		std::vector<double> distData;
		std::vector<double> pdf;

		distData = CreateDummyData(2500, m_testDistributions[i]);
		pdf = ComputePDF(distData, m_numHistogramIntervals);

		double current_p_value = ChiSquareTest(pdf, m_intervalDensity);

		for (int i = 0; i < pdf.size(); i++) {
			std::cout << "Observed: " << pdf[i] << ", Expected: " << m_intervalDensity[i] << std::endl;
		}

		// determine the smallest p-value
		// and save distribution.
		if (smallest_p_value > current_p_value) {

			smallest_p_value = current_p_value;
			smallest = m_testDistributions[i];
		}
	}

	return smallest;
}

// NOT TESTED
Distribution* StatisticsObject::GenerateBestFitDistribution(std::string fileName)
{
	ResetStatistics();
	ReadStatFile(fileName);
	ComputeFilePDF();
	GenerateTestDistributions(m_dataset);
	Distribution* bestFit = DetermineBestFit();

	return bestFit;
}

std::vector<double> StatisticsObject::MLE_Exponential(std::vector<double>& data)
{
	std::vector<double> estimatedParams;
	double sum = 0.0;
	double size = data.size();

	for (int i = 0; i < size; i++) {
		sum += data[i];
	}

	double MLE_Estimate = sum / size;
	estimatedParams.push_back(MLE_Estimate);
	std::cout << "Exponential Mean Estimate: " << MLE_Estimate << '\n';

	return estimatedParams;
}

std::vector<double> StatisticsObject::MLE_Normal(std::vector<double>& data)
{
	std::vector<double> estimatedParams;
	double sum = 0.0;
	double size = data.size();
	double variance = 0.0;
	
	// Sum all the data
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}

	// Calculate mean
	double meanEstimate = size / sum;
	estimatedParams.push_back(meanEstimate);
	std::cout << "Normal Mean Estimate: " << meanEstimate << '\n';

	// Calculate variance
	double varEstimate = 0.0;
	for (int i = 0; i < size; i++) {
		variance += pow((data[i] - meanEstimate), 2);
	}
	varEstimate = variance / (double)(size - 1);
	estimatedParams.push_back(varEstimate);
	std::cout << "Normal Variance Estimate: " << varEstimate << '\n';

	return estimatedParams;
}

std::vector<double> StatisticsObject::MLE_Triangular(std::vector<double>& data)
{
	return std::vector<double>();
}

std::vector<double> StatisticsObject::MLE_Poisson(std::vector<double>& data)
{
	return std::vector<double>();
}

std::vector<double> StatisticsObject::MLE_Weibull(std::vector<double>& data)
{
	return std::vector<double>();
}

std::vector<double> StatisticsObject::MLE_Erlang(std::vector<double>& data)
{
	return std::vector<double>();
}

unsigned long long StatisticsObject::factorial(unsigned int n)
{
	unsigned long long sum = 1;
	for (int i = n; i > 0; i--) {
		sum *= i;
	}

#if DEBUG
	std::cout << "Factorial for " << n << ": " << sum << std::endl;
#endif // DEBUG


	return sum;
}

double StatisticsObject::GammaFunction(unsigned int n)
{
	double gamma = (double)factorial(n - 1);


#if DEBUG
	std::cout << "Gamma Function for " << n << ": " << gamma << std::endl;
#endif // DEBUG


	return gamma;
}

std::vector<double> StatisticsObject::Get_LCG_Stream(int x0, int a, int c, int m, int N)
{
	std::vector<int> xStream;
	std::vector<double> stream;
	double xi = 0;
	double ri = 0;

	for (int i = 1; i <= N; i++) {
		if (i == 1) {
			xStream.push_back((a * x0 + c) % m);
		}
		else {
			xStream.push_back((a * xStream[i - 1] + c) % m);
		}
		xi = xStream[i];
		ri = ((double)xi / (double)m);
		stream.push_back(ri);
	}

	return stream;
}

std::vector<double> StatisticsObject::Chi_Square_Distribution(int dof, int numPoints)
{
	// Plug in values to the chi-square distribution pdf

	double stepSize = 0.01;

	std::vector<double> points;
	std::string filename1 = ".\\Output Files\\ChiSquareDistribution.txt";
	std::string filename2 = ".\\Output Files\\ChiSquareXValues.txt";

	WriteToFile(filename1, "", 1);
	WriteToFile(filename2, "", 1);

	for (int i = 0; i < numPoints; i++) {
		
		double x = i * stepSize;
		double numerator = (pow(x, ((dof / 2.0) - 1.0))) * exp(-x / 2.0);
		double denominator = (pow(2.0, (dof / 2.0)) * GammaFunction((dof / 2.0)));
		double currentPoint = numerator / denominator;

		points.push_back(currentPoint);

		WriteToFile(filename1, currentPoint, 0);
		WriteToFile(filename2, x, 0);
	}

	return points;
}

double StatisticsObject::ChiSquareCDF(double dof, double x_upper, int steps)
{    
	// Define step size
	double stepSize = x_upper / steps;

	// Initialize area
	double area = 0.0;

	// Apply Simpson's Rule
	for (int i = 0; i <= steps; i++) {
		double x = i * stepSize;
		double multiplier = 1.0;

		// Check if it's the first point or the last point
		if (i == 0 || i == steps)
			multiplier = 1;
		// Check if it's an odd or even step
		else if (i % 2 == 1)
			multiplier = 4;
		else if (i % 2 == 0)
			multiplier = 2;

		double y = PDF_Chi_Square(x, dof);

		area += multiplier * y;
	}

	// Multiply the result by h/3
	area *= (stepSize / 3.0);

	if (area > 1.0) {
		std::cout << "\nERROR IN INTEGRATING CHI_SQUARE PDF. AREA > 1." << std::endl;
		std::cout << "CALCULATED AREA: " << area << std::endl << std::endl;
		return -1.0;
	}

	return area;
}

double StatisticsObject::PDF_Chi_Square(double x, double dof)
{
	// Evaluate the chi-square PDF at x and t
	// used for integrating for the CDF

	int intDOF = (int)dof;

	double numerator = (pow(x, ((intDOF / 2.0) - 1.0))) * exp(-x / 2.0);
	double denominator = (pow(2.0, (intDOF / 2.0)) * GammaFunction((intDOF / 2.0)));
	double currentPoint = numerator / denominator;
	return currentPoint;
}