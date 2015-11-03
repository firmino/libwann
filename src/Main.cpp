#include "../include/WiSARD.hpp"
#include "../include/SS_WiSARD.hpp"


#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <tuple>
#include <random>
#include <chrono>

using namespace std;

vector<int> split(string str, char delimiter) {
    vector<int> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
  
    while(getline(ss, tok, delimiter)) {
        internal.push_back( stoi(tok));
    }
  
    return internal;
}

vector<string> split_str(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
  
    while(getline(ss, tok, delimiter)) {
        internal.push_back( tok );
    }
  
    return internal;
}

//class tuple : public _Tuple_impl(vector<vector<int>>, vector<string>, vector<vector<int>>, vector<vector<int>>, vector<string>);

tuple<vector<vector<int>>, vector<string>, vector<vector<int>>, vector<vector<int>>, vector<string>> randomSubSampling (vector<vector<int>> &input_X, vector<string> &input_y, float f_X, float f_Xun, float f_testing)
{
    if(f_X + f_Xun + f_testing > 1.0)
    {
        cout << "f_X + f_Xun + f_testing cannot sum more than 1.0" << endl;
        exit(0);
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(begin(input_X), end(input_X), default_random_engine(seed));
    shuffle(begin(input_y), end(input_y), default_random_engine(seed));

    int lenght_X = input_X.size();

    float aux = f_X * lenght_X;
    int end_input = (int) aux;
    aux = f_Xun * lenght_X;
    int end_unlabeled_input = (int) aux;
    aux = f_testing * lenght_X;
    int testing_end = (int) aux;

    vector<vector<int>>::const_iterator first = input_X.begin();
    vector<vector<int>>::const_iterator mid = input_X.begin() + end_input;
    vector<vector<int>>::const_iterator mid2 = input_X.begin() + end_input + end_unlabeled_input;
    vector<vector<int>>::const_iterator last = input_X.begin() + end_input + end_unlabeled_input + testing_end;

    vector<string>::const_iterator first_y = input_y.begin(); 
    vector<string>::const_iterator mid_y = input_y.begin() + end_input;
    vector<string>::const_iterator mid2_y = input_y.begin() + end_input + end_unlabeled_input;
    vector<string>::const_iterator last_y = input_y.begin() + end_input + end_unlabeled_input + testing_end;

    vector<vector<int>> X(first, mid);
    vector<vector<int>> Xun(mid, mid2);
    vector<vector<int>> testing_X(mid2, last);

    vector<string> y(first_y, mid_y);
    vector<string> testing_y(mid2_y, last_y);
    
    return make_tuple(X, y, Xun, testing_X, testing_y);
}

int  main(void)
{
    ifstream input ("new_sts_cpp_X.csv");
    ifstream annotation ("new_sts_cpp_y.csv");
    string line;

    std::vector<vector<int>> X;
    std::vector<string> y;

    //reading X

    if (input.is_open())
    {
        while(getline(input, line))
        {
            X.push_back(split(line, ','));
        }
    }
    else
    {
        cout << "cannot open file" << "\n";
        return 0;
    }

    //reading y
    getline(annotation, line);
    y = split_str(line, ',');

    int len_vector = X.size();
    int len_y = y.size();

    input.close();
    annotation.close();

    int retinaLength = X[0].size();

    int numBitsAddr;
    vector<vector<int>> input_X;
    vector<string> input_y;
    vector<vector<int>> input_Xun;
    vector<vector<int>> testing_X;
    vector<string> testing_y;

    //tuple<vector<vector<int>>, vector<string>, vector<vector<int>>, vector<vector<int>>, vector<string>> sampling = randomSubSampling(X, y, 0.1, 0.6, 0.3);
    tie(input_X, input_y, input_Xun, testing_X, testing_y) = randomSubSampling(X, y, 0.1, 0.6, 0.3);
    
    cout << "X : " << input_X.size() << "y : " << input_y.size() << "Xun : " << input_Xun.size() << "testing_X : " << testing_X.size() << "testing_y: " << testing_y.size() << endl;
    SS_WiSARD w (retinaLength, 24, {"-1", "1"}, true, 0.0, true, 0.9278612484835791);

    clock_t tStart = clock();
    w.fit(input_X, input_Xun, input_y);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    int count = 0;
    tStart = clock();
    for(int i =0; i < testing_X.size(); i++)
    {   

        string rightLabel =  testing_y[i];
       
        auto result = w.predict(testing_X[i]);
     
        string predictedLabel = "";
        int maxValue = -1;

        for (auto it = result.begin(); it != result.end(); ++it )
        {
            string label = it->first;
            int value = it->second;
            
            if(value > maxValue)
            {
                predictedLabel = label;
                maxValue = value;
            }    
        }

        if(rightLabel == predictedLabel)
            count++;        
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    float acc = count/ ((float)testing_X.size());
    cout << "ACURACIA: "<< acc << "\n"; 
        
    return 0;            
}