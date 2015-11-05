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

    if(!hasRestMemory)
    {
        for(int i=0; i<numMemories; i++)
        {
            memories.push_back(new Memory(numBits, isCummulative, ignoreZeroAddr));
        }

    }

    else
    {
        for(int i=0; i<numMemories - 1; i++)
            memories.push_back(new Memory(numBits, isCummulative, ignoreZeroAddr));

        //the rest memory 
        memories.push_back(new Memory((retinaLength % numBits), isCummulative, ignoreZeroAddr));
    }
    cout << memories.size() << endl;
}

Discriminator::~Discriminator(void)
{}

int Discriminator::getNumMemories(void)
{
    return numMemories;
}

vector<Memory *> Discriminator::getMemories()
{
    return memories;
}

vector<int> Discriminator::getMapping(long addr)
{
    return memoryAddressMapping;
}

Memory * Discriminator::getMemory(long addr)
{
    return memories[addr];
}

void Discriminator::addTrainning(const vector<int> &retina)
{
    int memIndex;
    long addr;
    long lastMemoryPosition;
    int restOfPositions;
    
    // each group of numBitsAddr is related with a memory
    for(int i=0; i < retinaLength; i+= numBitsAddr)
    {
        addr = 0;
        for(long j=0; j<numBitsAddr; j++)
        {
            if(retina[i+j] != 0)
            {
                addr += pow(2,j);
            }
        }
        if(addr == 64)
        {
            cout << retinaLength << ": retinaLength" << endl;
            cout << addr << ": addr" << endl;
            //cout << restOfPositions << ": restOfPositions" << endl;
            cout << memIndex << ": memIndex" << endl;
            cout << memories[memIndex]->getMemorySize() << ": many memories address" << endl;
        }
        memIndex = i / numBitsAddr;
        memories[memIndex]->addValue(addr, 1);
        //cout << "passou" << endl;

    }
    //  the rest of the retina (when the retina length is not a multiple of number of bits of address)

    restOfPositions = retinaLength % numBitsAddr;    
    if(restOfPositions != 0)
    {
        cout << restOfPositions << ": restOfPositions" << endl;
        cout << retinaLength << ": retinaLength" << endl;
        addr = 0;
        for(long j=0; j< restOfPositions; j++)
        {
            addr += pow(2,j) * retina[(retinaLength - restOfPositions - 1)+j];
        }
        if(addr == 64)
        {
            cout << retinaLength << ": retinaLength" << endl;
            cout << addr << ": addr" << endl;
            cout << restOfPositions << ": restOfPositions" << endl;
            cout << memIndex << ": memIndex" << endl;
        }
        cout << addr << ": addr" << endl;
        lastMemoryPosition = ceil(retinaLength/numBitsAddr);

        cout << lastMemoryPosition << ": lastMemoryPosition" << endl;
        memories[lastMemoryPosition]->addValue(addr, 1);
    }
    

}

vector<int> Discriminator::predict(const vector<int> &retina)
{
    long addr;
    int memIndex;
    int restOfPositions;
    long lastMemoryPosition;

    vector<int> result(numMemories);
    // each group of numBitsAddr is related with a memory
    for(int i=0; i < retinaLength; i+= numBitsAddr)
    {
        addr = 0;
        for(long j=0; j<numBitsAddr; j++)
        {
            addr += pow(2,j) * retina[i+j];
        }       
        memIndex = i / numBitsAddr;
        result.push_back( memories[memIndex]->getValue(addr) );
    }
    //  the rest of the retina (when the retina length is not a multiple of number of bits of address)   
    restOfPositions = retinaLength % numBitsAddr;
    addr = 0;
    if(restOfPositions != 0)
    {
        for(long j=0; j< restOfPositions; j++)
        {
            addr += pow(2,j) * retina[(retinaLength - restOfPositions - 1)+j];
        }        

        lastMemoryPosition = ceil(retinaLength/numBitsAddr);
        result.push_back( memories[lastMemoryPosition]->getValue(addr) );
    }

    return result;

}


