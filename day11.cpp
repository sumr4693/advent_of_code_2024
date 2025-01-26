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
        cout << v[i] << endl;
    }
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

void convert_to_strings(vector<long long> numbers, vector<string>& strs)
{
    for (int i = 0; i < numbers.size(); i++)
    {
        strs.push_back(to_string(numbers[i]));
    }
}

void write_map_to_file(map<long long, vector<long long>> myMap, string filepath)
{
    ofstream out (filepath);

    for (auto& line : myMap)
    {
        string s;

        s += to_string(line.first) + ":";

        for (auto& ele : line.second)
        {
            s += " " + to_string(ele);
        }

        s += "\n";

        out << s;
    }

    out.close();
}

long long witness_stones_evolution(vector<long long> initial_stones, map<long long, vector<long long>>& stone_evolution, int blinks)
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
            if (stone_evolution.count(stones[j]) != 0)
            {
                size_t num_of_next_stones = stone_evolution[stones[j]].size();

                for (int k = 0; k < num_of_next_stones; k++)
                {
                    new_stones.push_back(stone_evolution[stones[j]][k]);
                }
            }
            else
            {
                string str = to_string(stones[j]);

                if (stones[j] == 0)
                {
                    new_stones.push_back(1);
                    stone_evolution[stones[j]].push_back(1);
                }
                else if (str.length() % 2 == 0)
                {
                    long long split_length = str.length() / 2;
                    long long start_idx = 0;

                    for (int k = 0; k < 2; k++)
                    {
                        new_stones.push_back(stoll(str.substr(start_idx, split_length)));
                        stone_evolution[stones[j]].push_back(stoll(str.substr(start_idx, split_length)));
                        start_idx += split_length;
                    }
                }
                else
                {
                    new_stones.push_back((long long) (stones[j] * 2024));
                    stone_evolution[stones[j]].push_back((long long) (stones[j] * 2024));
                }
            }
        }

        evolution_of_stones.push_back(new_stones);

            // cout << "No of stones at " << i+1 << " blink: " << new_stones.size() << endl;
    }

    num_of_stones = evolution_of_stones.back().size();

    // print_vector<long long>(evolution_of_stones);

    return num_of_stones;
}

long long witness_stones_evolution_dfs_memo(vector<long long> initial_stones, map<long long, vector<long long>>& stone_evolution, int max_blinks)
{
    long long num_of_stones = 0;

    for (int i = 0; i < initial_stones.size(); i++)
    {
        vector<pair<long long, int>> stone_graph;

        stone_graph.push_back(make_pair(initial_stones[i], 0));

        while (stone_graph.size() != 0)
        {
            pair<long long, int> stone_at_t = stone_graph.back();
            stone_graph.pop_back();

            // Store the neighbours in the graph
            if (stone_evolution.count(stone_at_t.first) != 0)
            {
                size_t neighbours = stone_evolution[stone_at_t.first].size();

                if (stone_at_t.second == max_blinks - 1)
                {
                    num_of_stones += neighbours;
                }
                else
                {
                    for (int k = 0; k < neighbours; k++)
                    {
                        stone_graph.push_back(make_pair(stone_evolution[stone_at_t.first][k], stone_at_t.second + 1));
                    }
                }
            }
            else
            {
                string str = to_string(stone_at_t.first);

                if (stone_at_t.second == max_blinks - 1)
                {
                    if (str.length() % 2 == 0)
                    {
                        num_of_stones += 2;
                    }
                    else
                    {
                        num_of_stones += 1;
                    }
                }
                else
                {
                    if (stone_at_t.first == 0)
                    {
                        stone_evolution[stone_at_t.first].push_back(1);
                        stone_graph.push_back(make_pair(1, stone_at_t.second + 1));
                    }
                    else if (str.length() % 2 == 0)
                    {
                        long long split_length = str.length() / 2;
                        long long start_idx = 0;

                        for (int k = 0; k < 2; k++)
                        {
                            stone_evolution[stone_at_t.first].push_back(stoll(str.substr(start_idx, split_length)));
                            stone_graph.push_back(make_pair(stoll(str.substr(start_idx, split_length)), stone_at_t.second + 1));
                            start_idx += split_length;
                        }
                    }
                    else
                    {
                        stone_evolution[stone_at_t.first].push_back((long long) (stone_at_t.first * 2024));
                        stone_graph.push_back(make_pair((long long) (stone_at_t.first * 2024), stone_at_t.second + 1));
                    }
                }
            }
        }
    }

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

    map<long long, vector<long long>> stone_evolution;

    long long num_of_stones = witness_stones_evolution_dfs_memo(stones, stone_evolution, 25);
    cout << "No of stones part 1: " << num_of_stones << endl;
    cout << "Stone evolution memoization size after 25 blinks: " << stone_evolution.size() << endl;

    long long num_of_stones_2 = witness_stones_evolution_dfs_memo(stones, stone_evolution, 75);
    cout << "No of stones part 2: " << num_of_stones_2 << endl;
    cout << "Stone evolution memoization size after 75 blinks: " << stone_evolution.size() << endl;

    // write_map_to_file(stone_evolution, "./d11_dbg_out1.txt");

    // vector<long long> stone_zero_cache;

    // stone_zero_evolution(stone_zero_cache, 75);
    // print_vector<long long>(stone_zero_cache);

    // long long num_of_stones_2 = witness_stones_evolution(stones, 75);

    // cout << "No of stones part 2: " << num_of_stones << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}