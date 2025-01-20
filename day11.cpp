#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <tuple>
#include <math.h>
#include <stdint.h>

#include <chrono>

#include "file_operations.hpp"

using namespace std;

template<typename T>
void print_vector(vector<T> const v)
{
    cout << "Printing elements in 1D vector: " << endl;

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }

    cout << endl;
}

template<typename T>
void print_vector(vector<vector<T>> const v)
{
    cout << "Printing elements in 2D vector: " << endl;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j];
        }
        cout << endl;
    }
}

void convert_to_strings(vector<long long> numbers, vector<string>& strs)
{
    for (int i = 0; i < numbers.size(); i++)
    {
        strs.push_back(to_string(numbers[i]));
    }
}

long long witness_stones_evolution(vector<long long> initial_stones, int blinks)
{
    long long num_of_stones = 0;
    vector<vector<long long>> evolution_of_stones;

    evolution_of_stones.push_back(initial_stones);

    for (int i = 0; i < blinks; i++)
    {
        vector<long long> stones = evolution_of_stones.back();
        vector<long long> new_stones;

        for (long long j = 0; j < stones.size(); j++)
        {
            string str = to_string(stones[j]);

            if (stones[j] == 0)
            {
                new_stones.push_back(1);
            }
            else if (str.length() % 2 == 0)
            {
                long long split_length = str.length() / 2;
                long long start_idx = 0;

                for (int k = 0; k < 2; k++)
                {
                    new_stones.push_back(stoll(str.substr(start_idx, split_length)));
                    start_idx += split_length;
                }
            }
            else
            {
                new_stones.push_back((long long) (stones[j] * 2024));
            }
        }

        evolution_of_stones.push_back(new_stones);
    }

    num_of_stones = evolution_of_stones.back().size();

    // print_vector<long long>(evolution_of_stones);

    return num_of_stones;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d11.txt";
    // file_path = "./d11_dbg.txt";
    file_path = "./d11.txt";

    fileOperations<long long> fOp(file_path);

    vector<long long> stones;
    fOp.get_num_data_from_file(stones, ' ');

    // print_vector(stones);

    long long num_of_stones = witness_stones_evolution(stones, 25);

    cout << "No of stones part 1: " << num_of_stones << endl;

    long long num_of_stones_2 = witness_stones_evolution(stones, 75);

    cout << "No of stones part 2: " << num_of_stones << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}