#include "Sensor.hpp"

Sensor::Sensor(){
	// fill with 0's
	for (int i = 0; i < SIZE; i++){
		data.push_back(0);
	}
	m_samplingRate = 360.0;
}

void Sensor::smooth(int kernelSize){
	int index, count;
	float dataS[SIZE];
	for(int i = 0; i < SIZE; i++){
		float average = 0;
		count = 0;
		for(int j = -kernelSize; j < kernelSize + 1; j++){
			index = i + j;
			if(index < 0 || index >= SIZE) continue;
			average += workingBuffer[index];
			count++;
		}
		average /= count;
		dataS[i] = average;
	}
	for(int i = 0; i < SIZE; i++){
		workingBuffer[i] = dataS[i];
	}
}

void Sensor::thresholdValues(float t){
	for(int i = 0; i < SIZE; i++){
		if(workingBuffer[i] < t) workingBuffer[i] = 0;
		else workingBuffer[i] = workingBuffer[i];
	}	
}

void Sensor::scaleDataValues(){
	float newmin = 0;
	float newmax = 1;
	float prevmin = std::numeric_limits<float>::max();
	float prevmax = std::numeric_limits<float>::min();
	for(int i = 0; i < SIZE; i++){
		if(workingBuffer[i] <= prevmin) prevmin = workingBuffer[i];
		if(workingBuffer[i] >= prevmax) prevmax = workingBuffer[i];
	}
	// scale to new values
	for(int i = 0; i < SIZE; i++){
		workingBuffer[i] = 1 - (((newmax - newmin)*(workingBuffer[i] - prevmin)) / (prevmax - prevmin)) + newmin;
		//cout << "new workingBuffer : " << workingBuffer[i] << endl;
	}
}

void Sensor::normaliseDataValues(){
	float total = 0;
	for(int i = 0; i < SIZE; i++){
		total += data[i];
	}
	float mean = total / SIZE;
	// scale to new values
	for(int i = 0; i < SIZE; i++){
		workingBuffer[i] = data[i] - mean;
		//cout << "new data : " << data[i] << endl;
	}
}


// extremely naive approach
// TODO: wait for more samples and timestamp cycle detection
bool Sensor::isNewCycle(){
	// Check outcome of calcPeaks peak vector to see whether
	// there is a new peak or not
    return false;
}

void Sensor::copyToWorkingBuffer(){
	for(int i = 0; i < SIZE; i++){
		workingBuffer[i] = data[i];
	}
}

float Sensor::estimateAngle(){

	// Get time period
	float timePeriod = estimatePeriod();

	// Calculate angle based on estimated period and time since last peak
	// Note that timeSinceLastPeak will get set to 0 on discovery of a new peak
	float periodOfSensor = 1 / m_samplingRate;
	float angle = (timeSinceLastPeak * periodOfSensor * M_PI * 2) / timePeriod;

	return angle;
}

