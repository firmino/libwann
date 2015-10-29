#include "../include/WiSARD.hpp"


#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
int  main(void)
{
	ifstream input_X ("teste_X_cpp");
	ifstream input_y ("teste_X_cpp");
	string line;

	std::vector<int> X;
	std::vector<string> y;


	int num;
	if (input_X.is_open())
  	{
		while(getline(input_X, line))
	    {

	    	num = (int) line[0];
	    	X.push_back(num);
	    }
	}
	else
	{
		cout << "cannot open file" << "\n";
		return 0;
	}
    input_X.close();

	int retinaLength;
	int numBitsAddr;

	WiSARD w (10, 2);

	vector<int> values = { -8, 8, 0, 9, 5, -3, 4, 6, -1, 15, 31 };
    sort(values.begin(), values.end());
    		
    cout <<  values[values.size()-1]<< ", " << values[values.size() -2] << std::endl;
    cout <<"CARALHO";

	return 0;
}