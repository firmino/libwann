/*
 * Util.cpp
 *
 *  Created on: Apr 1, 2016
 *      Author: fabricio
 */


#ifndef UTIL_CPP_
#define UTIL_CPP_

#include <string>
#include <unordered_map>


namespace wann
{
	namespace util 
	{
		float calculateConfidence(std::unordered_map<std::string, int> &);
		float maxValue(std::unordered_map<std::string, float> &);
		std::string argMax(std::unordered_map<std::string, float>&v);
	}
}

#endif /* UTIL_CPP_ */
