#include "../include/SS_WiSARD.hpp"
#include "../include/WiSARD.hpp"



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
float getAccuracy(SS_WiSARD * ssw, vector<vector<int>> &testing_X, vector<string> &testing_y)
{
    float count = 0.0;
    int testing_size = testing_X.size();

    for(int k = 0; k < testing_size; k++)
    {
        auto prediction = ssw->predict(testing_X[k]);
        string rightLabel =  testing_y[k];           
        string predictedLabel = "";
        int maxValue = -1;
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
        if(rightLabel == predictedLabel)
            count += 1.0;
    }
    float accuracy = count/testing_size;
    return count/testing_size;
}

void crossover (vector<tuple<int, bool, float, bool, float>> &index, vector<float> &results, int num_survivers)
{
    cout << "starting crossover" << endl;
    std::map<float, int> lists;
    int population_size = index.size();
    std::map<float,int>::iterator it = lists.begin();
    std::vector<tuple<int, bool, float, bool, float>> new_index;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> get_int(0, num_survivers - 1);
    std::uniform_int_distribution<> get_zero_one(0, 1);
    std::uniform_int_distribution<> random_param_number(0, 4);
    std::uniform_real_distribution<> get_real(0, 1);
    std::uniform_int_distribution<> get_random_numbits(2, 32);

    tuple<int, bool, float, bool, float> aux_tuple;

    float mutation_rate = 0.001;

    array<int, 2> num_bits;
    array<bool, 2> bleaching;
    array<float, 2> conf_threshold;
    array<bool, 2> ignoreZeros;
    array<float, 2> ss_threshold;
    int crossover_father = 0;
    int crossover_mother = 0;
    for(int i = 0; i < population_size; i++)
    {
        lists.insert(std::pair<float,int>((-1)*results[i], i));
    }
    int i = 0;
    for(it=lists.begin(); (it!=lists.end() && i != num_survivers); ++it)
    {
        new_index.push_back(index[it->second]);
        i++;
    }
    for(int i = 0; i < population_size - num_survivers; i++)
    {
        do
        {
            crossover_father = get_int(gen);
            crossover_mother = get_int(gen);
        }while(crossover_mother == crossover_father);
        tie(num_bits[0], bleaching[0], conf_threshold[0], ignoreZeros[0], ss_threshold[0]) = new_index[crossover_father];
        tie(num_bits[1], bleaching[1], conf_threshold[1], ignoreZeros[1], ss_threshold[1]) = new_index[crossover_mother];

        aux_tuple = make_tuple(num_bits[get_zero_one(gen)], bleaching[get_zero_one(gen)], conf_threshold[get_zero_one(gen)], ignoreZeros[get_zero_one(gen)], ss_threshold[get_zero_one(gen)]);
        //mutation goes here
        if(get_real(gen) < mutation_rate)
        {            
            switch(random_param_number(gen))
            {
                case 0:
                    std::get<0>(aux_tuple) = get_random_numbits(gen);
                case 1:
                    std::get<1>(aux_tuple) = get_zero_one(gen);
                case 2:
                    std::get<2>(aux_tuple) = get_real(gen);
                case 3:
                    std::get<3>(aux_tuple) = get_zero_one(gen);
                case 4:
                    std::get<4>(aux_tuple) = get_real(gen);
            }
        }
        new_index.push_back(aux_tuple);      
    }
    index = new_index;
    cout << "ended crossover"<< endl;
}

