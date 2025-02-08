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

template<typename T>
void print_vector(vector<pair<T,T>> const v)
{
    cout << "Printing elements in 1D vector: " << endl;

    for (int i = 0; i < v.size(); i++)
    {
        cout << "(" << v[i].first << ", " << v[i].second << ") ";
    }
    cout << endl;
}

template<typename T>
void print_vector(vector<vector<pair<T,T>>> const v)
{
    cout << "Printing elements in 2D vector: " << endl;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << "(" << v[i][j].first << ", " << v[i][j].second << ") ";
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
            cout << "(" << p.first << "," << p.second << ") ";
        }
        cout << endl;
    }
}

typedef enum
{
    e_LEFT = 0,
    e_RIGHT,
    e_UP,
    e_DOWN,
    e_NUM_DIR = 4
} directions_t;

typedef struct 
{
    int16_t row;
    int16_t col;
} cell_t;

int dir_arr[e_NUM_DIR][2] = {{0, -1},
                             {0, 1},
                             {-1, 0},
                             {1, 0}};
                                
void find_garden_types(const vector<vector<char>>& garden_plots, map<char, vector<pair<int,int>>>& garden_types)
{
    size_t dimension = garden_plots.size();

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            garden_types[garden_plots[i][j]].push_back(make_pair(i,j));
        }
    }
    // print_map(garden_types);
}

void calculate_garden_perimeters(const vector<vector<char>>& garden_plots, vector<vector<int>>& garden_perimeters)
{
    size_t dimension = garden_plots.size();
    // cout << "Dimension: " << dimension << endl;

    directions_t dir = e_LEFT;
    cell_t c;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            for (int k = 0; k < e_NUM_DIR; k++)
            {
                c.row = i + dir_arr[dir][0];
                c.col = j + dir_arr[dir][1];

                if (c.row == -1 || c.row == dimension || c.col == -1 || c.col == dimension)
                {
                    garden_perimeters[i][j]++;
                }
                else if (garden_plots[c.row][c.col] != garden_plots[i][j])
                {
                    garden_perimeters[i][j]++;
                }
                else
                {
                    // Do nothing
                }

                dir = (directions_t) ((dir + 1) % e_NUM_DIR);
            }
        }
    }
}

void find_garden_regions(const vector<vector<char>>& garden_plots, map<char, vector<pair<int,int>>> garden_types, vector<vector<pair<int,int>>>& garden_regions)
{
    pair<int,int> c;
    map<pair<int,int>, bool> visited_history;
    size_t dimension = garden_plots.size();

    for (const auto& row : garden_types)
    {
        size_t num_indices = row.second.size();
        int visited_cells = 0;

        int i = 0;

        while(visited_cells < num_indices)
        {
            if (visited_history[row.second[i]] == false)
            {
                vector<pair<int,int>> region_depth_graph;
                vector<pair<int,int>> region_graph;
                region_depth_graph.push_back(row.second[i]);
                // if (garden_plots[row.second[i].first][row.second[i].second] == 'C')
                // {
                //     cout << endl;
                //     cout << "(" << row.second[i].first << ", " << row.second[i].second << ")";
                // }

                while (region_depth_graph.size() != 0)
                {
                    region_graph.push_back(region_depth_graph.back());
                    region_depth_graph.pop_back();
                    pair<int,int> current_cell = region_graph.back();

                    directions_t dir = e_LEFT;
                    for (int k = 0; k < e_NUM_DIR; k++)
                    {
                        c.first = current_cell.first + dir_arr[dir][0];
                        c.second = current_cell.second + dir_arr[dir][1];

                        if (!(c.first == -1 || c.first == dimension || c.second == -1 || c.second == dimension))
                        {
                            if (garden_plots[c.first][c.second] == row.first)
                            {
                                if ((find(region_graph.begin(), region_graph.end(), c) == region_graph.end()) &&
                                    (find(region_depth_graph.begin(), region_depth_graph.end(), c) == region_depth_graph.end()))
                                {
                                    // if (garden_plots[c.first][c.second] == 'C')
                                    // {
                                    //     cout << "(" << c.first << ", " << c.second << ")";
                                    // }
                                    region_depth_graph.push_back(c);
                                }
                            }
                        }
                        dir = (directions_t) ((dir + 1) % e_NUM_DIR);
                    }

                    if (visited_history[current_cell] == false)
                    {
                        visited_cells++;
                        visited_history[current_cell] = true;
                    }
                }
                garden_regions.push_back(region_graph);
            }
            else
            {
                i = (i + 1) % num_indices;
            }
        }
    }
    // cout << endl;
    // print_vector<int>(garden_regions);
}

long long calculate_fencing_price(const vector<vector<pair<int,int>>>& garden_regions, const vector<vector<int>>& garden_perimeters)
{
    long long fencing_price = 0;
    size_t total_garden_regions = garden_regions.size();

    for (int i = 0; i < total_garden_regions; i++)
    {
        int area = garden_regions[i].size();
        int perimeter = 0;

        for (int j = 0; j < area; j++)
        {
            perimeter += garden_perimeters[garden_regions[i][j].first][garden_regions[i][j].second];
        }

        fencing_price += (area * perimeter);
    }

    return fencing_price;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    file_path = "./input_sample_d12.txt";
    // file_path = "./d12_dbg.txt";
    // file_path = "./d12.txt";

    fileOperations<char> fOp(file_path);

    vector<vector<char>> garden_plots;
    map<char, vector<pair<int,int>>> garden_types;
    map<char, int> garden_areas;
    vector<vector<pair<int,int>>> garden_regions;
    long long fencing_price = 0;

    fOp.get_char_data_from_file(garden_plots);
    // print_vector<char>(garden_plots);
    find_garden_types(garden_plots, garden_types);

    size_t dimension = garden_plots.size();
    vector<vector<int>> garden_perimeters(dimension, vector<int>(dimension, 0));
    calculate_garden_perimeters(garden_plots, garden_perimeters);

    find_garden_regions(garden_plots, garden_types, garden_regions);
    fencing_price = calculate_fencing_price(garden_regions, garden_perimeters);

    cout << "Fencing price, part 1: " << fencing_price << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}