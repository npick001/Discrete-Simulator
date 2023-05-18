#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <list>
//#include <boost/math/distributions/chi_squared.hpp>
//#include <boost/math/distributions/kolmogorov_smirnov.hpp>
#include "Distribution.h"
#include "Directives.h"

template <typename T>
void WriteFile(std::string filename, T data, int openMode) {

	std::ofstream outfile;

	switch (openMode) {
	case 0:
		outfile.open(filename, std::ios_base::app);
		break;
	case 1:
		outfile.open(filename, std::ios_base::trunc);
		break;
	}

	outfile << data << '\n';
	outfile.close();
}

// Self contained fild reader and container.
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

	// required file format is:
	// int number of data points \n
	// data point i \n until eof
	void GenerateHistogramFromData(std::string fileName);

private:
	// distributions to test input data against
	std::list<Distribution*> testDistributions;

	// Input data
	std::unique_ptr<float[]> dataset;
	int numDataPoints;
	float dataRange;
	float binSize;
	std::string currentFile;
	float xMinimum, xMaximum;
	float yMinimum,  yMaximum;

	// Histogram
	int numHistogramIntervals;
	std::unique_ptr<float[]> intervalDensity;

	// Theoretical distributions
	int numCurvePoints;
	float p_value;
};