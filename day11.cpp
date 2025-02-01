#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <unordered_map>
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

long long witness_stones_evolution_dfs_memo(vector<long long> initial_stones, unordered_map<long long, vector<long long>>& stone_evolution_cache,
                                            map<pair<long long, int>, long long>& stone_count_cache, int max_blinks)
{
    long long num_of_stones = 0;

    for (int i = 0; i < initial_stones.size(); i++)
    {
        vector<pair<long long, int>> stone_graph;

        stone_graph.push_back(make_pair(initial_stones[i], 0));

        // If dfs graph is not empty
        while (stone_graph.size() != 0)
        {
            pair<long long, int> stone_at_t = stone_graph.back();

            int remaining_blinks = max_blinks - stone_at_t.second;
            static int prev_remaining_blinks = 0;

            // If the neighbours of stone at t are already stored in the evolution cache
            if (stone_evolution_cache.count(stone_at_t.first) != 0)
            {
                // If we already know the total number of stones for the remaining blinks from stone at t, then skip recomputation
                if (stone_count_cache[make_pair(stone_at_t.first, remaining_blinks)] != 0)
                {
                    stone_graph.pop_back();
                }
                else
                {
                    size_t neighbours = stone_evolution_cache[stone_at_t.first].size();
                    // Every time when a root node of neighbours is revisited, store the stone counts in cache,
                    // and remove the root node from the graph
                    if (remaining_blinks == prev_remaining_blinks + 1)
                    {
                        for (int k = 0; k < neighbours; k++)
                        {
                            auto neighbour_stone = stone_evolution_cache[stone_at_t.first][k];
                            auto stones = stone_count_cache[make_pair(neighbour_stone, remaining_blinks - 1)];
                            stone_count_cache[make_pair(stone_at_t.first, remaining_blinks)] += stones;
                        }
                        stone_graph.pop_back();
                    }
                    else
                    {
                        // Store the last blink neighbour counts in the stone count cache
                        if (remaining_blinks == 1)
                        {
                            stone_count_cache[make_pair(stone_at_t.first, remaining_blinks)] = neighbours;
                            stone_graph.pop_back();
                        }
                        // Else, store the neighbours in the graph for traversal
                        else
                        {
                            int current_blink = stone_at_t.second + 1;
                            for (int k = 0; k < neighbours; k++)
                            {
                                stone_graph.push_back(make_pair(stone_evolution_cache[stone_at_t.first][k], current_blink));
                            }
                        }
                    }
                }
            }
            else
            {
                string str = to_string(stone_at_t.first);

                if (remaining_blinks == 1)
                {
                    // Store the last blink neighbour counts in the stone count cache
                    if (str.length() % 2 == 0)
                    {
                        stone_count_cache[make_pair(stone_at_t.first, remaining_blinks)] = 2;
                    }
                    else
                    {
                        stone_count_cache[make_pair(stone_at_t.first, remaining_blinks)] = 1;
                    }
                    stone_graph.pop_back();
                }
                // Store the neighbours in both the evolution cache and the graph (for traversal)
                else
                {
                    int current_blink = stone_at_t.second + 1;
                    if (stone_at_t.first == 0)
                    {
                        stone_evolution_cache[stone_at_t.first].push_back(1);
                        stone_graph.push_back(make_pair(1, current_blink));
                    }
                    else if (str.length() % 2 == 0)
                    {
                        long long split_length = str.length() / 2;
                        long long start_idx = 0;

                        for (int k = 0; k < 2; k++)
                        {
                            stone_evolution_cache[stone_at_t.first].push_back(stoll(str.substr(start_idx, split_length)));
                            stone_graph.push_back(make_pair(stoll(str.substr(start_idx, split_length)), current_blink));
                            start_idx += split_length;
                        }
                    }
                    else
                    {
                        stone_evolution_cache[stone_at_t.first].push_back((long long) (stone_at_t.first * 2024));
                        stone_graph.push_back(make_pair((long long) (stone_at_t.first * 2024), current_blink));
                    }
                }
            }
            prev_remaining_blinks = remaining_blinks;
        }
        num_of_stones += stone_count_cache[make_pair(initial_stones[i], max_blinks)];
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

    unordered_map<long long, vector<long long>> stone_evolution_cache;
    map<pair<long long, int>, long long> stone_count_cache;

    long long num_of_stones = witness_stones_evolution_dfs_memo(stones, stone_evolution_cache, stone_count_cache, 25);
    cout << "No of stones part 1: " << num_of_stones << endl;
    cout << "Stone evolution memoization size after 25 blinks: " << stone_evolution_cache.size() << endl;

    long long num_of_stones_2 = witness_stones_evolution_dfs_memo(stones, stone_evolution_cache, stone_count_cache, 75);
    cout << "No of stones part 2: " << num_of_stones_2 << endl;
    cout << "Stone evolution memoization size after 75 blinks: " << stone_evolution_cache.size() << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}