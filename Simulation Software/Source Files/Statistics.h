#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <cmath>
#include <list>
#include <random>

#include "gcem.hpp"
#include "stats.hpp"
#include "Utility.h"
#include "Distribution.h"
#include "Directives.h"

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
	// MAKE SURE TO ADD FORMATTING CONSTRAINTS TO CODE.
	void ReadStatFile(std::string filename);

	// Returns a vector of size dataSize
	// with random variates drawn from the distribution/
	std::vector<double> CreateDummyData(int dataSize, Distribution* distribution);
	void ResetStatistics();
	void ComputeFilePDF();
	void StatTesting();

	// required file format is:
	// int number of data points \n
	// data point i \n until eof
	Distribution* GenerateBestFitDistribution(std::string fileName);

	// Distribution testing functions
	void GenerateTestDistributions(std::vector<double> data);
	Exponential* GenerateExponentialFromParams(std::vector<double> params);
	Normal* GenerateNormalFromParams(std::vector<double> params);

	// returns a vector containing the intervals for the PDF
	std::vector<double> ComputePDF(std::vector<double> data, int numBins);

	// MUST BE CALLED AFTER GENERATE TEST DISTRIBUTIONS
	Distribution* DetermineBestFit();

	// for this to work, data.size() must == probabilities.size()
	// Test returns p-value
	// Chi-square testing and p-values are approximately correct.
	// code I wrote
	double ChiSquareTest(std::vector<double> data, std::vector<double> probabilities, bool iWrote);

	// code using StatsLib library
	double ChiSquareTest(std::vector<double> observed, std::vector<double> expected);

	double ChiSquareCDF(double dof, double x_upper, int steps);
	double PDF_Chi_Square(double x, double dof);

	// Approximate the critical value using Newton-Raphson iteration
	// max iterations = 2500
	// sooooo close
	double ChiSquareCriticalValue(double alpha, double dof);

	// NEED TO BE IMPLEMENTED //
	void KolmogorovSmirnovTest();
	void LeastSquaresEstimate();
	////////////////////////////


	// setter for tolerance
	// default tolerance = 10^-6
	// default iterations = 500
	inline void SetIterativeTolerance(double tolerance) { m_iterationTolerance = tolerance; }
	inline std::vector<double> GetDataset() { return m_dataset; }

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

	// GAMMA FUNCTION
	// CONSTRAINED x > 0 for x = 1,2,3,...,n
	double GammaFunction(unsigned int n);

	// NOTHING CREATED
	//double GetChiSquareCriticalValue(double alpha, double dof);

private:
	// distributions to test input data against
	std::vector<Distribution*> m_testDistributions;

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
};