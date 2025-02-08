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

long long calculate_sides(const vector<pair<int,int>>& pairs, size_t area)
{
    long long sides = 4;
    vector<vector<pair<int,int>>> region_table;
    vector<vector<pair<int,int>>> surface;

    if (area > 2)
    {
        vector<pair<int,int>> row_pairs;
        row_pairs.push_back(pairs[0]);
        for (int i = 1; i < area; i++)
        {
            if (pairs[i].first != row_pairs.back().first)
            {
                region_table.push_back(row_pairs);
                row_pairs.clear();
                row_pairs.push_back(pairs[i]);
            }
            else
            {
                row_pairs.push_back(pairs[i]);
            }
        }
        region_table.push_back(row_pairs);

        size_t table_rows = region_table.size();

        for (int i = 0; i < table_rows; i++)
        {
            size_t row_size = region_table[i].size();

            if (row_size <= 2)
            {
                surface.push_back(region_table[i]);
            }
            else
            {
                vector<pair<int,int>> endpoints;
                endpoints.push_back(region_table[i][0]);
                endpoints.push_back(region_table[i][row_size - 1]);

                surface.push_back(endpoints);
            }
        }

        if ((surface.size() != 1) && (surface.size() != area))
        {
            typedef enum
            {
                e_TOP_DOWN = 1,
                e_BOTTOM_UP = -1,
                e_TRAVERSAL_TYPES
            } traversal_type_t;

            traversal_type_t traversal_type = e_TOP_DOWN;

            // int i = 0, j = 0;
            // pair<int,int> start_index = surface[i][j];
            // pair<int,int> previous_index = start_index;
            // pair<int,int> current_index = make_pair(-1,-1);
            // bool dir_change = false;
            // sides++;
            // while ((current_index.first != start_index.first) || (current_index.second != start_index.second))
            // {
            //     if (i == surface.size() - 1)
            //     {
            //         traversal_type = e_BOTTOM_UP;
            //         dir_change = true;
            //     }

            //     if ((i == 0) || (i == surface.size() - 1))
            //     {
            //         if (surface[i].size() == 1)
            //         {
            //             i += (int) traversal_type;
            //         }
            //         else
            //         {
            //             j += (int) traversal_type;
            //         }
            //     }
            //     else
            //     {
            //         i += (int) traversal_type;

            //         if (traversal_type == e_TOP_DOWN)
            //         {
            //             j = surface[i].size() - 1;
            //         }
            //         else if (traversal_type == e_BOTTOM_UP)
            //         {
            //             j = 0;
            //         }
            //         else
            //         {
            //             // Do nothing
            //         }
            //     }

            //     current_index = surface[i][j];

            //     if ((current_index.first != previous_index.first) && (current_index.second != previous_index.second))
            //     {
            //         sides += 2;
            //     }
            //     else
            //     {
            //         if (current_index.first != previous_index.first)
            //         {
            //             sides += 1;
            //         }
            //         else
            //         {

            //         }
            //     }

            //     previous_index = current_index;

            // }
        
            int i = 0, j = 0;
            pair<int,int> start_index = surface[i][0];
            pair<int,int> previous_index = surface[i][surface[i].size() - 1];
            pair<int,int> current_index = make_pair(-1,-1);

            while ((current_index.first != start_index.first) || (current_index.second != start_index.second))
            {
                if (i == surface.size() - 1)
                {
                    traversal_type = e_BOTTOM_UP;
                    previous_index = surface[i][0];
                }

                i += (int) traversal_type;

                if (traversal_type == e_TOP_DOWN)
                {
                    j = surface[i].size() - 1;
                }
                else if (traversal_type == e_BOTTOM_UP)
                {
                    j = 0;
                }
                else
                {
                    // Do nothing
                }

                current_index = surface[i][j];

                if ((current_index.first != previous_index.first) && (current_index.second != previous_index.second))
                {
                    sides += 2;
                }
                else
                {
                    if ((abs(current_index.first - previous_index.first) > 1) || (abs(current_index.second - previous_index.second) > 1))
                    {
                        sides += 2;
                    }
                }

                previous_index = current_index;
            }

        }
    }
    // print_vector<int>(region_table);
    // print_vector<int>(corners);
    cout << sides << endl;

    return sides;
}

long long calculate_discounted_fencing_price(const vector<vector<pair<int,int>>>& garden_regions)
{
    long long discounted_fencing_price = 0;
    vector<vector<pair<int,int>>> garden_regions_sorted = garden_regions;
    size_t total_regions = garden_regions_sorted.size();
    vector<vector<long>> sides;

    for (int i = 0; i < total_regions; i++)
    {
        sort(garden_regions_sorted[i].begin(), garden_regions_sorted[i].end());
    }

    // print_vector<int>(garden_regions_sorted);
    for (int i = 0; i < total_regions; i++)
    {
        size_t area = garden_regions_sorted[i].size();
        discounted_fencing_price += (calculate_sides(garden_regions_sorted[i], area) * area);
    }

    return discounted_fencing_price;
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
    long long initial_fencing_price = 0;
    long long discounted_fencing_price = 0;

    fOp.get_char_data_from_file(garden_plots);
    find_garden_types(garden_plots, garden_types);

    size_t dimension = garden_plots.size();
    vector<vector<int>> garden_perimeters(dimension, vector<int>(dimension, 0));
    calculate_garden_perimeters(garden_plots, garden_perimeters);

    find_garden_regions(garden_plots, garden_types, garden_regions);
    initial_fencing_price = calculate_fencing_price(garden_regions, garden_perimeters);

    cout << "Fencing price, part 1: " << initial_fencing_price << endl;

    discounted_fencing_price = calculate_discounted_fencing_price(garden_regions);

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}