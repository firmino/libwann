/*
 * SS_WiSARD.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: fabio
 */

#ifndef SS_WISARD_HPP_
#define SS_WISARD_HPP_

#include "./WiSARD.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

 using namespace std;

class SS_WiSARD
{
	public:
		SS_WiSARD(int retinaLength, 
				  int numBitsAddr,
				  vector<string> classes,
				  bool useBleaching=true, 
				  float confidenceThreshold=0.1, 
				  bool ignoreZeroAddr=false,
				  float ssThreshold=0.1);

		~SS_WiSARD(void);
		void fit(const vector< vector<int> > &X, const vector< vector<int> > &Xun, const vector<string> &y);
		unordered_map<string, int> predict(const vector<int> &retina);
		int getNumberUnlabeledTrained();
	private:
		int retinaLength;
		int numBitsAddr;
		vector<string> classes;
		bool useBleaching;
		float confidenceThreshold;
		bool ignoreZeroAddr;
		float ssThreshold;
		vector< vector<int> > unlabeledTraining;
 		vector<string> labels;
		WiSARD *wisard;
		int unlabeledTraining_counter;

};

#endif /* SS_WISARD_HPP_ */