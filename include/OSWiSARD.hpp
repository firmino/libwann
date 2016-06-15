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


namespace wann
{

	class WiSARD
	{
		public:
			WiSARD(int numBitsAddr,
				   int maxMemoryValue,
				   float confidenceThreshold,
				   bool randomizePositions, 
				   bool ignoreZeroAddr);

			~WiSARD(void);

			void fit(const std::vector< std::vector<int> > &, const std::vector<std::string> &);
			std::vector<std::string> predict(const std::vector< std::vector<int> > &);
			void onFit(const vector<int> X, const y);
			std::vector<std::unordered_map<std::string, float>> predictProba(const std::vector<std::vector<int> > &);
			
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
			std::unordered_map <std::string, Discriminator*> discriminators;
			std::vector<int> memoryAddressMapping;

			std::unordered_map <std::string,float> applyBleaching(std::unordered_map<std::string,float> &,
				std::unordered_map<std::string, std::vector<int>> &);


	};
}

#endif /* WISARD_HPP_ */
