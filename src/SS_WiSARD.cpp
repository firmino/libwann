/*
 * SS_WiSARD.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: fabio
 */

#include "../include/SS_WiSARD.hpp"
#include "../include/WiSARD.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>


using namespace std;

SS_WiSARD::SS_WiSARD(int retinaLength, 
					 int numBitsAddr, 
					 vector<string> classes,
					 bool useBleaching, 
					 float confidenceThreshold, 
					 bool ignoreZeroAddr,
					 float ssThreshold)

:retinaLength(retinaLength),
 numBitsAddr(numBitsAddr),
 classes(classes),
 useBleaching(useBleaching),
 confidenceThreshold(confidenceThreshold),
 ignoreZeroAddr(ignoreZeroAddr),
 ssThreshold(ssThreshold)
 {
 	wisard = new WiSARD(retinaLength, 
 						numBitsAddr,
 						useBleaching,
 						confidenceThreshold,
 						1,
 						true,
 						true,
 						ignoreZeroAddr);
 	for(int i = 0; i < classes.size(); i++)
 	{
 		wisard->createDiscriminator(classes[i]);
 	}

 }
SS_WiSARD::~SS_WiSARD(void)
{
	
	delete wisard;

}
void SS_WiSARD::fit(const vector< vector<int> > &X, const vector< vector<int> > &Xun, const vector<string> &y)
 {
 		wisard->fit(X, y);
 		for(int i = 0; i < Xun.size(); i++)
 		{
 			int maxValue = -1;
 			string predictedLabel = "";
 			unordered_map<string, int> prediction = wisard->predict(Xun[i]);
 			float confidence = wisard->calculateConfidence(prediction);
 			if(confidence > ssThreshold)
 			{
 				for (auto it = prediction.begin(); it != prediction.end(); ++it )
		        {
		            string label = it->first;
		            int value = it->second;
		            
		            if(value > maxValue)
		            {
		                predictedLabel = label;
		                maxValue = value;
		            }
		        }
		        unlabeledTraining.push_back(Xun[i]);
		        labels.push_back(predictedLabel);
 			}
 		}
 		wisard->fit(unlabeledTraining, labels);
 }
unordered_map<string, int> SS_WiSARD::predict(const vector<int> &retina)
{
	return wisard->predict(retina);
}