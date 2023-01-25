
#include "stdafx.h"
#include <iostream>
#include "audioIO.h"
#include "ms_getenv.h"
#include <iomanip>
#include <string>

using namespace std;

//Code begins here.
int main()
{
	

	//Declares the file path as a string, which will be called back to in when defining "fullPath" as a string.
	//Also decleares fileLocation so that the output code has a definition of the string, preventing issues.
	//This is defined in the user input stage.
	string fileLocation;

	//Tells the code to look to the user's name as the beginning of the full path.
	//Also defines homeDir as the users directory.
	string homeDir = ms_getenv("USERPROFILE");

	//Defines inputSignal as NULL so that the while loop will run.
	double* inputSignal = NULL;

	// Introduces the user to the Code.
	cout << "Welcome to Robert Cardoso's Real-Time Audio Processor." << endl;

	//The length, samplerate and number of channels are declared as integers here.
	int numSamples, fs, numChannels;

	//While loop enabling the user to enter the file location and name.
	//If either is invalid, the user will be prompted to enter it again.
	while (inputSignal == NULL) {
		//Prompts user to enter file location
		cout << "Please enter your full file location: ";
		getline(cin, fileLocation);
		//Prompts user to enter file name.
		cout << "Please enter your audio file name: ";
		//Prompts user to define filename with their input.
		string filename;
		getline(cin, filename);
		//Takes the information about the file's location and name, and uses it to define fullPath to be used in the audioread function.
		string fullPath = homeDir + "/" + fileLocation + "/" + filename;
		//Audioread() returns a pointer to an array of floating point audio samples
		//In a nutshell:Reads audio
		inputSignal = audioread(numSamples, fs, numChannels, fullPath);
		//Runs this code if the input signal is valid.
		//This will happen provided the information entered by the user is valid.
		if (inputSignal != NULL) {
			//prints out the file name, sample rate, length (in seconds) and number of channels.
			cout << "File: " << filename.c_str()
				<< "\nSamplerate: " << fs
				<< "\nLength: " << (float)numSamples / fs << "s"
				<< "\nChannels: " << numChannels << endl;
		}
		//Runs this code if the input is not valid.
		//This will happen if the information entered by the user is invalid.
		else {
			//lets user know that the information is not valid and the file could not be found.
			//The code will start again, prompting the user to enter a valid file location and name over and over again until it is correct.
			cout << "Error: file not found, please try again." << endl;
		}
	}
	
	
	//Declares "sampleAnswer" as a character. This will be defined in a Yes or No question. 
	char sampleAnswer;
	//Declares the number of values to print out based on whether or not the user wishes to print them out.
	//If they choose to, they will be prompted to change this value.
	//If not, the code will print out 0 values and move on.
	int numValuesToPrintOut = 0;
	//Sets a value for "numValuesToPrintOut" to compare to, essentially creating a failsafe to prevent the user from requesting less than 0 samples.
	int noSamples = 0;
	
	//This is a while loop that will continue to loop until broken by a valid answer to the Yes or No question.
	while (1)
		{
		//Prompts user to decide whether or not they wish to print out some sample values with a Yes or No Question.
			cout << "Would you like to check the values of a number of samples? (Total Samples: " << numSamples << ") y/n \n" << endl;
			//Takes the answer and uses it as a validation to continue with the loop.
			cin >> setw(1) >> sampleAnswer;
			cout << endl;

			//If the user enters a "y" or "Y", then it is presumed as a "yes" and the function within this if statement will continue.
			if ((sampleAnswer == 'y' || sampleAnswer == 'Y'))
			{
				//This is a while loop that will continue to loop until broken by a valid number of samples requested.
				while (2)
				{
					//Prompts user to enter a value for the number of samples requested.
					cout << "Please insert the number of samples you wish to print: ";
					//Takes user's input and uses it as a value for the number of samples to be printed.
					cin >> numValuesToPrintOut;
					//Failsafe to prevent code from continuing when user enters an invalid value or a number larger than the total number of samples.
					//If the new value is still invalid, the user will be prompted again to enter a valid value.
					while (!cin.good() || (numValuesToPrintOut > numSamples))
					{
						cin.clear();
						cin.ignore(INT_MAX, '\n');
						//Prompts the user to again to enter a valid value.
						cout << "Please enter a valid numerical value (Less than " << numSamples << "): \n" << endl;
						//Takes the value entered and uses it to define the integer "numValuesToPrintOut". 
						cin >> numValuesToPrintOut;
						
					}
					//Condition that allows the code to run if "numValuesToPrintOut" is within a valid range (between no samples and the maximum number of samples.
					if (numValuesToPrintOut <= numSamples && numValuesToPrintOut > noSamples)
					{
						//function to print out the requested number of samples.
						printSamples(inputSignal, numValuesToPrintOut);
						cout << endl;
						break;
					}
					//Condition that allows the code to run if the user enters a value less than or equal to 0.
					if (numValuesToPrintOut <= noSamples)
					{
						//Lets user know that no samples will be printed out. Skips the function entirely.
						cout << "You have requested no samples. \n" << endl;
						break;
					}
					
				}
				break;
			}
			// Skips the function if the user inputs a "No" answer to the Question, "Would you like to check the values of a number of samples".
			if ((sampleAnswer == 'n' || sampleAnswer == 'N'))
			{
				break;
			}
			
			
			//Failsafe that ignores any values other than those that represent Yes or No.
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			//Reminds user that the question must be answered with either Yes or No.
			cout << "Please answer with either \"y\" or \"n\"." << endl;
			
			
			
		}

	//Declares "scalingAnswer" as a character. This will be defined in a Yes or No question.
	char scalingAnswer;		
	//Declares the scaling factor as 1, which is the maximum. If the function used to change the volume of the signal is skipped, this will remain at 1.
	//If the user decides to change the volume, this will change.
	float scalingFactor = 1;
	//Sets values for the scaling factor to compare to. Will be called back to in different contexts when the user choses a scaling factor.
	float minVolume = 0;
	float maxVolume = 1;

	//This is a while loop that will continue to loop until broken by a valid answer to the Yes or No question.
	while (1)

	{
		//Prompts the user to decide if they want to change the volume.
		cout << "Would you like to change the volume of the audio file? y/n \n" << endl;
		//Takes the answer and uses it as validation whether to continue the loop or not.
		cin >> scalingAnswer;
		cout << endl;
		//Runs this code if the answer is Yes.
		if (scalingAnswer == 'y' || scalingAnswer == 'Y')
		{
			//This is a while loop that will continue until the user enters a valid answer.
			while (2)
			{
				//Prompts the user to enter the scaling factor.
				cout << "Enter a value between 0 and 1 for the volume: ";
				//Takes the answer and uses it as the scaling factor.
				cin >> scalingFactor;
				//Failsafe to prevent the user from entering a value that is not a numerical value. Ignores any incorrect inputs.
				//If user continues to enter an invalid value, they will continue to be asked to enter a valid value.
				while (!cin.good())
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					//Prompts user to enter a valid numerical value.
					cout << "Please enter a valid numerical value between 0 and 1: \n" << endl;
					//Takes the user's input and uses it as the scaling factor.
					cin >> scalingFactor;
				}
				//Runs code if the user enters a valid value and uses their input as the scaling factor.
				if (scalingFactor > minVolume && scalingFactor < maxVolume)
				{
					//Scales the signal using the scaling factor as a multiplier.
					scaleSignal(inputSignal, numSamples, scalingFactor);
					break;
				}
				//Runs code if user enters a value less than 0 (the minimum value).
				if (scalingFactor <= minVolume)
				{
					//sets the value to 0.
					scalingFactor = minVolume;
					//runs code with 0 as the scaling factor.
					scaleSignal(inputSignal, numSamples, scalingFactor);
					cout << "volume set to min \n" << endl;
					break;
				}
				//Runs code if user enters a value more than 1 (the maximum value).
				if (scalingFactor >= maxVolume)
				{
					//sets the value to 1.
					scalingFactor = maxVolume;
					//runs the code with 1 as the scaling factor.
					scaleSignal(inputSignal, numSamples, scalingFactor);
					cout << "volume set to max \n" << endl;
					break;
				}
			} 
			break;			
		}//Runs this code if the answer is No.
		if (scalingAnswer == 'n' || scalingAnswer == 'N')
		{
			//Ignores the loop and skips over the "scaleSignal" function entirely.
			break;
		}

		//Failsafe that ignores any values other than those that represent Yes or No.
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		//Reminds user that the question must be answered with either Yes or No.
		cout << "Please answer with either \"y\" or \"n\"." << endl;
	
	}


	//Declares "reverseAnswer" as a character. This will be defined in a Yes or No question.
	char reverseAnswer;
	//This is a while loop that will continue to loop until broken by a valid answer to the Yes or No question.
	while (1)
	{
		//Asks the user if they wish to reverse the audio signal.
		cout << "Would you like to reverse the signal? y/n \n" << endl;
		//Takes the user's answer and uses it as validation whether to continue the loop or not.
		cin >> reverseAnswer;
		cout << endl;
		//Runs this code if the answer is yes.
		if (reverseAnswer == 'y' || reverseAnswer == 'Y')
		{
			//Reverses the audio signal.
			reverseSignal(inputSignal, numSamples);
			break;
		}
		//Runs this code if the answer is no.
		if (reverseAnswer == 'n' || reverseAnswer == 'N')
		{
			//Breaks the loop and ignores the "revereSignal" function entirely.
			break;
		}
		//Failsafe that ignores any values other than those that represent Yes or No.
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		//Reminds user that the question must be answered with either Yes or No.
		cout << "Please answer with either \"y\" or \"n\"." << endl;
		
	}
	
	//Declares "distortionAnswer" as a character. This will be defined in a Yes or No question.
	char distortionAnswer;
	//Declares the threshold as 1, which is the maximum. If the function used to change the threshold is skipped, this will remain at 1.
	//If the user decides to change the threshold, this will change.
	double threshold = 1;
	//Sets values for the scaling factor to compare to. Will be called back to in different contexts when the user choses a scaling factor.
	double minThreshold = 0;
	double maxThreshold = 1;

	//This is a while loop that will continue to loop until broken by a valid answer to the Yes or No question.
	while (1) {
		//Asks the user whether or not they wish to distort the signal.
		cout << "Would you like to apply clipping distortion? y/n \n" << endl;
		//Takes the user's answer and uses it as validation whether to continue the loop or not.
		cin >> distortionAnswer;
		cout << endl;
		//Runs this function if the answer is yes.
		if (distortionAnswer == 'y' || distortionAnswer == 'Y')
			//This is a while loop that will continue until the user enters a valid answer.
			while (2)
			{
				//Prompts the user to enter a value for the threshold.
				cout << "Enter a value between 0 and 1 for the threshold (0 = max clipping, 1 = no clipping): ";
				//Takes the user's input and uses it as the threshold.
				cin >> threshold;
				//Failsafe to prevent the user from entering a value that is not a numerical value. Ignores any incorrect inputs.
				//If user continues to enter an invalid value, they will continue to be asked to enter a valid value.
				while (!cin.good())
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					//Prompts the user to enter a valid numerical value.
					cout << "Please enter a valid numerical value between 0 and 1: \n" << endl;
					//Takes the user's input and uses it as the threshold.
					cin >> threshold;
				}
				//Runs this code if the answer is within the valid numerical range
				if (threshold > minThreshold && threshold < maxThreshold)
				{
					//Applies clipping distortion based on the threshold set by the user.
					distortion(inputSignal, numSamples, threshold);
					break;
				}
				//Runs this code if the answer is more than or equal to the maximum threshold (1).
				if (threshold >= maxThreshold)
				{
					//Sets the threshold to the maximum threshold (1).
					threshold = maxThreshold;
					//Applies clipping distortion with a threshold of 1.
					distortion(inputSignal, numSamples, threshold);
					cout << "Threshold set to 1. No clipping applied. \n" << endl;
					break;
				}
				//Runs this code if the answer is less than or equal to the minimum threshold. (0)
				if (threshold <= minThreshold)
				{
					//Sets the threshold to the minimum threshold (0).
					threshold = minThreshold;
					//Applies clipping distortion with a threshold of 0.
					distortion(inputSignal, numSamples, threshold);
					cout << "Threshold set to 0. Maximum clipping applied. \n" << endl;
					break;
				}
				
			} break;

		//Runs this code if the answer is no.
		if (distortionAnswer == 'n' || distortionAnswer == 'N')
		{
			break;
		}
		//Failsafe that ignores any values other than those that represent Yes or No.
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		//Reminds user that the question must be answered with either Yes or No.
		cout << "Please answer with either \"y\" or \"n\"." << endl;
	}

	cout << endl;
	

	//Outputs the processed audio, determining where to put it and what to name it.
	string filename2 = "processedAudio.wav";
	string fullPathOutput = homeDir + "/" + fileLocation + "/" + filename2;
	audiowrite(inputSignal, numSamples, fs, fullPathOutput);
	cout << "Your new file \"" << filename2 << "\" has been created. \n" << endl;
	cout << "Check \"/" << fileLocation << "/\" for your new file!" << endl;
	

	//important: Make sure your free up the allocated memory from earlier
	delete[] inputSignal;

	cout << "Thank you for using my Real-Time Audio Processing Code! :)" << endl;

	return 0;
}
