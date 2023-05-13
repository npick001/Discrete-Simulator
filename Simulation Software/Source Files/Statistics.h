#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
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

void ReadStatFile(std::string filename);
void CreateDummyData(int dataSize, Distribution* distribution);
void ResetStatistics();
void ComputeFilePDF();
void StatTesting();
void ChiSquareTest();