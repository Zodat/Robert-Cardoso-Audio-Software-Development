#ifndef audioIO_hpp
#define audioIO_hpp

#include <iostream>
#include <sndfile.h>

using namespace std;
//reads the audio signal
double* audioread(int &len, int &fs,  int &numChannels, string fileName);
//writes out the audio signal
void audiowrite(double *inputBuffer, int bufferLength, int fs, string fileNameOut);
//prints out number of samples
void printSamples(double* inputSignal, int numSamplesToPrint);
//signal processing: scale signal. Changes volume of audio file based on a numerical value
void scaleSignal(double* inputSignal, int numSamples, float scalingFactor);
//signal processing: reverse signal
void reverseSignal(double* inputSignal, int numSamples);
//signal processing: hard clip distorts the audio file. Threshold is determined by a value between 0 and 1
void distortion(double* inputSignal, int numSamples, double threshold);

#endif /* audioIO_hpp */
