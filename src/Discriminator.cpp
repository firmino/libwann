/*
 * Discriminator.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/Discriminator.hpp"
#include "../include/Memory.hpp"
#include <cmath>
#include <iostream>


using namespace std;

Discriminator::Discriminator(int retinaLength, 
                             int numBits,
                             vector<int> memoryAddressMapping, 
                             bool isCummulative, 
                             bool ignoreZeroAddr)
: retinaLength(retinaLength),
  numBitsAddr(numBits),
  memoryAddressMapping(memoryAddressMapping),
  isCummulative(isCummulative),
  ignoreZeroAddr(ignoreZeroAddr)
{
    numMemories = (int) ceil(((float)retinaLength)/numBits);
    bool hasRestMemory = ((retinaLength % numBits) > 0) ? true : false;


    if(! hasRestMemory)
    {
        for(int i=0; i<numMemories; i++)
            memories.push_back(Memory(numBits, isCummulative, ignoreZeroAddr));
    }
    else
    {
        for(int i=0; i<numMemories - 1; i++)
            memories.push_back(Memory(numBits, isCummulative, ignoreZeroAddr));

        //the rest memory 
        memories.push_back(Memory((retinaLength % numBits), isCummulative, ignoreZeroAddr));
    } 
}

Discriminator::~Discriminator(void)
{}

int Discriminator::getNumMemories(void)
{
    return numMemories;
}

vector<Memory> Discriminator::getMemories()
{
    return memories;
}

vector<int> Discriminator::getMapping(int addr)
{
    return memoryAddressMapping;
}

Memory Discriminator::getMemory(int addr)
{
    return memories[addr];
}

void Discriminator::addTrainning(const vector<int> &retina)
{
 
    // each group of numBitsAddr is related with a memory
    for(int i=0; i < retinaLength; i+= numBitsAddr)
    {
        int addr = 0;

        for(int j=0; j<numBitsAddr; j++)
        {
            addr += pow(2,j) * retina[i+j];
        }       

        int memIndex = i / numBitsAddr;

        memories[memIndex].addValue(addr, 1);
    }
 
    //  the rest of the retina (when the retina length is not a multiple of number of bits of address)
    int restOfPositions = retinaLength % numBitsAddr;
    int addr = 0;
    for(int j=0; j< numBitsAddr; j++)
    {
        addr += pow(2,j) * retina[(retinaLength - restOfPositions)+j];
    }        
    int lastMemoryPosition = ceil(retinaLength/numBitsAddr);
    memories[lastMemoryPosition].addValue(addr, 1);
}


vector<int> Discriminator::predict(const vector<int> &retina)
{
    vector<int> result(numMemories);

    // each group of numBitsAddr is related with a memory
    for(int i=0; i < retinaLength; i+= numBitsAddr)
    {
        int addr = 0;

        for(int j=0; j<numBitsAddr; j++)
        {
            addr += pow(2,j) * retina[i+j];
        }       

        int memIndex = i / numBitsAddr;

        result.push_back( memories[memIndex].getValue(addr) );
    }

    //  the rest of the retina (when the retina length is not a multiple of number of bits of address)   
    int restOfPositions = retinaLength % numBitsAddr;
    int addr = 0;
    
    for(int j=0; j< numBitsAddr; j++)
    {
        addr += pow(2,j) * retina[(retinaLength - restOfPositions)+j];
    }        

    int lastMemoryPosition = ceil(retinaLength/numBitsAddr);
    result.push_back( memories[lastMemoryPosition].getValue(addr) );


    return result;

}


