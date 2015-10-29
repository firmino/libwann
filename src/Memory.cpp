/*
 * Memory.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/Memory.hpp"
#include <math.h>


using namespace std;

Memory::Memory(int numBits, bool isCummulative=true, bool ignoreZeroAddr=false)
:numBits(numBits),isCummulative(isCummulative),ignoreZeroAddr(ignoreZeroAddr)
{
	numAddrs = pow(2, numBits);

	for(int i=0; i < numAddrs; i++)
		data[i] = 0;
}

Memory::~Memory(void)
{}

int Memory::getMemorySize(void)
{
	return numAddrs;
}

void Memory::addValue(int addr, int value=1)
{
	data[addr] = value;
}

int Memory::getValue(int addr)
{
	if(ignoreZeroAddr && addr == 0)
		return 0;

	return data[addr];
}




