#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <tuple>

#include <chrono>

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
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d6.txt";
    file_path = "./d6.txt";

    fileOperations<char> fOp(file_path);

    vector<vector<char>> map_2d;
    fOp.get_char_data_from_file(map_2d);

    char guard_appearance = '^';
    char record_guard_movement = 'X';
    char default_obstacle = '#';
    navigation_direction_t current_direction = e_UP;

    int direction_arr[e_NUM_DIR][2] = {{-1, 0},
                                       {0, 1},
                                       {1, 0},
                                       {0, -1}};

    pair<int,int> start_index = find_starting_position(map_2d, guard_appearance);
    pair<int,int> current_index = start_index;

    vector<pair<int,int>> travel_path;
    travel_path.push_back(current_index);

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

    size_t map_row_size = map_2d.size();
    size_t map_col_size = map_2d[0].size();
    size_t travel_path_length = travel_path.size();

    vector<vector<char>> obs_map (map_row_size, vector<char>(map_col_size, '.'));

    bool fancy_part2_enabled = false;

    if (fancy_part2_enabled)
    {
        // Create and fill in the jump table values for all directions to facilitate teleporting
        map<tuple<int,int,int>, tuple<int,int,int>> jump_table;
        for (int i = 0; i < map_row_size; i++)
        {
            // "Don't care" position values
            tuple<int,int,int> jump_from_left_to_up = make_tuple(-1, -1, e_UP);
            for (int j = 0; j < map_col_size; j++)
            {
                if (map_2d[i][j] == default_obstacle)
                {
                    jump_from_left_to_up = make_tuple(i, j+1, e_UP);
                }
                jump_table.emplace(make_tuple(i, j, e_LEFT), jump_from_left_to_up);
            }

            // "Don't care" position values
            tuple<int,int,int> jump_from_right_to_down = make_tuple(-1, -1, e_DOWN);
            for (int j = map_col_size - 1; j > 0; j--)
            {
                if (map_2d[i][j] == default_obstacle)
                {
                    jump_from_right_to_down = make_tuple(i, j-1, e_DOWN);
                }
                jump_table.emplace(make_tuple(i, j, e_RIGHT), jump_from_right_to_down);
            }
        }

        for (int j = 0; j < map_col_size; j++)
        {
            // "Don't care" position values
            tuple<int,int,int> jump_from_up_to_right = make_tuple(-1, -1, e_RIGHT);
            for (int i = 0; i < map_row_size; i++)
            {
                if (map_2d[i][j] == default_obstacle)
                {
                    jump_from_up_to_right = make_tuple(i+1, j, e_RIGHT);
                }
                jump_table.emplace(make_tuple(i, j, e_UP), jump_from_up_to_right);
            }

            // "Don't care" position values
            tuple<int,int,int> jump_from_down_to_left = make_tuple(-1, -1, e_LEFT);
            for (int i = map_row_size - 1; i > 0; i--)
            {
                if (map_2d[i][j] == default_obstacle)
                {
                    jump_from_down_to_left = make_tuple(i-1, j, e_LEFT);
                }
                jump_table.emplace(make_tuple(i, j, e_DOWN), jump_from_down_to_left);
            }
        }

        for (int i = 1; i < travel_path_length; i++)
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
                // If current position is neither in the row nor in the column of that of the manual obstacle
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

                    if (find(loop_path.begin(), loop_path.end(), current_pos) != loop_path.end())
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
    }
    else
    {
        for (int i = 1; i < travel_path_length; i++)
        {
            vector<vector<char>> copy_map_2d = map_2d;
            bool is_valid_obs_position = true;
            bool is_guard_looping = false;

            add_obstacle(copy_map_2d, travel_path[i], manual_obs);

            vector<array<int,3>> loop_path;

            // Include starting position in the loop path and increment the visits to 1 for the starting pos index
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

                if ((current_index.first == 0 || current_index.first == map_2d.size() - 1) ||
                    (current_index.second == 0 || current_index.second == map_2d[0].size() - 1))
                {
                    is_inside_edges = false;
                    is_valid_obs_position = false;
                }
                else
                {
                    current_pos[0] = current_index.first;
                    current_pos[1] = current_index.second;
                    current_pos[2] = (int) current_direction;

                    if (find(loop_path.begin(), loop_path.end(), current_pos) != loop_path.end())
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
    }

    cout << "Part 2: " << no_of_obstacle_positions << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}