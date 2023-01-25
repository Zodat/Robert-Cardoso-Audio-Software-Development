#include "stdafx.h"
#include "audioIO.h"

//Reads the audio signal
double* audioread(int& len, int& fs, int& numChannels, string fileName)
{
    int readStatus = 1;
    // Reads the audio file's header.
    SF_INFO fileInfo;
    SNDFILE* sndFile = sf_open(fileName.c_str(), SFM_READ, &fileInfo);
    // Checks to see if the audio file was read properly.
    if (sndFile == NULL)
    {
        cout << "Error reading file..." << endl;
        readStatus = 0;
        return NULL;
    }
    // Allocates some memory for the audio data to be brought into the code.
    double* x = new double[fileInfo.frames];
    sf_readf_double(sndFile, x, fileInfo.frames);
    sf_close(sndFile);

    // Sets the other audio file properties (passed by &ref).
    len = (int)fileInfo.frames;
    fs = fileInfo.samplerate;
    numChannels = fileInfo.channels;

    // Returns the audio buffer.
    return x;
}

//Writes out the audio signal
void audiowrite(double* inputBuffer, int bufferLength, int fs, string outputFileName)
{
    // Creates the new file's settings (16bit Mono PCM).
    SF_INFO info;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    info.channels = 1;
    info.samplerate = fs;

    // Opens sound file for writing
    SNDFILE* sndFile = sf_open(outputFileName.c_str(), SFM_WRITE, &info);

    // Checks the file was correctly opened.
    if (sndFile == NULL)
    {
        cout << "Error: Could not write file " << endl;
        return;
    }
    // Writes frames.
    sf_writef_double(sndFile, inputBuffer, bufferLength);

    sf_write_sync(sndFile);
    sf_close(sndFile);
}

//Prints out number of samples.
void printSamples(double* inputSignal, int numSamplesToPrint)
{
    cout << "Samples 0 - " << numSamplesToPrint-1 << ":" << endl;
    //Goes through the array of samples printed.
    for (int i = 0; i < numSamplesToPrint; i++)
    {
        cout << i << ": " << inputSignal[i] << endl;
    }
};
//signal processing: scale signal. Changes volume of audio file based on a numerical value
void scaleSignal(double* inputSignal, int numSamples, float scalingFactor)
{
    //Goes through the array of all the samples.
    for (int i = 0; i < numSamples; i++)
    {
        inputSignal[i] = inputSignal[i] * scalingFactor;
    }


};

//signal processing: reverse signal
void reverseSignal(double* inputSignal, int numSamples)
{
    //Goes through the array of all the samples.
    for (int i = 0; i < numSamples / 2; i++)
    {
        //Creates a temporary array.
        float temp = inputSignal[i];
        //Tells the code to take a value of one of the samples and pace it at the end of the new temporary array.
        inputSignal[i] = inputSignal[(numSamples - 1) - i];
        //Changes the original array to the temporary array.
        inputSignal[(numSamples - 1) - i] = temp;
    }


}

//signal processing: hard clip distorts the audio file. Threshold is determined by a value between 0 and 1
void distortion(double* inputSignal, int numSamples, double threshold) {
    //Goes through the array of all the samples.
    for (int i = 0; i < numSamples; i++)
    {
        //Sets any value above the threshold to the same value as the threshold
        if (inputSignal[i] > threshold) {
        inputSignal[i] = threshold;
        }
        //Does the same, but to the negative values.
        else if (inputSignal[i] < -threshold) {
            inputSignal[i] = -threshold;
        }

    }
}
