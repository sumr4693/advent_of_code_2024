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

void extract_data_from_disk_map(vector<string> disk_map, vector<int>& file_blocks, vector<int>& free_spaces)
{
    size_t disk_map_size = disk_map.size();

    for (int i = 0; i < disk_map_size; i++)
    {
        string disk_map_row = disk_map[i];

        size_t row_size = disk_map_row.size();

        for (int j = 0; j < row_size; j++)
        {
            if (j % 2 == 0)
            {
                file_blocks.emplace_back((int) stoi(disk_map_row.substr(j,1)));
            }
            else
            {
                free_spaces.emplace_back((int) stoi(disk_map_row.substr(j,1)));
            }
        }
    }
}

void expand_data(vector<int> const file_blocks, vector<int> const free_spaces, vector<string>& formatted_data)
{
    size_t no_of_file_blocks = file_blocks.size();

    for (int i = 0; i < no_of_file_blocks; i++)
    {
        for (int j = 0; j < file_blocks[i]; j++)
        {
            formatted_data.push_back(to_string(i));
        }
        if (i < no_of_file_blocks - 1)
        {
            for (int k = 0; k < free_spaces[i]; k++)
            {
                formatted_data.push_back(".");
            }
        }
    }
}

void apply_compacting_process(vector<string>& compacted_data)
{
    bool is_compaction_over = false;
    string free_space_str = ".";

    auto idx = compacted_data.size() - 1;
    while (!is_compaction_over)
    {
        while (compacted_data[idx] == free_space_str)
        {
            idx--;
        }
        auto it = find(compacted_data.begin(), compacted_data.end(), free_space_str);
        long insert_index = it - compacted_data.begin();
        long last_number_index = idx;

        if (insert_index < last_number_index)
        {
            compacted_data[insert_index] = compacted_data[idx];
            compacted_data[last_number_index] = free_space_str;
        }
        else
        {
            is_compaction_over = true;
            break;
        }
    }
}

long long calculate_checksum(vector<string> const compacted_data)
{
    string free_space_str = ".";
    long long checksum = 0;

    auto it = find(compacted_data.begin(), compacted_data.end(), free_space_str);
    long data_length = it - compacted_data.begin();

    cout << "Data lengths: " << compacted_data.size() << " " << data_length << endl;

    for (long long i = 0; i < data_length; i++)
    {
        checksum += (long long) (i * stoll(compacted_data[i])); 
    }

    return checksum;
}

int main()
{
    auto time_start = chrono::high_resolution_clock::now();

    string file_path;
    // file_path = "./input_sample_d9.txt";
    file_path = "./d9.txt";

    fileOperations<string> fOp(file_path);
    vector<string> disk_map;

    fOp.get_data_from_file(disk_map);
    // print_vector<string>(disk_map);
    cout << "Original disk map length: " << disk_map[0].size() << endl;

    vector<int> file_blocks;
    vector<int> free_spaces;
    extract_data_from_disk_map(disk_map, file_blocks, free_spaces);
    cout << "File blocks length: " << file_blocks.size() << endl;
    cout << "Free spaces length: " << free_spaces.size() << endl;

    vector<string> formatted_data;
    expand_data(file_blocks, free_spaces, formatted_data);
    cout << "Formatted data length: " << formatted_data.size() << endl;
    fOp.write_to_file(formatted_data, "./d9_format_data.txt");

    vector<string> compacted_data = formatted_data;
    apply_compacting_process(compacted_data);
    fOp.write_to_file(compacted_data, "./d9_compacted_data.txt");

    long long checksum = 0;
    checksum = calculate_checksum(compacted_data);

    cout << "Checksum: " << checksum << endl;

    auto time_end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

    cout << "Execution time: " << duration << " ms" << endl;

    return 0;   
}