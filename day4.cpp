#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "file_operations.hpp"

using namespace std;

void print_vector(vector<char> v)
{
    cout << "Printing elements in 1D vector: " << endl;

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i];
    }

    cout << endl;
}

int get_pattern_counts_part1(vector<vector<char>> input_vector, vector<char> pattern, vector<char> reverse_pattern)
{
    int result = 0;

    int pattern_size = pattern.size();

    // Horizontal match
    for (int i = 0; i < input_vector.size(); i++)
    {
        for (int j = 0; j < input_vector[i].size() - pattern_size + 1;)
        {
            int k = 0;

            vector<char> temp_pattern;

            if (input_vector[i][j] == pattern[k])
            {
                temp_pattern = pattern;
                k++;
            }
            else if (input_vector[i][j] == reverse_pattern[k])
            {
                temp_pattern = reverse_pattern;
                k++;
            }
            else
            {
                // Do nothing
            }

            j++;

            while (temp_pattern.size() != 0 && (k < pattern_size))
            {
                if (input_vector[i][j] == temp_pattern[k])
                {
                    j++;
                    k++;
                }
                else
                {
                    break;
                }
            }

            if (k == pattern_size)
            {
                result++;
                // cout << "Row: " << i << endl;
                // print_vector(temp_pattern);
                // The last letter could be the starting letter of the next matching pattern, therefore decrementing the index by 1
                j--;
            }
        }
    }

    // Vertical match
    // For every element in a row
    for (int j = 0; j < input_vector[0].size(); j++)
    {
        for (int i = 0; i < input_vector.size() - pattern_size + 1;)
        {
            int k = 0;

            vector<char> temp_pattern;

            if (input_vector[i][j] == pattern[k])
            {
                temp_pattern = pattern;
                k++;
            }
            else if (input_vector[i][j] == reverse_pattern[k])
            {
                temp_pattern = reverse_pattern;
                k++;
            }
            else
            {
                // Do nothing
            }

            i++;

            while (temp_pattern.size() != 0 && (k < pattern_size))
            {
                if (input_vector[i][j] == temp_pattern[k])
                {
                    i++;
                    k++;
                }
                else
                {
                    break;
                }
            }

            if (k == pattern_size)
            {
                result++;
                // cout << "Column: " << j << endl;
                // print_vector(temp_pattern);
                // The last letter could be the starting letter of the next matching pattern, therefore decrementing the index by 1
                i--;
            }
        }
    }

    // Diagonal match
    for (int i = 0; i < input_vector.size() - pattern_size + 1; i++)
    {
        for (int j = 0; j < input_vector[i].size(); j++)
        {
            if (j + pattern_size - 1 < input_vector[0].size())
            {
                int temp_i = i;
                int temp_j = j;
                int k = 0;

                vector<char> temp_pattern;

                if (input_vector[temp_i][temp_j] == pattern[k])
                {
                    temp_pattern = pattern;
                    k++;
                }
                else if (input_vector[temp_i][temp_j] == reverse_pattern[k])
                {
                    temp_pattern = reverse_pattern;
                    k++;
                }
                else
                {
                    // Do nothing
                }

                temp_i++;
                temp_j++;    

                while (temp_pattern.size() != 0 && (k < pattern_size))
                {
                    if (input_vector[temp_i][temp_j] == temp_pattern[k])
                    {
                        temp_i++;
                        temp_j++;
                        k++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (k == pattern_size)
                {
                    result++;
                    // cout << "Right diagonal pattern. Start Row, Column: " << i << ", " << j << endl;
                    // print_vector(temp_pattern);
                }
            }

            if (j - pattern_size + 1 >= 0)
            {
                int temp_i = i;
                int temp_j = j;
                int k = 0;

                vector<char> temp_pattern;

                if (input_vector[temp_i][temp_j] == pattern[k])
                {
                    temp_pattern = pattern;
                    k++;
                }
                else if (input_vector[temp_i][temp_j] == reverse_pattern[k])
                {
                    temp_pattern = reverse_pattern;
                    k++;
                }
                else
                {
                    // Do nothing
                }

                temp_i++;
                temp_j--;    

                while (temp_pattern.size() != 0 && (k < pattern_size))
                {
                    if (input_vector[temp_i][temp_j] == temp_pattern[k])
                    {
                        temp_i++;
                        temp_j--;
                        k++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (k == pattern_size)
                {
                    result++;
                    // cout << "Left diagonal pattern. Start Row, Column: " << i << ", " << j << endl;
                    // print_vector(temp_pattern);
                }
            }
        }
    }

    return result;
}

int get_pattern_counts_part2(vector<vector<char>> input_vector, vector<char> pattern, vector<char> reverse_pattern)
{
    int result = 0;

    int pattern_size = pattern.size();

    for (int i = 0; i < input_vector.size() - pattern_size + 1; i++)
    {
        for (int j = 0; j < input_vector[0].size() - pattern_size + 1; j++)
        {
            int k = 0;

            vector<char> temp_pattern;

            int temp_i = i;
            int temp_j = j;
            if (input_vector[temp_i][temp_j] == pattern[k])
            {
                temp_pattern = pattern;
                k++;
            }
            else if (input_vector[temp_i][temp_j] == reverse_pattern[k])
            {
                temp_pattern = reverse_pattern;
                k++;
            }
            else
            {
                // Do nothing
            }

            temp_i++;
            temp_j++;

            while (temp_pattern.size() != 0 && (k < pattern_size))
            {
                if (input_vector[temp_i][temp_j] == temp_pattern[k])
                {
                    k++;
                    temp_i++;
                    temp_j++;
                }
                else
                {
                    break;
                }
            }

            if (k == pattern_size)
            {
                temp_i = i;
                temp_j = j + pattern_size - 1;
                k = 0;

                if (input_vector[temp_i][temp_j] == pattern[k])
                {
                    temp_pattern = pattern;
                    k++;
                }
                else if (input_vector[temp_i][temp_j] == reverse_pattern[k])
                {
                    temp_pattern = reverse_pattern;
                    k++;
                }
                else
                {
                    // Do nothing
                }

                temp_i++;
                temp_j--;

                while (temp_pattern.size() != 0 && (k < pattern_size))
                {
                    if (input_vector[temp_i][temp_j] == temp_pattern[k])
                    {
                        k++;
                        temp_i++;
                        temp_j--;
                    }
                    else
                    {
                        break;
                    }
                }

                if (k == pattern_size)
                {
                    result++;
                }
            }
        }
    }

    return result;
}

int main()
{

    // string file_path = "./input_sample_d4p1.txt";
    string file_path = "./d4.txt";

    fileOperations<char> fOp(file_path);

    vector<vector<char>> ch_2d;
    fOp.get_char_data_from_file(ch_2d);

    // fOp.print_vector(ch_2d);

    vector<char> pattern = {'X', 'M', 'A', 'S'};
    vector<char> reverse_pattern = {'S', 'A', 'M', 'X'};

    int result;

    result = get_pattern_counts_part1(ch_2d, pattern, reverse_pattern);

    cout << "Result Part 1: " << result << endl;

    // Get pattern counts part 2
    vector<char> pattern2 = {'M', 'A', 'S'};
    vector<char> reverse_pattern2 = {'S', 'A', 'M'};
    int result2 = get_pattern_counts_part2(ch_2d, pattern2, reverse_pattern2);

    cout << "Result Part 2: " << result2 << endl;

    return 0;
}