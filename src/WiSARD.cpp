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

using namespace wann;
using namespace std;

WiSARD::WiSARD(int numBitsAddr,
			   bool useBleaching,
			   float confidenceThreshold,
			   int defaultBleaching_b,
			   bool randomizePositions,
			   bool isCummulative,
			   bool ignoreZeroAddr,
			   int onlineMax)

:numBitsAddr(numBitsAddr),
 useBleaching(useBleaching),
 confidenceThreshold(confidenceThreshold),
 defaultBleaching_b(defaultBleaching_b),
 randomizePositions(randomizePositions),
 isCummulative(isCummulative),
 ignoreZeroAddr(ignoreZeroAddr),
 onlineMax(onlineMax)
{

}

WiSARD::~WiSARD(void)
{
	for (auto it = discriminators.begin(); it != discriminators.end(); ++it )
	{
		delete it->second;
	}
}


void WiSARD::fit(const vector< vector<int> > &X, const vector<string> &y)
{
	std::cout << retinaLength << '\n';

	if(retinaLength == 0)
	{
		retinaLength = X[0].size();

		for(int i=0; i < retinaLength; i++)
		memoryAddressMapping.push_back(i);

		if(randomizePositions)
		{
			seed = chrono::system_clock::now().time_since_epoch().count();
			shuffle(begin(memoryAddressMapping), end(memoryAddressMapping), default_random_engine(seed));
		}
		// get unique labels
		vector<string> labels;
		unordered_map <string, int> auxMap ;
		for(int i = 0; i < y.size(); i++)
		{
			auxMap[y[i]] = 0;
		}
		for(auto it = auxMap.begin(); it != auxMap.end(); ++it)
		{
			labels.push_back(it->first);
		}

		//creating discriminators
		for(int i = 0; i < labels.size(); i++ )
		{
			string label = labels[i];

			Discriminator *d = new Discriminator(retinaLength,
												 numBitsAddr,
												 memoryAddressMapping,
												 isCummulative,
												 ignoreZeroAddr,
												 onlineMax);
			discriminators[label] = d;
		}
	}




	//training discriminators
	for(int i=0; i < y.size(); i++)
	{
		string label = y[i];
		discriminators[label]->addTrainning(X[i]);
	}
}

void WiSARD::onFit(const vector< vector<int> > &X, const vector<string> &y)
{
	if(retinaLength == 0)
	{
		retinaLength = X[0].size();

		for(int i=0; i < retinaLength; i++)
		memoryAddressMapping.push_back(i);

		if(randomizePositions)
		{
			seed = chrono::system_clock::now().time_since_epoch().count();
			shuffle(begin(memoryAddressMapping), end(memoryAddressMapping), default_random_engine(seed));
		}

		// get unique labels
		vector<string> labels;
		unordered_map <string, int> auxMap ;
		for(int i = 0; i < y.size(); i++)
		{
			auxMap[y[i]] = 0;
		}
		for(auto it = auxMap.begin(); it != auxMap.end(); ++it)
		{
			labels.push_back(it->first);
		}


		//creating discriminators
		for(int i = 0; i < labels.size(); i++ )
		{
			string label = labels[i];

			Discriminator *d = new Discriminator(retinaLength,
												 numBitsAddr,
												 memoryAddressMapping,
												 isCummulative,
												 ignoreZeroAddr,
												 onlineMax);
			discriminators[label] = d;
		}
	}
	retinaLength = X[0].size();

	//training discriminators
	for(int i=0; i < y.size(); i++)
	{
		string label = y[i];
		discriminators[label]->addOnlineTrainning(X[i]);
	}
}


vector<unordered_map<string, float>> WiSARD::predictProba(const vector< vector<int> > &X)
{
	vector<unordered_map<string, float>> results;
	int numMemories =  (int) ceil( (float)retinaLength/ (float) numBitsAddr );

	//for each retina
	for(int i=0; i < X.size(); i++)
	{
		string label;

		unordered_map<string, float> result;
		unordered_map<string, vector<int>> memoryResult;

		Discriminator *auxDisc;
		vector<int> memoryResultAux;

		//getting a retina
		vector<int> retina = X[i];

		// for each discriminator
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

			// to calc probability, what percentage of memories recognize the element;
			result[label] = (float)sumMemoriesValue / (float)numMemories;
			memoryResult[label] = memoryResultAux;
		}


		if(useBleaching)
			result = WiSARD::applyBleaching(result, memoryResult);

		results.push_back(result);
	}

	return results;
}


vector<string> WiSARD::predict(const vector< vector<int> > &X)
{
	vector<string> vecRes;
	vector<unordered_map<string, float>>  results = WiSARD::predictProba(X);

	for(int i=0; i< results.size(); i++)
	{
		unordered_map<string, float> result = results[i];
		string resultLabel = util::argMax(result);


		vecRes.push_back(resultLabel);
	}

	return vecRes;
}

unordered_map<string, float> WiSARD::applyBleaching(unordered_map<string, float> &result,  unordered_map<string, vector<int>> &memoryResult)
{
	unordered_map<string, float> resultFinal = result;

	//apply bleaching
	vector<int> labelResult;
	string label;
	int b = defaultBleaching_b;
	int numMemories = (int) ceil( (float)retinaLength/ (float) numBitsAddr );


	float confidence = util::calculateConfidence(resultFinal);

	while(confidence < confidenceThreshold)
	{

		for(auto it = resultFinal.begin(); it != resultFinal.end(); ++it )
		{
			label = it->first;
			int sumMemoriesValue = 0;

			labelResult = memoryResult[label];

			for(int i = 0; i < labelResult.size(); i++)
			{
				if(labelResult[i] > b)
					sumMemoriesValue += 1;
			}

			resultFinal[label] = ((float) sumMemoriesValue / (float) numMemories);
		}

		// if no memory recognize the pattern, return previous value
		float maxValue = util::maxValue(resultFinal);

		if(maxValue <= 0.000001)  // if is zero
		{
			resultFinal = result;
			break;
		}

		b ++;
		confidence = util::calculateConfidence(resultFinal);
	}

	return resultFinal;
}
