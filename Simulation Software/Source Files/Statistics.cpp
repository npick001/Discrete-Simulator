#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Statistics.h"

// Input data
std::unique_ptr<float[]> dataset(new float[0]);
int numDataPoints = 0;
float dataRange = 0;
float binSize = 0;
std::string currentFile;
float xMinimum = 0, xMaximum = 0;
float yMinimum = 0, yMaximum = 0;

// Histogram
const int numHistogramIntervals = 10;
std::unique_ptr<float[]> intervalDensity(new float[numHistogramIntervals]);

// Theoretical distributions
int numCurvePoints = 100;
float p_value = -1;

void ResetStatistics() {

	dataset.reset(new float[0]);
	numDataPoints = 0;
	dataRange = 0;
	binSize = 0;
	currentFile = "";
	intervalDensity.reset(new float[numHistogramIntervals]);
	numCurvePoints = 100;
	p_value = -1;
#if DEBUG
	std::cout << "Resetting Statistics.\n" << std::endl;
#endif // DEBUG

}

void ReadStatFile(std::string filename)
{
	ResetStatistics();

	std::ifstream iFile;
	iFile.open(filename);
	std::string currentFile = filename;

	// make sure the file can open correctly
	// if opened correctly, then save the first value as number of points
	if (iFile.is_open()) {
		iFile >> numDataPoints;
	}

	// allocate memory for all the data points in file
#if DEBUG
	std::cout << "Data set size: " << numDataPoints << '\n';
#endif // DEBUG
	dataset.reset(new float[numDataPoints]);
	xMinimum = FLT_MAX, xMaximum = FLT_MIN;
	yMinimum = FLT_MAX, yMaximum = FLT_MIN;

	int currentLine = 0;
	float dataSum = 0;

	// get all lines in file
	while (!iFile.eof() && currentLine < numDataPoints) {
		iFile >> dataset[currentLine];

		// compute data min and max
		if (dataset[currentLine] < xMinimum)
			xMinimum = dataset[currentLine];
		if (dataset[currentLine] > xMaximum)
			xMaximum = dataset[currentLine];

		dataSum += dataset[currentLine];
		currentLine++;
	}
	float dataAvg = dataSum / numDataPoints;

	// make sure to close file when done
	iFile.close();
}

void ComputeFilePDF() {
	// go through and check every value for if its in current bin
	// if so, increment count to current bin

	// allocate new memory
	intervalDensity.reset(new float[numHistogramIntervals]);

	// initialize memory
	for (int j = 0; j < numHistogramIntervals; j++) {
		intervalDensity[j] = 0;
	}

	dataRange = xMaximum - xMinimum;
	binSize = dataRange / (float)numHistogramIntervals;

	// define the intervals for the bings
	std::unique_ptr<float[]> ranges(new float[numHistogramIntervals + 1]);
	ranges[0] = xMinimum;
	for (int i = 1; i <= numHistogramIntervals; i++) {
		ranges[i] = ranges[i - 1] + binSize;
#if DEBUG
		std::cout << "[" << ranges[i - 1] << ", " << ranges[i] << "]" << '\n';
#endif // DEBUG
	}
#if DEBUG
	std::cout << "Data range: " << dataRange << ", Bin Size: " << binSize << '\n';
	std::cout << "X max: " << xMaximum << ", X min: " << xMinimum << '\n';
#endif // DEBUG

	// put number of values into respective bin
	for (int i = 0; i < numDataPoints; i++) {

		for (int j = 0; j < numHistogramIntervals; j++) {
			if (dataset[i] < ranges[j+1]) {
				//std::cout << "Increasing bin " << j << '\n';
				intervalDensity[j]++;
				break;
			}
		}
				
		//std::cout << "Data Point: " << dataset[i] << ", Bin Index: " << binIndex << '\n';
	}
	
#if WRITETOFILE
	WriteFile("HistogramDensity.txt", "Resetting file.", 1);
#endif // WRITETOFILE
	double total = 0;
	for (int j = 0; j < numHistogramIntervals; j++) {

		// get the density of points in bin / total number of points
		intervalDensity[j] /= numDataPoints;
		//intervalDensity[j] /= binSize;
		//intervalDensity[j] /= numHistogramIntervals;
		
		// compute pdf min and max
		if (intervalDensity[j] < yMinimum)
			yMinimum = intervalDensity[j];
		if (intervalDensity[j] > yMaximum)
			yMaximum = intervalDensity[j];
#if DEBUG
		std::cout << "Bin: " << j << ", " << intervalDensity[j] << '\n';
#endif // DEBUG

#if WRITETOFILE
		WriteFile("HistogramDensity.txt", intervalDensity[j], 0);
#endif // WRITETOFILE
		total += intervalDensity[j];
	}
	std::cout << "\nTOTAL: " << total << '\n';

	dataRange = yMaximum - yMinimum;
}

// Generate some dummy data to determine if this is working
void CreateDummyData(int dataSize, Distribution* distribution) {

	Distribution* dist = distribution;
	std::unique_ptr<double[]> distData(new double[dataSize]);

#if WRITETOFILE
	WriteFile("test.txt", dataSize, 1);
#endif // WRITETOFILE

	for (int i = 0; i < dataSize; i++) {
		distData[i] = dist->GetRV();

#if WRITETOFILE
		WriteFile("test.txt", distData[i], 0);
#endif // WRITETOFILE
	}
}