// extremely naive approach, but the filtering seems to make it work ok
// effectiveness is highly dependant on the threshold
float Sensor::calcPeaks(){

	//ofstream outfile;
	//ofstream wavefile;
	//outfile.open("peaks.txt");
	//wavefile.open("wave.txt");

    //cout << "calc peaks" << endl;

    if(data.size() < SIZE) return 1.0f;

	// increment time since detection of last peak
	timeSinceLastPeak++;


    //cout << "copy to worker buffer" << endl;
	copyToWorkingBuffer();


	//smooth(3);
	//smooth(2);

	scaleDataValues();
	thresholdValues(0.5f);
	
	// copy the array to file
	//for (int i = 0; i < SIZE; i++) {
		//wavefile << to_string(workingBuffer[i]) << endl;
	//}

	int rising_threshold_count = 3;
	int current_rising_count = 0;
	int current_falling_count = 0;
	float previous_value = 0;
	bool falling = false;

	// to calculate time at which the data sample is recieved
	// given the index, it is: 
	//
	// time = currentTime - ((SIZE-1) - i)
	//
	vector<int> newPeaks;


	for(int i = 0; i < SIZE; i++){
		//cout << "buffer val [" << i << "]" << workingBuffer[i] << endl;

		if(falling){
			if(workingBuffer[i] == 0){
				falling = false;
			}
		}
		else{
			if(workingBuffer[i] > previous_value){
				current_rising_count++;
			}
			if(workingBuffer[i] < previous_value
					&& current_rising_count > rising_threshold_count){
				current_rising_count = 0;
				falling = true;
				int time = currentTime - ((SIZE-1) - i);
				newPeaks.push_back(time);
			}
		}
		previous_value = workingBuffer[i];

	}

	//for(int i = 0; i < newPeaks.size(); i++){
	//	cout << newPeaks[i] << endl;
	//}

	// check if newest peak is already contained in the vector
	bool contained = false;
	for (int i = 0; i < oldPeaks.size(); i++) {
		if(oldPeaks[i] == newPeaks.back()){
			contained = true;
			break;
		}
	}

    if(newPeaks.size() <= 1) return 1.0f;

	if(!contained){
		newCycle = true;
		timeSinceLastPeak = currentTime - newPeaks.back();
		cout << "New cycle:" << newPeaks.back() << endl;
	}

	// set old peaks to new peaks
	
	oldPeaks.clear();
	for(int i = 0; i < newPeaks.size(); i++){
		oldPeaks.push_back(newPeaks[i]);
	}

    //cout << "current val count : " << currentTime << endl;
	float average_period = 0;
	//cout << "newPeaks count: " << newPeaks.size() << endl;


	// calculate differences and average
	for(int i = 0; i < newPeaks.size() - 1; i++){
		average_period += newPeaks[i+1] - newPeaks[i];
	}
	//cout << "Peaks counted : " << newPeaks.size() << endl;
	average_period /= newPeaks.size() - 1;

	// calculate in milliseconds, using sampling rate
	float time_period = average_period / m_samplingRate;
	//float time_period = average_period;
	//wavefile.close();
	//outfile.close();

    if(time_period > 2.0f)
        time_period = 2.0f;
    if(time_period < 0.2f)
        time_period = 0.2f;

	return time_period;

}

float Sensor::estimatePeriod(){
	if(data.size() < SIZE) return 0;

	switch(method){
		case FFT:
			return calcFFT();
			break;
		case PEAKS:
			return calcPeaks();
			break;
	}
}

void Sensor::push_back(float value){
	//if(value < threshold)	data.push_back(value);
	//else					data.push_back(threshold);
	data.push_back(value);
	data.pop_front();
	currentTime++;
}

float Sensor::calcFFT(){
	ofstream outfile;
	ofstream wavefile;
	outfile.open("magnitudes.txt");
	wavefile.open("wave.txt");

	// copy the array
	for (int i = 0; i < SIZE; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
		wavefile << to_string(data[i]) << endl;
	}

	fftw_plan my_plan = fftw_plan_dft_1d(SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(my_plan);

	for (int i = 0; i < SIZE; i++) {
		magnitude[i] = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
		outfile << to_string(magnitude[i]) << endl;
	}

	int peak_frequency = 0;
	float peak_val = 0;

	for(int i = 0; i < SIZE/2; i++){
		if(magnitude[i] > peak_val){
			peak_val = magnitude[i];
			peak_frequency = i;
		}
	}

	float time_period = SIZE / peak_frequency;
	cout << "peak frequency : " << peak_frequency << endl;

	outfile.close();
	wavefile.close();
	fftw_destroy_plan(my_plan);

	// calculate in milliseconds, using sampling rate
	time_period = time_period * 1000 / m_samplingRate;

	return time_period;
}

Sensor::Sensor(string filename){
	m_samplingRate = 50.0;
	ifstream infile(filename);

	float total = 0;
	int i = 0;
	float x;

	for(; i < SIZE; i++){
		data.push_back(0);
	}

	i = 0;

	while(infile >> x && i < SIZE){
		push_back(x);
		total += x;
		i++;
	}
		
	float mean = total / i;
	// fill with mean thus far
	for (; i < SIZE; i++){
		push_back(mean);
	}
}

void Sensor::createFakeData(){
	for (int i = 0; i < SIZE; i++) {
		float rad = (float)i * M_PI / 180.0;
		push_back(sin(rad));
	}
}

void Sensor::timer_start(Sensor *sensor, unsigned int interval){

	std::thread([sensor, interval]() {
		while(true){
			float val = sin((float)sensor->fakeDataIncrement * M_PI / 70.0);
			//cout << "Fake data : " << val << endl;
			sensor->push_back(val);
			sensor->fakeDataIncrement++;
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			float est = sensor->estimateAngle();
			cout << "E : " << est << endl;
		}
	}).detach();

}

void Sensor::initialiseFakeStream(float frequency){
	fakeDataIncrement = 0;
	timer_start(this, (int)frequency);
}
