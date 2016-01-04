/* Sensor class - Jess McIntosh

-FIFO data
-Member function push_back(float) to store incoming data
-Member function isNewCycle() to detect start of a new cycle
-Member function estimatePeriod to estimate the time period

usage:

Sensor sensor = Sensor();
sensor.m_samplingRate = 50.0; // For 50 Hz

<add data here>
sensor.push_back(x);

...

float timePeriod = sensor.estimatePeriod();

*/

#include <fftw3.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <limits>

// For timer
#include <chrono>
#include <thread>
#include <functional>

using namespace std;

#define SIZE 		450	// Size of data buffer (also FFT window)

enum METHOD { FFT, PEAKS };

class Sensor { 
	private:

	float calcPeaks();
	float calcFFT();
	void smooth(int kernelSize);
	void copyToWorkingBuffer();
	void incrementFakeData();
	void thresholdValues(float t);
	void scaleDataValues();
	void writeMagnitudes();

	// working data buffer 
	float workingBuffer[SIZE];
	// raw data buffer
	// index SIZE is the newest data, index 0 is the oldest
	// also note that data at SIZE was received at currentTime
	deque<float> data;

	float magnitude[SIZE];
	int fakeDataIncrement;
	int method = PEAKS;
	//int method = FFT;
	fftw_complex in[SIZE], out[SIZE];
	float threshold = 0.513f;
	float previousEstimate;

	// Counting number of data samples collected from the sensor
	// therefore, real time = currentTime * (1000 / sampling rate)
	int currentTime = 0;
	vector<int> oldPeaks;

	int timeSinceLastPeak = 0;
	
	// flagged upon peak detection
	bool newCycle = false;

	public:
	
	Sensor();
	Sensor(string filename);

	// frequency in Hertz
	void initialiseFakeStream(float frequency);
	void timer_start(Sensor *sensor, unsigned int interval);
	void createFakeData();
	void push_back(float value);
	float estimatePeriod();
	float estimateAngle();
	bool isNewCycle();
	void normaliseDataValues();

	float m_samplingRate;

};

