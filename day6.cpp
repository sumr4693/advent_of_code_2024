#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <tuple>

#include "file_operations.hpp"

using namespace std;

typedef enum {
    e_UP,
    e_RIGHT,
    e_DOWN,
    e_LEFT,
    e_NUM_DIR = 4
} navigation_direction_t;

typedef struct {
    pair<int,int> index;
    navigation_direction_t direction;
} guard_path_t;

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

template<typename T>
int find_index(vector<T> v, T ele)
{
    int index = -1;
    auto it = find(v.begin(), v.end(), ele);
    index = it - v.begin();

    return index;
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

bool check_for_obstacle(vector<vector<char>> const map_2d, pair<int,int> const current_idx, navigation_direction_t const current_direction, vector<char> const obs_vec, char &obs)
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

    for (int i = 0; i < obs_vec.size(); i++)
    {
        if (map_2d[next_idx.first][next_idx.second] == obs_vec[i])
        {
            is_obs_detected = true;
            obs = obs_vec[i];
            break;
        }
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

void populate_map_travelled(vector<vector<char>> &map_2d, vector<pair<int,int>> const path_2d, char populate_ch)
{
    for (int i = 0; i < path_2d.size(); i++)
    {
        map_2d[path_2d[i].first][path_2d[i].second] = populate_ch;
    }
}

void add_obstacle(vector<vector<char>> &map_2d, pair<int,int> idx, char manual_obs)
{
    map_2d[idx.first][idx.second] = manual_obs;
}

bool predict_next_step(vector<vector<char>> const map_2d, pair<int,int> current_idx, pair<int,int> &next_idx, navigation_direction_t current_direction)
{
    bool is_valid_next_step = true;

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

    if ((next_idx.first == 0 || next_idx.first == map_2d.size() - 1) || 
        (next_idx.second == 0 || next_idx.second == map_2d[0].size() - 1))
    {
        is_valid_next_step = false;
    }

    return is_valid_next_step;
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

    vector<char> obs_vec;
    char obs;

    char guard_appearance = '^';
    char record_guard_movement = 'X';
    char default_obstacle = '#';
    navigation_direction_t current_direction = e_UP;

    int direction_arr[e_NUM_DIR][2] = {{-1, 0},
                                       {0, 1},
                                       {1, 0},
                                       {0, -1}};

    obs_vec.push_back(default_obstacle);

    pair<int,int> start_index = find_starting_position(map_2d, guard_appearance);
    pair<int,int> current_index = start_index;

    vector<pair<int,int>> travel_path;

    travel_path.push_back(current_index);

    vector<navigation_direction_t> travel_directions;
    travel_directions.push_back(current_direction);

    bool is_inside_edges = true;

    pair<int,int> next_index;

    while (is_inside_edges != false)
    {
        next_index.first = current_index.first + direction_arr[current_direction][0];
        next_index.second = current_index.second + direction_arr[current_direction][1];

        if(map_2d[next_index.first][next_index.second] == default_obstacle)
        {
            current_direction = (navigation_direction_t) ((current_direction + 1) % e_NUM_DIR);
        }
        else
        {
            current_index = next_index;
        }

        if ((current_index.first == 0 || current_index.first == map_2d.size() - 1) ||
            (current_index.second == 0 || current_index.second == map_2d[0].size() - 1))
            {
                is_inside_edges = false;
            }

        travel_path.push_back(current_index);
    }

    cout << "Robot has reached to one of the edges: " << travel_path.back().first << "," << travel_path.back().second << endl;

    populate_map_travelled(map_2d, travel_path, record_guard_movement);
    // display_map(map_2d);

    int distinct_position_counts = count_distinct_positions(map_2d, record_guard_movement);
    cout << "Part 1: " << distinct_position_counts << endl;

    char manual_obs = 'O';
    int no_of_obstacle_positions = 0;

    obs_vec.push_back(manual_obs);

    vector<vector<char>> obs_map (map_2d.size(), vector<char>(map_2d[0].size(), '.'));

    // Create and fill in the jump table values for all directions to facilitate teleporting
    map<tuple<int,int,int>, tuple<int,int,int>> jump_table;

    for (int i = 0; i < map_2d.size(); i++)
    {
        // "Don't care" position values
        tuple<int,int,int> jump_from_left_to_up = make_tuple(-1, -1, e_UP);
        for (int j = 0; j < map_2d[0].size(); j++)
        {
            if (map_2d[i][j] == default_obstacle)
            {
                jump_from_left_to_up = make_tuple(i, j+1, e_UP);
            }
            jump_table.emplace(make_tuple(i, j, e_LEFT), jump_from_left_to_up);
        }

        // "Don't care" position values
        tuple<int,int,int> jump_from_right_to_down = make_tuple(-1, -1, e_DOWN);
        for (int j = map_2d[0].size() - 1; j > 0; j--)
        {
            if (map_2d[i][j] == default_obstacle)
            {
                jump_from_right_to_down = make_tuple(i, j-1, e_DOWN);
            }
            jump_table.emplace(make_tuple(i, j, e_RIGHT), jump_from_right_to_down);
        }
    }

    for (int j = 0; j < map_2d[0].size(); j++)
    {
        // "Don't care" position values
        tuple<int,int,int> jump_from_up_to_right = make_tuple(-1, -1, e_RIGHT);
        for (int i = 0; i < map_2d.size(); i++)
        {
            if (map_2d[i][j] == default_obstacle)
            {
                jump_from_up_to_right = make_tuple(i+1, j, e_RIGHT);
            }
            jump_table.emplace(make_tuple(i, j, e_UP), jump_from_up_to_right);
        }

        // "Don't care" position values
        tuple<int,int,int> jump_from_down_to_left = make_tuple(-1, -1, e_LEFT);
        for (int i = map_2d.size() - 1; i > 0; i--)
        {
            if (map_2d[i][j] == default_obstacle)
            {
                jump_from_down_to_left = make_tuple(i-1, j, e_LEFT);
            }
            jump_table.emplace(make_tuple(i, j, e_DOWN), jump_from_down_to_left);
        }
    }

    for (int i = 1; i < travel_path.size(); i++)
    {
        vector<vector<char>> copy_map_2d = map_2d;
        bool is_valid_obs_position = true;
        bool is_guard_looping = false;

        add_obstacle(copy_map_2d, travel_path[i], manual_obs);

        vector<array<int,3>> loop_path;

        array<int,3> current_pos;

        current_pos[0] = travel_path[0].first;
        current_pos[1] = travel_path[0].second;
        current_pos[2] = (int) e_UP;
        loop_path.push_back(current_pos);

        current_index = travel_path[0];
        current_direction = e_UP;

        is_inside_edges = true;

        while (is_inside_edges == true && is_guard_looping == false)
        {
            if (current_index.first != travel_path[i].first && current_index.second != travel_path[i].second)
            {
                tuple<int,int,int> current_pos_tup, next_pos_tup;
                current_pos_tup = make_tuple(current_index.first, current_index.second, current_direction);
                next_pos_tup = jump_table[current_pos_tup];

                next_index.first = get<0>(next_pos_tup);
                next_index.second = get<1>(next_pos_tup);
                current_direction = (navigation_direction_t) get<2>(next_pos_tup);

                current_index = next_index;
            }
            else
            {
                next_index.first = current_index.first + direction_arr[current_direction][0];
                next_index.second = current_index.second + direction_arr[current_direction][1];

                if(copy_map_2d[next_index.first][next_index.second] == default_obstacle ||
                   copy_map_2d[next_index.first][next_index.second] == manual_obs)
                {
                    current_direction = (navigation_direction_t) ((current_direction + 1) % e_NUM_DIR);
                }
                else
                {
                    current_index = next_index;
                }
            }

            if ((current_index.first <= 0 || current_index.first >= map_2d.size() - 1) ||
                (current_index.second <= 0 || current_index.second >= map_2d[0].size() - 1))
            {
                is_inside_edges = false;
                is_valid_obs_position = false;
            }
            else
            {
                current_pos[0] = current_index.first;
                current_pos[1] = current_index.second;
                current_pos[2] = (int) current_direction;

                if (count(loop_path.begin(), loop_path.end(), current_pos) > 0)
                {
                    is_guard_looping = true;
                    break;
                }

                loop_path.push_back(current_pos);
            }
        }

        if ((is_valid_obs_position) && (obs_map[travel_path[i].first][travel_path[i].second] != manual_obs))
        {
            no_of_obstacle_positions++;
            add_obstacle(obs_map, travel_path[i], manual_obs);
        }
    }

    cout << "Part 2: " << no_of_obstacle_positions << endl;

    return 0;
}