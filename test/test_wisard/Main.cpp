#include <WiSARD>
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
#include <map>

using namespace std;


vector<int> split(string str, char delimiter) 
{
    vector<int> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
  
    while(getline(ss, tok, delimiter)) 
    {
        internal.push_back( stoi(tok));
    }
  
    return internal;
}


vector<string> split_str(string str, char delimiter) 
{
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
  
    while(getline(ss, tok, delimiter)) 
    {
        internal.push_back( tok );
    }

    return internal;
}




tuple< vector<vector<int>>, vector<string>> loadData(string XFile, string yFile)
{
 
    string line;

    ifstream input(XFile);
    ifstream annotation (yFile);

    vector<vector<int>> X;
    vector<string> y;

    //reading X
    if (input.is_open())
    {
        while(getline(input, line))
        {
            X.push_back(split(line, ','));
        }

        input.close();
    }

    //reading y
    getline(annotation, line);
    y = split_str(line, ',');

    return make_tuple(X,y);
}



int main(void)
{
    string XFile = "../data/imdb_cpp_X.csv";
    string yFile = "../data/imdb_cpp_y.csv";

    vector<vector<int>> X;
    vector<string> y;

    tie(X, y) = loadData(XFile, yFile);

    for(int i = 0; i < X.size(); i++)
    {
        for(int j = 0; j < X[i].size(); j++)
            cout << X[i][j];
        cout << "\n";

    } 

    cout << "FOI";
}