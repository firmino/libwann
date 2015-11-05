/*
 * Discriminator.hpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#ifndef DISCRIMINATOR_HPP_
#define DISCRIMINATOR_HPP_

#include "./Memory.hpp"

#include <vector>

using namespace std;

class Discriminator
{
	public:
		Discriminator(int retinaLength, 
					  int numBits, 
					  vector<int> memoryAddressMapping, 
					  bool isCummulative = true, 
					  bool ignoreZeroAddr = false);

		~Discriminator(void);
		
		int getNumMemories(void);
		vector<Memory *> getMemories(void);
		vector<int> getMapping(int addr);
		Memory * getMemory(int addr);
		void addTrainning(const vector<int> &retina);
		vector<int> predict(const vector<int> &retina);

	private:
		int retinaLength;
		int numBitsAddr;
		int numMemories;
		bool isCummulative;
		bool ignoreZeroAddr;
		vector<Memory *> memories;
		vector<int> memoryAddressMapping;

};


#endif /* DISCRIMINATOR_HPP_ */
