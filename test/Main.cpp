#include <iostream>
#include <wann/WiSARD.hpp>

using namespace wann;
using namespace std;
int main()
{
    vector<int> x1 = {0,0,0,1};
    vector<int> x2 = {0,0,1,1};
    vector<int> x3 = {1,1,0,1};
    vector<int> x4 = {1,1,0,1};

    vector<vector<int>> X = {x1,x2};
    vector<string> y = {"1","2"};

    WiSARD * w = new WiSARD(2, false, 0.1, 1, true, true, false, 10);

    w->onFit(X,y);

    Discriminator * d1 = w->discriminators["1"];
    Discriminator * d2 = w->discriminators["2"];

    int numMemories1 = d1->numMemories;
    int numMemories2 = d2->numMemories;

    vector<Memory *> memories1 = d1->memories;
    vector<Memory *> memories2 = d2->memories;

    cout << "numero de memorias d1: " << numMemories1 << endl;
    cout << "numero de memorias d2: " << numMemories2 << endl;


    for(int i = 0; i < memories1.size(); i++)
    {
        
        cout << "Iteration: " << i << endl;
        unordered_map<long long, int> data = memories1[i]->data;

        for(auto it = data.begin(); it != data.end(); ++it)
        {
            cout << it->second << endl;
        }
    }

    vector<vector<int>> X2 = {x3,x4};
    vector<string> y2 = {"1","1"};

    w->onFit(X2,y2);

    cout << "After training" << endl;

    for(int i = 0; i < memories1.size(); i++)
    {
        
        cout << "Iteration: " << i << endl;
        unordered_map<long long, int> data = memories1[i]->data;

        for(auto it = data.begin(); it != data.end(); ++it)
        {
            cout << it->second << endl;
        }
    }
}
