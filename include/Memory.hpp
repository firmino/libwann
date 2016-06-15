/*
 * Memory.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */


#ifndef MEMORY_CPP_
#define MEMORY_CPP_

#include <unordered_map>


namespace wann
{
	class Memory
	{
		public:
			Memory(int numBits, bool isCummulative, bool ignoreZeroAddr, int onlineMax);
			~Memory(void);
			
			void addValue(const long long addr, int value);
			void addOnlineValue(const long long addr);
			int getValue(const long long addr);
			void decrease();

		private:
			std::unordered_map<long long, int> data;
			long long numAddrs;
			int numBits;
			bool isCummulative;
			bool ignoreZeroAddr;
			int onlineMax;
	};

}

#endif /* MEMORY_CPP_ */
