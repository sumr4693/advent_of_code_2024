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

int find_trailhead_scores(pair<int,int> trailhead, vector<pair<int,int>> endpoints, map<pair<int,int>, vector<tuple<int,int,int>>> map_table)
{
    int score = 0;
    size_t total_endpoints = endpoints.size();

    for (int i = 0; i < total_endpoints; i++)
    {
        pair<int,int> current_index = trailhead;
        pair<int,int> end_index = endpoints[i];
        tuple<int,int,int> next_index_tuple = map_table[current_index][0];
        // map<pair<int,int>, int> distance_map;
        // vector<pair<int,int>> traverse_path;

        while ((current_index.first != end_index.first) && (current_index.second != end_index.second))
        {
            if (map_table[current_index].size() > 1)
            {
                pair<int,int> next_index, potential_current_index, prev_next_index;
                int distance, prev_distance;
                // Pick the minimum distance next index
                for (int j = 0; j < map_table[current_index].size(); j++)
                {
                    next_index_tuple = map_table[current_index][j];
                    next_index = make_pair(get<0>(next_index_tuple), get<1>(next_index_tuple));
                    distance = calculate_steps(next_index, end_index);

                    if (j > 0)
                    {
                        potential_current_index = (distance < prev_distance) ? next_index : prev_next_index;
                    }
                    prev_distance = distance;
                    prev_next_index = next_index;
                }
                current_index = potential_current_index;
            }
            else
            {
                current_index = make_pair(get<0>(map_table[current_index][0]), get<1>(map_table[current_index][0]));
            }

            if (map_table.count(current_index) == 0)
            {
                cout << "endpoint: (" << endpoints[i].first << "," << endpoints[i].second << ")" << endl;
                cout << "current index: (" << current_index.first << "," << current_index.second << ")" << endl;
                break;
            }
        }
        
        if ((current_index.first == end_index.first) && (current_index.second == end_index.second))
        {
            score++;
        }
    }

    return score;
    
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
    // print_vector<int>(topographical_map);

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

            // if (no_of_valid_next_steps == 0)
            // {
            //     forward_map_table[make_pair(i,j)].emplace_back(make_tuple(-1,-1,-1));
            // }
        }
    }

    // cout << "Forward map table before cleaning: " << endl;
    // print_map(forward_map_table);

    for (auto itr = forward_map_table.begin(); itr != forward_map_table.end(); ++itr)
    {
        for (auto path : itr->second)
        {
            pair<int,int> next = make_pair(get<0>(path), get<1>(path));
            if ((forward_map_table.count(next) == 0) && (topographical_map[next.first][next.second] != 9))
            {
                forward_map_table[itr->first].erase(remove(forward_map_table[itr->first].begin(), \
                    forward_map_table[itr->first].end(), path), forward_map_table[itr->first].end());
            }

            if (forward_map_table[itr->first].size() == 0)
            {
                forward_map_table.erase(itr->first);
                itr = forward_map_table.begin();
                itr--;
                break;
            }
        }
    }

    int total_scores = 0;
    for (int i = 0; i < trailheads.size(); i++)
    {
        total_scores += find_trailhead_scores(trailheads[i], endpoints, forward_map_table);
    }

    cout << "Total scores: " << total_scores << endl;

    // cout << "Forward map table after cleaning: " << endl;
    // print_map(forward_map_table);

    // map<pair<int,int>, vector<tuple<int,int,int>>> reverse_map_table;
    // for (int i = 0; i < num_rows; i++)
    // {
    //     for (int j = 0; j < num_cols; j++)
    //     {
    //         int no_of_valid_next_steps = 0;
    //         for (int k = 0; k < e_NUM_DIR; k++)
    //         {
    //             pair<int,int> next;
    //             if (is_valid_next_step(topographical_map, num_rows, num_cols, make_pair(i,j), next, (directions_t) k, e_REVERSE))
    //             {
    //                 reverse_map_table[make_pair(i,j)].emplace_back(make_tuple(next.first, next.second, k));
    //                 no_of_valid_next_steps++;
    //             }
    //         }

    //         // if (no_of_valid_next_steps == 0)
    //         // {
    //         //     reverse_map_table[make_pair(i,j)].emplace_back(make_tuple(-1,-1,-1));
    //         // }
    //     }
    // }

    // cout << "Reverse map table: " << endl;
    // print_map(reverse_map_table);
    
    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}
