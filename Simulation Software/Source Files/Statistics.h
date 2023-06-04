#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <list>
#include <random>
#include "Distribution.h"
#include "Directives.h"

// Differential equation callback function
typedef double (*callback_function_DBL)(double,double);

template <typename T>
void WriteToFile(std::string filename, T data, int openMode) {

	std::ofstream outfile;

	switch (openMode) {
	case 0:
		outfile.open(filename, std::ios_base::app);
		break;
	case 1:
		outfile.open(filename, std::ios_base::trunc);
		break;
	}

	if (!outfile.is_open()) {
		std::cout << filename << " WAS UNABLE TO OPEN. " << std::endl;
	}

	outfile << data << '\n';
	outfile.close();
}

class Entity;

// Self contained file reader and container.
// Provides support for generating probability density histograms for input data.
class StatisticsObject
{
public: 
	StatisticsObject(int histogramIntervals);

	// required file format is:
	// int number of data points \n
	// data point i \n until eof
	void ReadStatFile(std::string filename);
	void CreateDummyData(int dataSize, Distribution* distribution);
	void ResetStatistics();
	void ComputeFilePDF();
	void StatTesting();

	// setter for tolerance
	// default tolerance = 10^-6
	// default iterations = 500
	inline void SetIterativeTolerance(double tolerance) { m_iterationTolerance = tolerance; }

	// GAMMA FUNCTION
	// CONSTRAINED x > 0 for x = 1,2,3,...,n
	double GammaFunction(unsigned int n);

	// NEED TO BE IMPLEMENTED

	// for this to work, data.size() must == probabilities.size()
	// Test returns p-value
	double ChiSquareTest(std::vector<double> data, std::vector<double> probabilities);
	void KolmogorovSmirnovTest();
	void LeastSquaresEstimate();
	//////////////////////////

	// required file format is:
	// int number of data points \n
	// data point i \n until eof
	void GenerateHistogramFromData(std::string fileName);
	inline std::vector<double> GetDataset() { return m_dataset; }

	// needs to be tested
	double ChiSquareCDF(double dof, double x_upper, int steps);
	double PDF_Chi_Square(double x, double dof);

	// Approximate the critical value using Newton-Raphson iteration
	// max iterations = 2500
	// sooooo close
	double ChiSquareCriticalValue(double alpha, double dof);

	/*
		Define the Linear Congruential Generator (LCG).
		Params:
		- x0: Seed
		- a: Multiplier
		- c: increment
		- m: modulus
		- N: number of RN to generate

		This function returns a stream of numbers (0, 1), which may or may not be
		a valid number generator for analysis.

		Primarily used for testing Chi-Square Test.
	*/
	std::vector<double> Get_LCG_Stream(int x0, int a, int c, int m, int N);

protected:
	// CHI-SQUARE DISTRIBUTION => WORKING
	std::vector<double> Chi_Square_Distribution(int dof, int numPoints);

	// used for finding the quantile of the chi-square distribution
	// looking for a value x such that CDF(x) = alpha 
	// returns x => this is the critical value
	// => NOT WORKING
	double NewtonsMethod(double alpha, double initialGuess);

	// Maximum Likelihood Estimators
	std::vector<double> MLE_Exponential(std::vector<double>& data);
	std::vector<double> MLE_Normal(std::vector<double>& data);
	std::vector<double> MLE_Triangular(std::vector<double>& data);
	std::vector<double> MLE_Poisson(std::vector<double>& data);
	std::vector<double> MLE_Weibull(std::vector<double>& data);
	std::vector<double> MLE_Erlang(std::vector<double>& data);

	// Recursively compute the factorial => WORKING
	unsigned long long factorial(unsigned int n);

	// NOTHING CREATED
	//double GetChiSquareCriticalValue(double alpha, double dof);

private:
	// distributions to test input data against
	std::list<Distribution*> m_testDistributions;

	// Input data
	std::vector<double> m_dataset;
	int m_numDataPoints;
	float m_dataRange;
	float m_binSize;
	std::string m_currentFile;
	float m_xMinimum, m_xMaximum;
	float m_yMinimum, m_yMaximum;

	// Histogram
	int m_numHistogramIntervals;
	std::vector<double> m_intervalDensity;

	// Theoretical distributions
	int m_numCurvePoints;

	// Newtons Method
	double m_iterationTolerance;
	double m_iterations;

	// goodness of fit tests
	//std::chi_squared_distribution<double> m_chi_square_dist;
	float m_pValue;
};