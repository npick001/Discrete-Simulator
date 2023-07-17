#include "Utility.h"

std::vector<double> RungeKutta4(callback_function_DBL pdf, double x_init, double upperbound, int steps)
{

	/******************

	for solving the initial value problem:
	dy/dt = f(t,y), y(t_0) = y_0

	Apply the Runge Kutta 4 integrator method on the callback function.
	Returns the vector of points computed by the integrator for plotting.

	******************/

	std::string filename1 = ".\\Output Files\\RungeKuttaOutput.txt";
	std::string filename2 = ".\\Output Files\\RungeKuttaOutputXValues.txt";

	// vector to return
	std::vector<double> x;
	x.push_back(x_init);

	// define step guesses
	double k1 = 0, k2 = 0, k3 = 0;

	// define guess derivatives
	double k1_deriv = 0, k2_deriv = 0, k3_deriv = 0, k4_deriv = 0;
	k1_deriv = x_init;

	// for time advance
	double stepSize = ((upperbound - x_init) / steps);

	// time
	std::vector<double> t;
	t.push_back(0.0);

	WriteToFile(filename1, x[0], 1);
	WriteToFile(filename2, t[0], 1);

	// RK Iteration
	for (int i = 0; i < steps; i++) {

		auto tNow = t[i];
		auto xNow = x[i];

		// k1 is the increment based on the slope at the start of the interval
		double k1 = stepSize * pdf(xNow, tNow);

		// k2 is the increment based on the slope at the midpoint of the interval, using k1 to determine the value
		double k2 = stepSize * pdf(xNow + 0.5 * k1, tNow + 0.5 * stepSize);

		// k3 is again the increment based on the slope at the midpoint, but now using k2 to determine the value
		double k3 = stepSize * pdf(xNow + 0.5 * k2, tNow + 0.5 * stepSize);

		// k4 is the increment based on the slope at the end of the interval, using k3 to find the function value
		double k4 = stepSize * pdf(xNow + k3, tNow + stepSize);

		// Weighted sum of k values is used to generate the next x value
		double xNext = xNow + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
		double tNext = tNow + stepSize;

		// save computed values for step
		x.push_back(xNext);

		// advance time
		t.push_back(tNext);

		WriteToFile(filename1, xNext, 0);
		WriteToFile(filename2, tNext, 0);
	}

	auto returnVector = x;

	return returnVector;
}

unsigned long long factorial(unsigned int n)
{
	unsigned long long sum = 1;
	for (int i = n; i > 0; i--) {
		sum *= i;
	}

	std::cout << "Factorial for " << n << ": " << sum << std::endl;
	return sum;
}

double GammaFunction(unsigned int n)
{
	double gamma = (double)factorial(n - 1);

	std::cout << "Gamma Function for " << n << ": " << gamma << std::endl;

	return gamma;
}

std::vector<double> Chi_Square_Distribution(int dof, int numPoints, bool writeToFile)
{
	// Plug in values to the chi-square distribution pdf

	double stepSize = 0.01;

	std::vector<double> points;
	std::string filename1 = ".\\Output Files\\ChiSquareDistribution.txt";
	std::string filename2 = ".\\Output Files\\ChiSquareXValues.txt";

	if (writeToFile) {
		WriteToFile(filename1, "", 1);
		WriteToFile(filename2, "", 1);
	}

	for (int i = 0; i < numPoints; i++) {

		double x = i * stepSize;
		double numerator = (pow(x, ((dof / 2) - 1))) * exp(-x / 2);
		double denominator = (pow(2, (dof / 2)) * GammaFunction((dof / 2)));
		double currentPoint = numerator / denominator;

		points.push_back(currentPoint);

		if (writeToFile) {
			WriteToFile(filename1, currentPoint, 0);
			WriteToFile(filename2, x, 0);
		}
	}

	return points;
}

#if USE_WX
void wxWriteToFile(std::string filename, std::string data, int openMode) {

	wxString fileName(filename);
	wxFileName wxfilename(fileName);

	wxFile file(wxfilename.GetFullPath(), openMode == 0 ? wxFile::write_append : wxFile::write);
	if (!file.IsOpened()) {
		std::cout << filename << " WAS UNABLE TO OPEN. " << std::endl;
		return;
	}

	file.Write(wxString(data));
	file.Close();
}
#endif // USE_WX
