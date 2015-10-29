#include "../include/WiSARD.hpp"


#include <iostream>

#include <algorithm>
#include <vector>


using namespace std;


int  main(void)
{
	int retinaLength;
	int numBitsAddr;

	WiSARD w (10, 2);


	vector<int> values = { -8, 8, 0, 9, 5, -3, 4, 6, -1, 15, 31 };
    sort(values.begin(), values.end());
    		
    cout <<  values[values.size()-1]<< ", " << values[values.size() -2] << std::endl;
    cout <<"CARALHO";

	return 0;
}