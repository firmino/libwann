/*
 * WiSARD.hpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#ifndef WISARD_HPP_
#define WISARD_HPP_

#include "./Discriminator.hpp"

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class WiSARD
{
	public:
		WiSARD(int retinaLength, 
			   int numBitsAddr, 
			   bool useBleaching=true, 
			   float confidenceThreshold=0.1, 
			   int defaultBleaching_b=1, 
			   bool randomizePositions=true, 
			   bool isCummulative=true, 
			   bool ignoreZeroAddr=false);

		~WiSARD(void);

		void createDiscriminator(string);
		void fit(const vector< vector<int> > &X, const vector<string> &y);
		unordered_map<string, int> predict(const vector<int> &retina);
		float calculateConfidence(unordered_map<string, int> &);
		Discriminator * getDiscriminator(string label);

	private:
		int retinaLength;
		int numBitsAddr;
		bool useBleaching;
		float confidenceThreshold;
		int defaultBleaching_b;
		bool randomizePositions;
		bool isCummulative;
		bool ignoreZeroAddr;
		unsigned seed;
		unordered_map <string, Discriminator*> discriminators;
		vector<int> memoryAddressMapping;

};


#endif /* WISARD_HPP_ */
