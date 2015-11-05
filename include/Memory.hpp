/*
 * Memory.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */


#ifndef MEMORY_CPP_
#define MEMORY_CPP_

#include <string>
#include <unordered_map>

using namespace std;

class Memory
{
	public:
		Memory(int numBits, bool isCummulative, bool ignoreZeroAddr);
		
		~Memory(void);
		
		long getMemorySize(void);
		void addValue(const long addr, int value);
		int getValue(long addr);

	private:
		unordered_map<long,int> data;
		long numAddrs;
		int numBits;
		bool isCummulative;
		bool ignoreZeroAddr;
};

#endif /* MEMORY_CPP_ */
