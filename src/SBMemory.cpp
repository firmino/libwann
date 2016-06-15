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
using namespace wann;

Memory::Memory(int numBits,
			   int maxMemoryValue,
			   bool isCummulative=true, 
			   bool ignoreZeroAddr=false)
:numBits(numBits),isCummulative(isCummulative),ignoreZeroAddr(ignoreZeroAddr)
{
	if(numBits > 62)
		cout << "WARNING: Representation overflow due to number of bits" << endl;

	numAddrs = (long long)pow((long long)2, (long long)numBits);
	
}

Memory::~Memory(void)
{
	data.clear();
}

void Memory::addOnlineValue(const long long addr)
{	
	int value = maxMemoryValue;
	if(addr < 0L || addr >= numAddrs)
	{
		cout << "WARNING: invalid address to add value" << endl;
		cout << "WARNING: number of address: " << numAddrs << endl;
		exit(-1);
	}

	data[addr] = maxMemoryValue;
}

void Memory::addValue(const long long addr, value = 1)
{	
	if(addr < 0L || addr >= numAddrs)
	{
		cout << "WARNING: invalid address to add value" << endl;
		cout << "WARNING: number of address: " << numAddrs << endl;
		exit(-1);
	}

	if(!data.count(addr))
	{
		data[addr] = value;
	}
	else
	{
		data[addr] += value;
	}
}

int Memory::getValue(const long long addr)
{
	if(addr < 0 || addr >= numAddrs)
	{
		cout << "WARNING: invalid address to add value" << endl;
		cout << "WARNING: number of address: " << numAddrs << endl;
		exit(-1);
	}
	if(ignoreZeroAddr && addr == 0)
		return 0;
	if(!data.count(addr))
		return 0;

	return data[addr];
}

void Memory::decrease()
{
	for(auto it = data.begin(); it != data.end(); ++it)
	{
		it->second -= 1;
		if(it->second < 0)
		{
			it->second = 0;
		}
	}
}