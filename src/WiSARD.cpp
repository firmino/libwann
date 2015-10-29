/*
 * Discriminator.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/WiSARD.hpp"
#include "../include/Discriminator.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <string>


using namespace std;


WiSARD::WiSARD(int retinaLength, 
			   int numBitsAddr, 
			   bool useBleaching, 
			   float confidenceThreshold, 
			   int defaultBleaching_b, 
			   bool randomizePositions, 
			   bool isCummulative, 
			   bool ignoreZeroAddr)

:retinaLength(retinaLength), 
 numBitsAddr(numBitsAddr),
 useBleaching(useBleaching),
 confidenceThreshold(confidenceThreshold),
 defaultBleaching_b(defaultBleaching_b),
 randomizePositions(randomizePositions),
 isCummulative(isCummulative),
 ignoreZeroAddr(ignoreZeroAddr)
{
	
	for(int i=0; i < retinaLength; i++)
		memoryAddressMapping.push_back(i);

	if(randomizePositions)
	{	
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		shuffle(begin(memoryAddressMapping), end(memoryAddressMapping), default_random_engine(seed));
	}
}

WiSARD::~WiSARD(void)
{}



void WiSARD::createDiscriminator(string discriminatorLabel)
{
	Discriminator disc(retinaLength, numBitsAddr, memoryAddressMapping, isCummulative, ignoreZeroAddr);
	discriminators[discriminatorLabel] = &disc;
}


void WiSARD::fit(const vector< vector<int> > &X, const vector<string> &y)
{
	for(int i=0; i < y.size(); i++)
	{
		string label = y[i];
		discriminators[label]->addTrainning(X[i]);
	}	
}

unordered_map<string, int> WiSARD::predict(const vector<int> &retina)
{
	unordered_map<string, int> result;
	unordered_map<string, vector<int> *> memoryResult;

	for (auto it = discriminators.begin(); it != discriminators.end(); ++it )
	{
		string label = it->first;

		Discriminator *d = it->second;

		vector<int> resultDisc = d->predict(retina);
		
		int sumMemoriesValue = 0;
		for(int i = 0; i < resultDisc.size(); i++)
		{
			if(resultDisc[i] > 0)
				sumMemoriesValue += 1;
		}

		result[label] = sumMemoriesValue;
		memoryResult[label] = &resultDisc;
	}

	if( ! useBleaching)
	{
		return result;
	}
	else
	{
		//apply bleaching
		float confidence = calculateConfidence(result);
		int b = defaultBleaching_b;

		while(confidence < confidenceThreshold)
		{

			for (auto it = result.begin(); it != result.end(); ++it )
			{
				string label = it->first;

				vector<int> memRes =  *memoryResult[label];

				int sumMemoriesValue = 0;
				for(int i = 0; i < memRes.size(); i++)
				{
					if(memRes[i] > b)
						sumMemoriesValue += 1;
				}

				result[label] = sumMemoriesValue;
			}

			b++;
			confidence = calculateConfidence(result);
		}

		return result;	
	}

}

float WiSARD::calculateConfidence(unordered_map<string, int> &result)
{
	int max = 0;
	int secondMax = 0;

	for (auto it = result.begin(); it != result.end(); ++it )
	{
		int value = it->second;

		if (value > max)
		{
			secondMax = max;
			max = value;
		}	
		if(value < max && value > secondMax )
			secondMax = value;
	}
	
	float confidence = 1.0 - ( ( (float)secondMax )/max);
	return confidence;
}


