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

void print_map(map<char, vector<pair<int,int>>> myMap)
{
    for(const auto& elem : myMap)
    {
        cout << elem.first << ": ";

        for (const auto& p : elem.second)
        {
            cout << "(" << p.first << ", " << p.second << ") ";
        }

        cout << endl;
    }
}

void print_vector_pairs(vector<pair<int,int>> pairs)
{
    for (const auto& p : pairs)
    {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
}

void find_antennas(vector<vector<char>> const input_map, map<char, vector<pair<int,int>>>& antennas)
{
    size_t map_size = input_map.size();

    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if(input_map[i][j] != '.')
            {
                antennas[input_map[i][j]].emplace_back(make_pair(i,j));
            }
        }
    }
}

pair<int,int> find_collinear_point(pair<int,int> p1, pair<int,int> p2)
{
    pair<int,int> xy_length = make_pair(p2.first - p1.first, p2.second - p1.second);

    return make_pair(p2.first + xy_length.first, p2.second + xy_length.second);
}

bool is_out_of_bounds(size_t map_size[], pair<int,int> val)
{
    bool out_of_bounds = false;

    if (val.first < 0 || val.second < 0 || val.first >= map_size[0] || val.second >= map_size[1])
    {
        out_of_bounds = true;
    }

    return out_of_bounds;
}

void find_antinodes(size_t map_size[], map<char, vector<pair<int,int>>> const antennas, vector<pair<int,int>>& antinodes)
{
    for (auto it = antennas.begin(); it != antennas.end(); ++it)
    {
        size_t num_of_positions = it->second.size();
        pair<int,int> potential_antinode;
        for(int i = 0; i < num_of_positions; i++)
        {
            int j = (i + 1) % num_of_positions;
            while (j != i)
            {
                potential_antinode = find_collinear_point(it->second[i], it->second[j]);

                if(!is_out_of_bounds(map_size, potential_antinode))
                {
                    if (count(antinodes.begin(), antinodes.end(), potential_antinode) == 0)
                    {
                        antinodes.push_back(potential_antinode);
                    }
                }
                j = (j+1) % num_of_positions;
            }
        }
    }
}

void find_resonant_harmonics_based_antinodes(size_t map_size[], map<char, vector<pair<int,int>>> const antennas, vector<pair<int,int>>& antinodes)
{
    for (auto it = antennas.begin(); it != antennas.end(); ++it)
    {
        size_t num_of_positions = it->second.size();
        pair<int,int> potential_antinode;
        for(int i = 0; i < num_of_positions; i++)
        {
            int j = (i + 1) % num_of_positions;

            if (j != i)
            {
                if (count(antinodes.begin(), antinodes.end(), it->second[i]) == 0)
                {
                    antinodes.push_back(it->second[i]);
                }
                if (count(antinodes.begin(), antinodes.end(), it->second[j]) == 0)
                {
                    antinodes.push_back(it->second[j]);
                }
            }

            while (j != i)
            {
                pair<int,int> p1 = it->second[i];
                pair<int,int> p2 = it->second[j];
                potential_antinode = find_collinear_point(p1, p2);

                while(!is_out_of_bounds(map_size, potential_antinode))
                {
                    if (count(antinodes.begin(), antinodes.end(), potential_antinode) == 0)
                    {
                        antinodes.push_back(potential_antinode);
                    }

                    p1 = p2;
                    p2 = potential_antinode;
                    potential_antinode = find_collinear_point(p1, p2);
                }
                j = (j+1) % num_of_positions;
            }
        }
    }
}

void populate_char(vector<vector<char>> &map_2d, vector<pair<int,int>> const indexes, char populate_ch)
{
    for (int i = 0; i < indexes.size(); i++)
    {
        if (map_2d[indexes[i].first][indexes[i].second] == '.')
        {
            map_2d[indexes[i].first][indexes[i].second] = populate_ch;
        }
    }
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d8.txt";
    file_path = "./d8.txt";

    fileOperations<char> fOp(file_path);

    vector<vector<char>> map_2d;
    fOp.get_char_data_from_file(map_2d);

    map<char, vector<pair<int,int>>> antennas;
    vector<pair<int,int>> antinodes;
    vector<pair<int,int>> updated_antinodes;
    find_antennas(map_2d, antennas);

    size_t map_size[] = {map_2d.size(), map_2d[0].size()};

    find_antinodes(map_size, antennas, antinodes);

    cout << "No. of antinodes: " << antinodes.size() << endl;

    vector<vector<char>> map_with_antinodes = map_2d;
    populate_char(map_with_antinodes, antinodes, '#');

    find_resonant_harmonics_based_antinodes(map_size, antennas, updated_antinodes);
    cout << "No. of updated antinodes: " << updated_antinodes.size() << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}