void StatTesting() {
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

	delete tri, norm, exp, uni;
#endif // DEBUG
}

StatisticsObject::StatisticsObject(int histogramIntervals)
{
	// Input data
	dataset.reset(new float[0]);
	numDataPoints = 0;
	dataRange = 0;
	binSize = 0;
	currentFile = "";
	xMinimum = 0, xMaximum = 0;
	yMinimum = 0, yMaximum = 0;

	// Histogram
	numHistogramIntervals = histogramIntervals;
	intervalDensity.reset(new float[numHistogramIntervals]);

	// Theoretical distributions
	int numCurvePoints = 100;
	float p_value = -1;
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
		iFile >> numDataPoints;
	}

	// allocate memory for all the data points in file
#if DEBUG
	std::cout << "Data set size: " << numDataPoints << '\n';
#endif // DEBUG
	dataset.reset(new float[numDataPoints]);
	xMinimum = FLT_MAX, xMaximum = FLT_MIN;
	yMinimum = FLT_MAX, yMaximum = FLT_MIN;

	int currentLine = 0;
	float dataSum = 0;

	// get all lines in file
	while (!iFile.eof() && currentLine < numDataPoints) {
		iFile >> dataset[currentLine];

		// compute data min and max
		if (dataset[currentLine] < xMinimum)
			xMinimum = dataset[currentLine];
		if (dataset[currentLine] > xMaximum)
			xMaximum = dataset[currentLine];

		dataSum += dataset[currentLine];
		currentLine++;
	}
	float dataAvg = dataSum / numDataPoints;

	// make sure to close file when done
	iFile.close();
}

void StatisticsObject::CreateDummyData(int dataSize, Distribution* distribution)
{
	Distribution* dist = distribution;
	std::unique_ptr<double[]> distData(new double[dataSize]);

#if WRITETOFILE
	WriteFile("test.txt", dataSize, 1);
#endif // WRITETOFILE

	for (int i = 0; i < dataSize; i++) {
		distData[i] = dist->GetRV();

#if WRITETOFILE
		WriteFile("test.txt", distData[i], 0);
#endif // WRITETOFILE
	}

	delete dist;
}

void StatisticsObject::ResetStatistics()
{
	dataset.reset(new float[0]);
	numDataPoints = 0;
	dataRange = 0;
	binSize = 0;
	currentFile = "";
	intervalDensity.reset(new float[numHistogramIntervals]);
	numCurvePoints = 100;
	p_value = -1;
#if DEBUG
	std::cout << "Resetting Statistics.\n" << std::endl;
#endif // DEBUG
}

void StatisticsObject::ComputeFilePDF()
{
	// go through and check every value for if its in current bin
	// if so, increment count to current bin

	// allocate new memory
	intervalDensity.reset(new float[numHistogramIntervals]);

	// initialize memory
	for (int j = 0; j < numHistogramIntervals; j++) {
		intervalDensity[j] = 0;
	}

	dataRange = xMaximum - xMinimum;
	binSize = dataRange / (float)numHistogramIntervals;

	// define the intervals for the bings
	std::unique_ptr<float[]> ranges(new float[numHistogramIntervals + 1]);
	ranges[0] = xMinimum;
	for (int i = 1; i <= numHistogramIntervals; i++) {
		ranges[i] = ranges[i - 1] + binSize;
#if DEBUG
		std::cout << "[" << ranges[i - 1] << ", " << ranges[i] << "]" << '\n';
#endif // DEBUG
	}
#if DEBUG
	std::cout << "Data range: " << dataRange << ", Bin Size: " << binSize << '\n';
	std::cout << "X max: " << xMaximum << ", X min: " << xMinimum << '\n';
#endif // DEBUG

	// put number of values into respective bin
	for (int i = 0; i < numDataPoints; i++) {

		for (int j = 0; j < numHistogramIntervals; j++) {
			if (dataset[i] < ranges[j + 1]) {
				//std::cout << "Increasing bin " << j << '\n';
				intervalDensity[j]++;
				break;
			}
		}

		//std::cout << "Data Point: " << dataset[i] << ", Bin Index: " << binIndex << '\n';
	}

#if WRITETOFILE
	WriteFile("HistogramDensity.txt", "Resetting file.", 1);
#endif // WRITETOFILE
	double total = 0;
	for (int j = 0; j < numHistogramIntervals; j++) {

		// get the density of points in bin / total number of points
		intervalDensity[j] /= numDataPoints;
		//intervalDensity[j] /= binSize;
		//intervalDensity[j] /= numHistogramIntervals;

		// compute pdf min and max
		if (intervalDensity[j] < yMinimum)
			yMinimum = intervalDensity[j];
		if (intervalDensity[j] > yMaximum)
			yMaximum = intervalDensity[j];
#if DEBUG
		std::cout << "Bin: " << j << ", " << intervalDensity[j] << '\n';
#endif // DEBUG

#if WRITETOFILE
		WriteFile("HistogramDensity.txt", intervalDensity[j], 0);
#endif // WRITETOFILE
		total += intervalDensity[j];
	}
	std::cout << "\nTOTAL: " << total << '\n';

	dataRange = yMaximum - yMinimum;
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

void StatisticsObject::GenerateHistogramFromData(std::string fileName)
{
	ResetStatistics();
	ReadStatFile(fileName);
	ComputeFilePDF();
}

int main() {

	StatisticsObject statobj(50);

	statobj.StatTesting();

	//_CrtDumpMemoryLeaks();
	return 0;
}