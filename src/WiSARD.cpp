/*
 * Discriminator.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/WiSARD.hpp"
#include "../include/Discriminator.hpp"
#include "../include/Util.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <string>


using namespace std;
using namespace wann;

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
		seed = chrono::system_clock::now().time_since_epoch().count();
		shuffle(begin(memoryAddressMapping), end(memoryAddressMapping), default_random_engine(seed));
	}
}

WiSARD::~WiSARD(void)
{
	for (auto it = discriminators.begin(); it != discriminators.end(); ++it )
	{
		delete it->second;
	}
}
// Discriminator * WiSARD::getDiscriminator(string label)
// {
// 	return discriminators[label];
// }

// void WiSARD::createDiscriminator(string discriminatorLabel)
// {
// 	discriminators[discriminatorLabel] = new Discriminator(retinaLength, numBitsAddr, memoryAddressMapping, isCummulative, ignoreZeroAddr);
// }


void WiSARD::fit(const vector< vector<int> > &X, const vector<string> &y)
{

	// get unique values of each label
	vector<string> labels(y);
	sort(labels.begin(), labels.end());
	labels.erase(unique(label.begin(), label.end()), label.end() );



	for(int i=0; i < y.size(); i++)
	{
		string label = y[i];
		discriminators[label]->addTrainning(X[i]);
	}	
}

unordered_map<string, int> WiSARD::predict(const vector<int> &retina)
{
	unordered_map<string, int> result;
	unordered_map<string, vector<int>> memoryResult;
	string label;
	Discriminator * auxDisc;
	vector<int> memoryResultAux;
	
	for (auto it = discriminators.begin(); it != discriminators.end(); ++it )
	{
		label = it->first;
		auxDisc = it->second;
		memoryResultAux = auxDisc->predict(retina);
	
		int sumMemoriesValue = 0;
		for(int i = 0; i < memoryResultAux.size(); i++)
		{
			if(memoryResultAux[i] > 0)
				sumMemoriesValue += 1;
		}
		result[label] = sumMemoriesValue;
		memoryResult[label] = memoryResultAux;
	}

	if( ! useBleaching)
	{
		return result;
	}
	else
	{
		//apply bleaching
		float confidence = util::calculateConfidence(result);
		int b = defaultBleaching_b;

		while(confidence < confidenceThreshold)
		{
			for (auto it = result.begin(); it != result.end(); ++it )
			{
				label = it->first;
				int sumMemoriesValue = 0;

				const std::vector<int> &labelResult = memoryResult[label];
				for(int i = 0; i < labelResult.size(); i++)
				{
					if(labelResult[i] > b)
						sumMemoriesValue += 1;
				}
				

				result[label] = sumMemoriesValue;
			}
			
			b+=defaultBleaching_b;
			confidence = util::calculateConfidence(result);
		}
		
		return result;
	}

}
