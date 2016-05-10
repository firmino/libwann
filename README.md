# libwann
c++  Weightless Artificial Neural Network library

## Instructions

## How to Install

1. git clone https://github.com/firmino/libwann
2. cd libwann
3. make all (necessary clang++)
4. sudo make install

## How to Use


### WiSARD Signature

WiSARD(int retinaLength, 
	   int numBitsAddr, 
	   bool useBleaching=true, 
	   float confidenceThreshold=0.1, 
	   int defaultBleaching_b=1, 
	   bool randomizePositions=true, 
	   bool isCummulative=true, 
	   bool ignoreZeroAddr=false);

### How to Use

// import and other stuffs


vector<vector<int>> input_X;
vector<string> input_y;
vector<vector<int>> testing_X;

// load data into input_X, input_y and testing_X


int retinaLength = 10;
int numBitsAddr = 16;

WiSARD *w = new WiSARD(retinaLength, numBitsAddr);

w->fit(input_X, input_y);

vector<string> result = w->predict(testing_X);





