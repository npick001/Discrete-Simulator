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
	m_pValue = -1;

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

void StatisticsObject::CreateDummyData(int dataSize, Distribution* distribution)
{
	Distribution* dist = distribution;
	std::unique_ptr<double[]> distData(new double[dataSize]);

#if WRITETOFILE
	WriteToFile("test.txt", dataSize, 1);
#endif // WRITETOFILE

	for (int i = 0; i < dataSize; i++) {
		distData[i] = dist->GetRV();

#if WRITETOFILE
		WriteToFile("test.txt", distData[i], 0);
#endif // WRITETOFILE
	}

	delete dist;
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
	m_pValue = -1;
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
	WriteToFile("HistogramDensity.txt", "Resetting file.", 1);
#endif // WRITETOFILE
	double total = 0;
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
		WriteToFile("HistogramDensity.txt", m_intervalDensity[j], 0);
#endif // WRITETOFILE
		total += m_intervalDensity[j];
	}
	std::cout << "\nTOTAL: " << total << '\n';

	m_dataRange = m_yMaximum - m_yMinimum;
}

double StatisticsObject::ChiSquareTest(std::vector<double> data, std::vector<double> probabilities)
{
	// make sure they are the same size.
	_ASSERT(data.size() == probabilities.size());

	int size = data.size();
	std::vector<double> observed = data;
	std::vector<double> expected;
	double dataSum = 0;
	double testStat = 0;

	// calculate sample size
	for (int i = 0; i < size; i++) {
		dataSum += observed[i];
	}

	// calculate expected values
	for (int i = 0; i < size; i++) {
		expected.push_back(0.0);
		expected[i] = probabilities[i] * dataSum;
	}

	// calculate test statistic
	for (int i = 0; i < size; i++) {
		double numerator = pow((observed[i] - expected[i]), 2);
		double denominator = expected[i];

		double relativeError = (numerator / denominator);
		testStat += relativeError;
	}
	
	int dof = size - 1;
	double p = ChiSquareCDF(dof, testStat, 100);

	std::cout << "P value: " << p << std::endl;

	return p;
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


	double Xi = initialGuess;
	//for (int i = 0; i < m_iterations; i++) {

	//	// eval f()
	//	double c_point = m_chi_square_dist(Xi);
	//	double fx = c_point - alpha;
	//	if (std::abs(fx) < m_iterationTolerance) {
	//		return Xi;
	//	}
	//}
	//	
	//	// take the central difference to get the derivative of fx
	//	// these had to be stored in variables.
	//	// The chi-square distribution doesn't like taking in expressions
	//	double xPlus = Xi + m_iterationTolerance;
	//	double xMinus = Xi - m_iterationTolerance;

	//	// eval f'()
	//	// find the slope at the current point by using the 
	//	// points on either side of Xi using the iteration tolerance.
	//	double dfx = (m_chi_square_dist(xPlus) - m_chi_square_dist(xMinus)) / (2.0 * m_iterationTolerance);
	//	if (dfx == 0.0) {

	//		// would be div by 0
	//		std::cout << "Warning: derivative is 0, Newton's Method may fail." << std::endl;
	//		dfx = m_iterationTolerance;
	//	}

	//	// actually newtons method
	//	Xi -= fx / dfx;
	//}

	return Xi;
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
	//Triangular* tri = new Triangular(1, 2, 3);
	//Normal* norm = new Normal(5, 1);
	//Exponential* exp = new Exponential(5);
	//Uniform* uni = new Uniform(-1, 1);
	//int dataPoints = 500;

	//// UNIFORM
	//std::cout << "\nUNIFORM DISTRIBUTION (-1, 1)\n";
	//CreateDummyData(dataPoints, uni);
	//ReadStatFile("test.txt");
	//ComputeFilePDF();

	//// NORMAL
	//std::cout << "\nNORMAL DISTRIBUTION (5, 1)\n";
	//CreateDummyData(dataPoints, norm);
	//ReadStatFile("test.txt");
	//ComputeFilePDF();

	//std::vector<double> normParams = MLE_Normal(m_dataset);
	//Normal* norm_MLE = new Normal(normParams[0], normParams[1]);

	//std::cout << "\n MLE FROM NORMAL DISTRIBUTION (5, 1)\n";
	//CreateDummyData(dataPoints, norm_MLE);
	//ReadStatFile("test.txt");
	//ComputeFilePDF();

	//// EXPONENTIAL
	//std::cout << "\nEXPONENTIAL DISTRIBUTION (5)\n";
	//CreateDummyData(dataPoints, exp);
	//ReadStatFile("test.txt");
	//ComputeFilePDF();

	//// TRIANGULAR
	//std::cout << "\nTRIANGULAR DISTRIBUTION (1, 2, 3)\n";
	//CreateDummyData(dataPoints, tri);
	//ReadStatFile("test.txt");
	//ComputeFilePDF();

	//ResetStatistics();






#endif // DEBUG
}

