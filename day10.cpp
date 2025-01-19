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

void print_map(map<pair<int,int>, vector<pair<int,int>>> myMap)
{
    for(const auto& elem : myMap)
    {
        cout << "(" << elem.first.first << "," << elem.first.second << ")" << ": ";

        for (const auto& p : elem.second)
        {
            cout << "(" << p.first << "," << p.second << ") ";
        }
        cout << endl;
    }
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

int find_trailhead_scores(pair<int,int> trailhead, vector<pair<int,int>> endpoints, map<pair<int,int>, vector<pair<int,int>>> og_map_table, map<pair<int,int>, vector<pair<int,int>>> ic_map_table)
{
    int scores = 0;
    vector<pair<int,int>> valid_vertices;
    vector<pair<int,int>> traverse_stack;
    map<pair<int,int>, bool> visited_table;

    for(auto const& row : og_map_table)
    {
        valid_vertices.push_back(row.first);
    }

    if (og_map_table.count(trailhead) != 0)
    {
        for (int i = 0; i < endpoints.size(); i++)
        {
            pair<int,int> end_index = endpoints[i];
            if (ic_map_table.count(end_index) == 0)
            {
                endpoints.erase(endpoints.begin() + i);
                i--;
            }
        }

        pair<int,int> current_index = trailhead;
        traverse_stack.push_back(current_index);
        pair<int,int> next_index;

        while (traverse_stack.size() != 0)
        {
            current_index = traverse_stack.back();
            visited_table[current_index] = true;
            traverse_stack.pop_back();

            if (count(valid_vertices.begin(), valid_vertices.end(), current_index) == 0)
            {
                if (count(endpoints.begin(), endpoints.end(), current_index) > 0)
                {
                    scores++;
                }
            }

            size_t neighbours = og_map_table[current_index].size();
            for (int i = 0; i < neighbours; i++)
            {
                auto neighbour_index = og_map_table[current_index][i];

                if (visited_table[neighbour_index] == false)
                {
                    traverse_stack.push_back(neighbour_index);
                }
            }
        }
    }

    return scores;
}

int find_trailhead_ratings(pair<int,int> trailhead, vector<pair<int,int>> endpoints, map<pair<int,int>, vector<pair<int,int>>> og_map_table, map<pair<int,int>, vector<pair<int,int>>> ic_map_table)
{
    int ratings = 0;
    vector<pair<int,int>> valid_vertices;
    vector<pair<int,int>> traverse_stack;

    for(auto const& row : og_map_table)
    {
        valid_vertices.push_back(row.first);
    }

    if (og_map_table.count(trailhead) != 0)
    {
        for (int i = 0; i < endpoints.size(); i++)
        {
            pair<int,int> end_index = endpoints[i];
            if (ic_map_table.count(end_index) == 0)
            {
                endpoints.erase(endpoints.begin() + i);
                i--;
            }
        }

        pair<int,int> current_index = trailhead;
        traverse_stack.push_back(current_index);
        pair<int,int> next_index;

        while (traverse_stack.size() != 0)
        {
            current_index = traverse_stack.back();
            traverse_stack.pop_back();

            if (count(valid_vertices.begin(), valid_vertices.end(), current_index) == 0)
            {
                if (count(endpoints.begin(), endpoints.end(), current_index) > 0)
                {
                    ratings++;
                }
            }

            size_t neighbours = og_map_table[current_index].size();
            for (int i = 0; i < neighbours; i++)
            {
                auto neighbour_index = og_map_table[current_index][i];

                traverse_stack.push_back(neighbour_index);
            }
        }
    }

    return ratings;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d10.txt";
    // file_path = "./d10_dbg.txt";
    file_path = "./d10.txt";

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

    map<pair<int,int>, vector<pair<int,int>>> outgoing_map_table, incoming_map_table;
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
                    outgoing_map_table[make_pair(i,j)].push_back(next);
                }
                if (is_valid_next_step(topographical_map, num_rows, num_cols, make_pair(i,j), next, (directions_t) k, e_REVERSE))
                {
                    incoming_map_table[make_pair(i,j)].push_back(next);
                }
            }
        }
    }

    // cout << "Forward map table before cleaning: " << endl;
    // print_map(outgoing_map_table);

    int total_scores = 0;
    for (int i = 0; i < trailheads.size(); i++)
    {
        total_scores += find_trailhead_scores(trailheads[i], endpoints, outgoing_map_table, incoming_map_table);
    }

    cout << "Total scores: " << total_scores << endl;

    int total_ratings = 0;
    for (int i = 0; i < trailheads.size(); i++)
    {
        total_ratings += find_trailhead_ratings(trailheads[i], endpoints, outgoing_map_table, incoming_map_table);
    }
    
    cout << "Total ratings: " << total_ratings << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}
