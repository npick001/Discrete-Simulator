#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/***************************************

This file denotes completed functions that may be useful later.
All functions should be COMPLETE and tested before addition.

***************************************/

// Differential equation callback function
typedef double (*callback_function_DBL)(double, double);

// Define the start of the Runge Kutta family numerical integrators.
// works but apparently not for what I needed it for. I cry.
std::vector<double> RungeKutta4(callback_function_DBL pdf, double x_init, double upperbound, int steps);

// Iteratively compute the factorial
unsigned long long factorial(unsigned int n);

// GAMMA FUNCTION
// CONSTRAINED x > 0 for x = 1,2,3,...,n
double GammaFunction(unsigned int n);

// CHI-SQUARE DISTRIBUTION
// returns vector of points in the chi-square distribution
// also can write the results to a file, determined by last param
std::vector<double> Chi_Square_Distribution(int dof, int numPoints, bool writeToFile);

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