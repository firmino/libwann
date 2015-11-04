/*
 * Memory.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/Memory.hpp"
#include <math.h>
#include <iostream>
#include <unordered_map>

using namespace std;

Memory::Memory(int numBits, bool isCummulative=true, bool ignoreZeroAddr=false)
:numBits(numBits),isCummulative(isCummulative),ignoreZeroAddr(ignoreZeroAddr)
{
	numAddrs = pow(2, numBits);
	/*
	for(int i=0; i < numAddrs; i++)
	{			
		cout << i << "\n";
		data[i] = 0;
	}
	*/
}

Memory::~Memory(void)
{}

int Memory::getMemorySize(void)
{
	return numAddrs;
}

void Memory::addValue(long addr, int value=1)
{
	
	if(!data.count(addr))
	{
		
		data[addr] = 1;
	}
	else
	{
		
		data[addr] += value;
	}
	
}

int Memory::getValue(long addr)
{
	if(ignoreZeroAddr && addr == 0)
		return 0;
	if(!data.count(addr))
		return 0;

	return data[addr];
}




