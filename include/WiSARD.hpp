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
				   bool useBleaching=true,
				   float confidenceThreshold=0.1,
				   int defaultBleaching_b=1,
				   bool randomizePositions=true,
				   bool isCummulative=true,
				   bool ignoreZeroAddr=false,
				   int onlineMax = 2);

			~WiSARD(void);

			void fit(const std::vector< std::vector<int> > &, const std::vector<std::string> &);
			void onFit(const std::vector< std::vector<int> > &, const std::vector<std::string> &);
			std::vector<std::string> predict(const std::vector< std::vector<int> > &);
			std::vector<std::unordered_map<std::string, float>> predictProba(const std::vector<std::vector<int> > &);

		// private:
			int retinaLength = 0;
			int onlineMax;
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
