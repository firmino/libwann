/*
 * Discriminator.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: fabricio
 */

#include "../include/Util.hpp"

using namespace std;
using namespace wann;


float util::calculateConfidence(unordered_map<string, float> &result)
{
    float max = 0.0;
    float secondMax = 0.0;

    for (auto it = result.begin(); it != result.end(); ++it )
    {
        float value = it->second;

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
    
    float confidence = 1.0 - (secondMax /max);
    return confidence;
}


float util::maxValue(unordered_map<string, float> &values)
{
    float max = 0.0;
    for (auto it = values.begin(); it != values.end(); ++it )
    {
        float value = it->second;
        if( (value - max) > 0.0001 ) 
        {
            max = value;
        }
    }

    return max;
}


string util::argMax(unordered_map<string, float> &values)
{
    float max = 0.0;
    string maxLabel = "";

    for (auto it = values.begin(); it != values.end(); ++it )
    {

        float value = it->second;

        if(max <= value)
        {
            max = value;
            maxLabel = it->first;
        }
    }

    return maxLabel;
}



