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

typedef enum {
    e_ADD,
    e_MULT,
    e_NUM_OPERATIONS = 2
} operators_t;

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
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

long long perform_math(long long a, long long b, operators_t op)
{
    long long result = 0;

    switch (op)
    {
    case e_ADD:
        result = a + b;
        break;

    case e_MULT:
        result = a * b;
        break;
    
    default:
        break;
    }

    return result;
}

long long perform_combination_of_ops(vector<long long> const v, int combination_num)
{
    long long final_val = v[0];
    size_t v_size = v.size();

    for (int i = 1; i < v_size; i++)
    {
        int op_type = combination_num % 2;
        combination_num = floor(combination_num/2);

        final_val = perform_math(final_val, v[i], (operators_t) op_type);
    }

    return final_val;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d7.txt";
    file_path = "./d7.txt";

    fileOperations<long long> fOp(file_path);

    vector<long long> test_values;
    vector<vector<long long>> op_values;
    vector<vector<long long>> equations;
    fOp.get_num_data_from_file(equations, ' ');

    for (int i = 0; i < equations.size(); i++)
    {
        test_values.push_back(equations[i][0]);

        vector<long long> temp_op_values;
        for (int j = 1; j < equations[i].size(); j++)
        {
            temp_op_values.push_back(equations[i][j]);
        }
        op_values.push_back(temp_op_values);
    }

    size_t num_of_lines = equations.size();
    long long part1_result = 0;

    for (int i = 0; i < num_of_lines; i++)
    {
        int num_elements = op_values[i].size();
        int operations = num_elements - 1;
        int combinations = pow(2, operations);

        for (int c = 0; c < combinations; c++)
        {
            if (test_values[i] == perform_combination_of_ops(op_values[i], c))
            {
                part1_result += test_values[i];
                break;
            }
        }
    }

    cout << "Part 1: " << part1_result << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}