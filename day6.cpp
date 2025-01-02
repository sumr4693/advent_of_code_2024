#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

#include "file_operations.hpp"

using namespace std;

typedef enum {
    e_UP = 1,
    e_DOWN = 2,
    e_LEFT = 3,
    e_RIGHT = 4
} navigation_direction_t;

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

pair<int,int> find_starting_position(vector<vector<char>> const map_2d, char appearance)
{
    pair<int,int> pos = make_pair(-1,-1);

    for (int i = 0; i < map_2d.size(); i++)
    {
        for (int j = 0; j < map_2d[i].size(); j++)
        {
            if(map_2d[i][j] == appearance)
            {
                pos = make_pair(i,j);
                break;
            }
        }
    }

    return pos;
}

bool check_for_obstacle(vector<vector<char>> const map_2d, pair<int,int> const current_idx, navigation_direction_t const current_direction, char const obs)
{
    pair<int, int> next_idx;
    bool is_obs_detected = false;

    switch(current_direction)
    {
        case e_UP:
            next_idx = make_pair (current_idx.first - 1, current_idx.second);
            break;

        case e_DOWN:
            next_idx = make_pair (current_idx.first + 1, current_idx.second);
            break;

        case e_LEFT:
            next_idx = make_pair (current_idx.first, current_idx.second - 1);
            break;

        case e_RIGHT:
            next_idx = make_pair (current_idx.first, current_idx.second + 1);
            break;

        default:
            cout << "Choose one of the four directions: e_UP, e_DOWN, e_LEFT, e_RIGHT" << endl;
            break;
    }

    if (map_2d[next_idx.first][next_idx.second] == obs)
    {
        is_obs_detected = true;
    }

    return is_obs_detected;
}

navigation_direction_t turn_right(navigation_direction_t const current_direction)
{
    navigation_direction_t next_direction;

    switch (current_direction)
    {
    case e_UP:
        next_direction = e_RIGHT;
        break;

    case e_RIGHT:
        next_direction = e_DOWN;
        break;

    case e_DOWN:
        next_direction = e_LEFT;
        break;

    case e_LEFT:
        next_direction = e_UP;
        break;

    default:
        break;
    }

    return next_direction;
}

bool navigate_map(vector<vector<char>> &map_2d, vector<pair<int,int>> &travel_path, vector<navigation_direction_t> &travel_directions, navigation_direction_t current_direction)
{
    pair<int, int> current_idx = travel_path.back();
    pair<int, int> next_idx;

    bool is_inside_edges = true;

    map_2d[current_idx.first][current_idx.second] = '.';

    switch(current_direction)
    {
        case e_UP:
            next_idx = make_pair (current_idx.first - 1, current_idx.second);
            map_2d[next_idx.first][next_idx.second] = '^';
            break;

        case e_DOWN:
            next_idx = make_pair (current_idx.first + 1, current_idx.second);
            map_2d[next_idx.first][next_idx.second] = 'v';
            break;

        case e_LEFT:
            next_idx = make_pair (current_idx.first, current_idx.second - 1);
            map_2d[next_idx.first][next_idx.second] = '<';
            break;

        case e_RIGHT:
            next_idx = make_pair (current_idx.first, current_idx.second + 1);
            map_2d[next_idx.first][next_idx.second] = '>';
            break;

        default:
            cout << "Choose one of the four directions: e_UP, e_DOWN, e_LEFT, e_RIGHT" << endl;
            break;
    }

    if ((next_idx.first == 0 || next_idx.first == map_2d.size() - 1) || 
        (next_idx.second == 0 || next_idx.second == map_2d[0].size() - 1))
    {
        is_inside_edges = false;
    }

    travel_path.push_back(next_idx);
    travel_directions.push_back(current_direction);

    return is_inside_edges;
}

int count_distinct_positions(vector<vector<char>> const map_2d, char presence)
{
    int position_counts = 0;

    for (int i = 0; i < map_2d.size(); i++)
    {
        for (int j = 0; j < map_2d[i].size(); j++)
        {
            if(map_2d[i][j] == presence)
            {
                position_counts++;
            }
        }
    }

    return position_counts;
}

void populate_map_travelled(vector<vector<char>> &map_2d, vector<pair<int,int>> &idxs_2d, char populate_ch)
{
    for (int i = 0; i < idxs_2d.size(); i++)
    {
        map_2d[idxs_2d[i].first][idxs_2d[i].second] = populate_ch;
    }
}

void display_map(vector<vector<char>> map_2d)
{
    cout << "Printing map: " << endl;
    for (int i = 0; i < map_2d.size(); i++)
    {
        for (int j = 0; j < map_2d[i].size(); j++)
        {
            cout << map_2d[i][j];
        }
        cout << endl;
    }
}

int main()
{
    string file_path;
    // file_path = "./input_sample_d6.txt";
    file_path = "./d6.txt";

    fileOperations<char> fOp(file_path);

    vector<vector<char>> map_2d;
    fOp.get_char_data_from_file(map_2d);

    char robot_appearance = '^';
    char obs = '#';
    char record_robot_movement = 'X';
    navigation_direction_t current_direction = e_UP;

    pair<int,int> current_index = find_starting_position(map_2d, robot_appearance);

    vector<pair<int,int>> travel_path;
    travel_path.push_back(current_index);

    vector<navigation_direction_t> travel_directions;
    travel_directions.push_back(current_direction);

    bool is_inside_edges = true;

    while (is_inside_edges != false)
    {
        while(check_for_obstacle(map_2d, current_index, current_direction, obs))
        {
            current_direction = turn_right(current_direction);
        }

        is_inside_edges = navigate_map(map_2d, travel_path, travel_directions, current_direction);

        current_index = travel_path.back();
        current_direction = travel_directions.back();
    }

    cout << "Robot has reached to one of the edges: " << travel_path.back().first << "," << travel_path.back().second << endl;

    populate_map_travelled(map_2d, travel_path, record_robot_movement);
    // display_map(map_2d);

    int distinct_position_counts = count_distinct_positions(map_2d, record_robot_movement);
    cout << "Part 1: " << distinct_position_counts << endl;

    return 0;
}