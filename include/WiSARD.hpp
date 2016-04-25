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

namespace wann
{

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

			void fit(const vector< vector<int> > &, const vector<string> &);
			vector<string> predict(const vector< vector<int> > &);
			vector<unordered_map<string, float>> predictProba(const vector< vector<int> > &);
			
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

			void applyBleaching(unordered_map<string, float> &, const vector<int> &);

	};
}

#endif /* WISARD_HPP_ */
