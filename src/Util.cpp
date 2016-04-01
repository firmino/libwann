/*
 * Discriminator.cpp
 *
 *  Created on: Oct 2, 2015
 *      Author: fabricio
 */

#include "../include/Util.hpp"


float util::calculateConfidence(unordered_map<string, int> &result)
{
    int max = 0;
    int secondMax = 0;

    for (auto it = result.begin(); it != result.end(); ++it )
    {
        int value = it->second;

        if(max < value)
        {
            secondMax = max;
            max = value;
        }
        else if(secondMax < value)
        {
            secondMax = value;
        }
    }
    
    float confidence = 1.0 - ( ( (float)secondMax )/max);
    return confidence;
}