int genetic_optimization(int generations, int init_pop, int num_survivers, int iter_number, int retinaLength, vector<vector<int>> &X, vector<string> y)
{
    float global_best_accuracy = 0.0;
    int num_bits;
    float ss_threshold;
    float conf_threshold;
    bool ignoreZeros;
    bool bleaching;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution get_bool(0.5);
    std::uniform_int_distribution<> get_int(2, 31);
    std::uniform_real_distribution<> get_real(0, 1);

    vector<vector<int>> input_X;
    vector<string> input_y;
    vector<vector<int>> input_Xun;
    vector<vector<int>> testing_X;
    vector<string> testing_y;
    ofstream exit_file;
    exit_file.open("./results/optResult.txt");
    SS_WiSARD * ssw=NULL;
    std::vector<tuple<int, bool, float, bool, float>> params;
    tuple<int, bool, float, bool, float> best_params;
    int best_index;
    float sum = 0.0;
    float local_max = 0.0;
    float accuracy = 0.0;

    for(int i = 0; i < generations; i++)
    {
        cout << "starting generation: " << i << endl;
        vector<float> results;
        local_max = 0.0;
        for(int j = 0; j < init_pop; j++)
        {   
            
            if(global_best_accuracy == 0.0) //first iteration :: get random params
            {
                num_bits = get_int(gen);
                bleaching = get_bool(gen);
                conf_threshold = get_real(gen);
                ignoreZeros = get_bool(gen);
                ss_threshold = get_real(gen);
                params.push_back(make_tuple(num_bits, bleaching, conf_threshold, ignoreZeros, ss_threshold));
                
            }
            else
            {
                tie(num_bits, bleaching, conf_threshold, ignoreZeros, ss_threshold) = params[j];
            }
            cout << "\tpop: " << j << " params: " << " | " << num_bits << " | " << bleaching << " | " << conf_threshold << " | " << ignoreZeros << " | " << ss_threshold << endl;                   
            sum = 0.0;
            for(int iteration = 0; iteration < iter_number; iteration++)
            {
            	if (ssw)
                	delete(ssw);
                ssw = new SS_WiSARD(retinaLength, num_bits, {"-1", "1"}, bleaching, conf_threshold, ignoreZeros, ss_threshold);
                tie(input_X, input_y, input_Xun, testing_X, testing_y) = randomSubSampling(X, y, 0.2, 0.5, 0.3);
                ssw->fit(input_X, input_Xun, input_y);
                sum += getAccuracy(ssw, testing_X, testing_y);                
            }
            accuracy = sum/iter_number;
            cout << "\t\tAccuracy:  " << accuracy << endl;
            results.push_back(accuracy);
            if(accuracy > local_max)
            {
                local_max = accuracy;
                best_index = j;
            }
        }
        if(local_max > global_best_accuracy)
        {   
            cout << "\n" << endl;
            best_params = params[best_index];
            global_best_accuracy = local_max;
            cout << "global_best_accuracy changed to: " << global_best_accuracy << endl;
            tie(num_bits, bleaching, conf_threshold, ignoreZeros, ss_threshold) = best_params;
            cout << "best params: " << " | " << num_bits << " | " << bleaching << " | " << conf_threshold << " | " << ignoreZeros << " | " << ss_threshold << endl;
            exit_file << "accuracy: " << global_best_accuracy << ", num_bits: " << num_bits <<  ", bleaching: " << bleaching <<  ", conf_threshold: " << conf_threshold <<  ", ignoreZeros: " << ignoreZeros << ", ss_threshold: " << ss_threshold << "\n";
            exit_file.flush();
        }
        cout << "\n" << endl;
        crossover(params, results, num_survivers); 
    }
    exit_file.close();
    return 0;
}
int  main(void)
{
    ifstream input ("data/imdb_cpp_X.csv");
    ifstream annotation ("data/imdb_cpp_y.csv");
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
    
    // int numBitsAddr;
    // vector<vector<int>> input_X;
    // vector<string> input_y;
    // vector<vector<int>> input_Xun;
    // vector<vector<int>> testing_X;
    // vector<string> testing_y;
    
    // tie(input_X, input_y, input_Xun, testing_X, testing_y) = randomSubSampling(X, y, 0.2, 0.5, 0.3);
    // ////9, bleaching: 0, conf_threshold: 0.0161234, ignoreZeros: 1, ss_threshold: 0.893476
    // SS_WiSARD * w = new SS_WiSARD(retinaLength, 9, {"-1", "1"}, true, 0.0161234, true, 0.001);
    // // WiSARD * w = new WiSARD(retinaLength, 8, true, 0.01, 1, true, true, true); //last is ignoreZero
    
    // // w->createDiscriminator("-1");
    // // w->createDiscriminator("1");
    
    // clock_t tStart = clock();
    // w->fit(input_X, input_Xun, input_y);
    // cout << "trained with: " << w->getNumberUnlabeledTrained() << endl;
    // //w->fit(input_X, input_y);
    // printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // tStart = clock();
    // int count = 0;
    // for(int i =0; i < testing_X.size(); i++)
    // {   

    //     string rightLabel =  testing_y[i];
    //     auto result = w->predict(testing_X[i]);
    //     string predictedLabel = "";
    //     int maxValue = -1;

    //     for (auto it = result.begin(); it != result.end(); ++it )
    //     {
    //         string label = it->first;
    //         int value = it->second;
    //         if(value > maxValue)
    //         {
    //             predictedLabel = label;
    //             maxValue = value;
    //         }    
    //     }
    //     if(rightLabel == predictedLabel)
    //         count++;
    // }
    // float acc = count/ ((float)testing_X.size());
    // printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    // cout << "ACURACIA: "<< acc << "\n"; 

    int generations = 40; 
    int init_pop = 100; 
    int num_survivers = 20; 
    int iter_number = 30;

    genetic_optimization(generations, init_pop, num_survivers, iter_number, retinaLength, X, y);
    
    return 0;            
}