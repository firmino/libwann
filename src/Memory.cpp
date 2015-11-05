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
	numAddrs = (long) pow(2, numBits);
}

Memory::~Memory(void)
{
	data.clear();
}

long Memory::getMemorySize(void)
{
	return numAddrs;
}

void Memory::addValue(const long addr, int value = 1)
{	
	if(addr < 0L || addr >= numAddrs)
	{
		cout << "invalid address to add value" << endl;
		cout << "number of address: " << numAddrs << endl;
		exit(-1);
	}
	if(!isCummulative)
	{
		data[addr] = 1;
	}
	else
	{
		if(!data.count(addr))
		{
			data[addr] = value;
		}
		else
		{
			data[addr] += value;
		}
	}
	
}
int Memory::getValue(long addr)
{
	if(addr < 0 || addr >= numAddrs)
	{
		cout << "invalid address to get value" << endl;
		cout << "address: "<< addr << endl;
		exit(-1);
	}
	if(ignoreZeroAddr && addr == 0)
		return 0;
	if(!data.count(addr))
		return 0;

	return data[addr];
}




