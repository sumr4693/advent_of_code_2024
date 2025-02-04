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
    print_map(garden_types);
}

void calculate_garden_perimeters(const vector<vector<char>>& garden_plots, vector<vector<int>>& garden_perimeters)
{
    size_t dimension = garden_plots.size();
    // cout << "Dimension: " << dimension << endl;

    int dir_arr[e_NUM_DIR][2] = {{0, -1},
                                 {0, 1},
                                 {-1, 0},
                                 {1, 0}};

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

long long calculate_fencing_price(const vector<vector<char>>& garden_plots, map<char, vector<pair<int,int>>> garden_types, const vector<vector<int>>& garden_perimeters)
{
    long long fencing_price = 0;
    size_t total_garden_types = garden_types.size();

    cout << "Type, Area, Perimeter: " << endl;

    for (const auto& row : garden_types)
    {
        int area = 0;
        int perimeter = 0;
        size_t num_indices = row.second.size();

        for (int i = 0; i < num_indices; i++)
        {
            if (i == 0)
            {
                area++;
                perimeter += garden_perimeters[row.second[i].first][row.second[i].second];
                continue;
            }
            else
            {

            }
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
    long long fencing_price = 0;

    fOp.get_char_data_from_file(garden_plots);
    // print_vector<char>(garden_plots);
    find_garden_types(garden_plots, garden_types);

    size_t dimension = garden_plots.size();
    vector<vector<int>> garden_perimeters(dimension, vector<int>(dimension, 0));
    calculate_garden_perimeters(garden_plots, garden_perimeters);

    // fencing_price = calculate_fencing_price(garden_plots, garden_types, garden_perimeters);

    // cout << "Fencing price, part 1: " << fencing_price << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}