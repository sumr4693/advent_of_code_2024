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

#define GRADUAL_SLOPE 1

typedef enum
{
    e_LEFT,
    e_RIGHT,
    e_UP,
    e_DOWN,
    e_NUM_DIR = 4
} directions_t;

typedef enum
{
    e_FORWARD,
    e_REVERSE
} traversal_direction_t;

typedef struct 
{
    bool valid_direction;
    pair<int,int> index;
} step_t;


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

void print_map(map<pair<int,int>, vector<tuple<int,int,int>>> myMap)
{
    for(const auto& elem : myMap)
    {
        cout << "(" << elem.first.first << "," << elem.first.second << ")" << ": ";

        for (const auto& p : elem.second)
        {
            cout << "(" << get<0>(p) << "," << get<1>(p) << "," << get<2>(p) << ") ";
        }
        cout << endl;
    }
}

int calculate_steps(pair<int,int> p1, pair<int,int> p2)
{
    return (abs(p2.first - p1.first) + abs(p2.second - p1.second));
}

bool is_gradual_slope(vector<vector<int>> const v_map, pair<int,int> current, pair<int,int> next, traversal_direction_t t_dir)
{
    bool result = false;
    int slope = 0;

    if (t_dir == e_FORWARD)
    {
        slope = GRADUAL_SLOPE;
    }
    else if (t_dir == e_REVERSE)
    {
        slope = -GRADUAL_SLOPE;
    }
    else
    {
        // Do nothing
    }

    if(v_map[next.first][next.second] - v_map[current.first][current.second] == slope)
    {
        result = true;
    }
    return result;
}

bool is_valid_next_step(vector<vector<int>> const v_map, size_t map_rows, size_t map_cols, pair<int,int> current, pair<int,int>& next, directions_t dir, traversal_direction_t t_dir)
{
    bool is_valid = false;
    switch(dir)
    {
        case e_LEFT:
            next = make_pair(current.first, current.second - 1);
            break;
        case e_RIGHT:
            next = make_pair(current.first, current.second + 1);
            break;
        case e_UP:
            next = make_pair(current.first - 1, current.second);
            break;
        case e_DOWN:
            next = make_pair(current.first + 1, current.second);
            break;
        default:
            cout << "Not a valid direction!!" << endl;
            break;
    }

    if (!(next.first < 0 || next.first >= map_rows || next.second < 0 || next.second >= map_cols))
    {
        is_valid = is_gradual_slope(v_map, current, next, t_dir);
    }

    return is_valid;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    file_path = "./input_sample_d10.txt";
    // file_path = "./d10.txt";

    fileOperations<int> fOp(file_path);

    vector<vector<int>> topographical_map;
    fOp.get_num_data_from_file(topographical_map);
    print_vector<int>(topographical_map);

    size_t num_rows = topographical_map.size();
    size_t num_cols = topographical_map[0].size();

    // Store 0s and 9s indexes
    vector<pair<int,int>> trailheads;
    vector<pair<int,int>> endpoints;
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            if (topographical_map[i][j] == 0)
            {
                trailheads.emplace_back(make_pair(i,j));
            }
            else if (topographical_map[i][j] == 9)
            {
                endpoints.emplace_back(make_pair(i,j));
            }
            else
            {
                // Do nothing
            }
        }
    }

    map<pair<int,int>, vector<tuple<int,int,int>>> forward_map_table;
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            int no_of_valid_next_steps = 0;
            for (int k = 0; k < e_NUM_DIR; k++)
            {
                pair<int,int> next;
                if (is_valid_next_step(topographical_map, num_rows, num_cols, make_pair(i,j), next, (directions_t) k, e_FORWARD))
                {
                    forward_map_table[make_pair(i,j)].emplace_back(make_tuple(next.first, next.second, k));
                    no_of_valid_next_steps++;
                }
            }

            if (no_of_valid_next_steps == 0)
            {
                forward_map_table[make_pair(i,j)].emplace_back(make_tuple(-1,-1,-1));
            }
        }
    }

    cout << "Forward map table: " << endl;
    print_map(forward_map_table);

    map<pair<int,int>, vector<tuple<int,int,int>>> reverse_map_table;
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            int no_of_valid_next_steps = 0;
            for (int k = 0; k < e_NUM_DIR; k++)
            {
                pair<int,int> next;
                if (is_valid_next_step(topographical_map, num_rows, num_cols, make_pair(i,j), next, (directions_t) k, e_REVERSE))
                {
                    reverse_map_table[make_pair(i,j)].emplace_back(make_tuple(next.first, next.second, k));
                    no_of_valid_next_steps++;
                }
            }

            if (no_of_valid_next_steps == 0)
            {
                reverse_map_table[make_pair(i,j)].emplace_back(make_tuple(-1,-1,-1));
            }
        }
    }

    cout << "Reverse map table: " << endl;
    print_map(reverse_map_table);
    
    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}