// NOT TESTED
void StatisticsObject::GenerateHistogramFromData(std::string fileName)
{
	ResetStatistics();
	ReadStatFile(fileName);
	ComputeFilePDF();
}

std::vector<double> StatisticsObject::MLE_Exponential(std::vector<double>& data)
{
	std::vector<double> estimatedParams;
	double sum = 0;
	double size = data.size();

	for (int i = 0; i < size; i++) {
		sum += data[i];
	}

	double MLE_Estimate = size / sum;
	estimatedParams.push_back(MLE_Estimate);

	return estimatedParams;
}

std::vector<double> StatisticsObject::MLE_Normal(std::vector<double>& data)
{
	std::vector<double> estimatedParams;
	double sum = 0;
	double size = data.size();
	double variance = 0;
	
	// Sum all the data
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}

	// Calculate mean
	double meanEstimate = size / sum;
	estimatedParams.push_back(meanEstimate);
	std::cout << "Normal Mean Estimate: " << meanEstimate << '\n';

	// Calculate variance
	double varEstimate = 0;
	for (int i = 0; i < size; i++) {
		variance += pow((data[i] - meanEstimate), 2);
	}
	varEstimate = variance / (size - 1);
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
		double numerator = (pow(x, ((dof / 2) - 1))) * exp(-x / 2);
		double denominator = (pow(2, (dof / 2)) * GammaFunction((dof / 2)));
		double currentPoint = numerator / denominator;

		points.push_back(currentPoint);

		WriteToFile(filename1, currentPoint, 0);
		WriteToFile(filename2, x, 0);
	}

	return points;
}

double StatisticsObject::ChiSquareCDF(double dof, double x_upper, int steps)
{
	// define step size
	double stepSize = x_upper / steps;

	// initialize x and area under curve
	double x = 0.001;
	double area = 0.0;

	// container to hold the points for testing
	std::vector<double> points;
	points.push_back(x);

	std::vector<double> areas;

	// PDF Manual Integration
	for (int i = 1; i <= steps; i++) {

		// IF CONFUSED ON WHAT IS HERE, REFERENCE
		// NICKS NOTES ON HIS REMARKABLE

		double h = stepSize;
		double y1 = PDF_Chi_Square(x, dof);
		double y2 = PDF_Chi_Square(x + h, dof);

		// Split section into rectangle and triangle
		// get areas for each separately
		// then combine for approximated section area
		double rectArea = y1 * h;
		double triArea = (1 / 2) * h * (y2 - y1);

		// Combine terms to estimate area under curve for this segment
		double sectionArea = (rectArea + triArea);

		// is this producing the right values?
		areas.push_back(sectionArea);

		// Accumulate total area under curve
		area += sectionArea;

		// advance x
		x += stepSize;
		points.push_back(x);
	}

	//for (int i = 0; i < points.size(); i++) {
	//	std::cout << "Step: " << i << ", Value: " << points[i] << std::endl;
	//}

	double totalArea = 1 - area;
	std::cout << "Definite Integral Evaluated: " << totalArea << std::endl;

	return area;
}

double StatisticsObject::PDF_Chi_Square(double x, double dof)
{
	// Evaluate the chi-square PDF at x and t
	// used for integrating for the CDF
	double numerator = (pow(x, ((dof / 2) - 1))) * exp(-x / 2);
	double denominator = (pow(2, (dof / 2)) * GammaFunction((dof / 2)));
	double currentPoint = numerator / denominator;
	return currentPoint;
}

