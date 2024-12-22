#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <map>

#include "file_operations.hpp"

using namespace std;

int multiply_from_string(string s)
{
    int idx1 = s.find('(');
    int idx2 = s.find(',');
    int idx3 = s.find(')');

    int first = stoi(s.substr(idx1+1, idx2-idx1-1));
    int second = stoi(s.substr(idx2+1, idx3-idx2-1));

    return (first * second);
}

int multiply_from_text_plain(vector<string> v)
{
    int result = 0;

    for (int i = 0; i < v.size(); i++)
    {
        result += multiply_from_string(v[i]);
        // cout << "first: " << v[i].substr(idx1+1, idx2-idx1-1) << endl;
        // cout << "second: " << v[i].substr(idx2+1, idx3-idx2-1) << endl;
    }

    return result;
}

int multiply_only_enabled_functions(vector<string> v, vector<int> v_idxs, vector<int> do_idxs, vector<int> dont_idxs)
{
    int mul_i = 0;
    int i = 0;
    int j = 0;

    int result = 0;

    while (mul_i < v.size())
    {
        if (j == dont_idxs.size() - 1)
        {
            if (i == do_idxs.size() - 1)
            {
                if ((v_idxs[mul_i] > dont_idxs[j]) &&
                    (v_idxs[mul_i] < do_idxs[i]))
                {
                    mul_i++;
                }
                else if (((v_idxs[mul_i] > do_idxs[i]) &&
                          (v_idxs[mul_i] < dont_idxs[j])) ||
                         ((v_idxs[mul_i] < do_idxs[i]) &&
                          (v_idxs[mul_i] < dont_idxs[j]) &&
                          (do_idxs[i] < dont_idxs[j])) ||
                         ((v_idxs[mul_i] > do_idxs[i]) &&
                          (v_idxs[mul_i] > dont_idxs[j]) &&
                          (do_idxs[i] > dont_idxs[j])))
                {
                    result += multiply_from_string(v[mul_i]);
                    // cout << "j,i last: " << v[mul_i] << endl;
                    mul_i++;
                }
                else
                {
                    // Nothing
                    mul_i++;
                    // break;
                }
            }
            else
            {
                if ((v_idxs[mul_i] > dont_idxs[j]) &&
                    (v_idxs[mul_i] < do_idxs[i]))
                {
                    mul_i++;
                }
                else if ((v_idxs[mul_i] < dont_idxs[j]) &&
                         (v_idxs[mul_i] < do_idxs[i]))
                {
                    result += multiply_from_string(v[mul_i]);
                    // cout << "j last: " << v[mul_i] << endl;
                    mul_i++;
                }
                else if ((v_idxs[mul_i] > dont_idxs[j]) &&
                         (v_idxs[mul_i] > do_idxs[i]) &&
                         (dont_idxs[j] > do_idxs[i]))
                {
                    i++;
                }
                else
                {
                    result += multiply_from_string(v[mul_i]);
                    mul_i++;
                }
            }
        }
        else if (i == do_idxs.size() - 1)
        {
            if ((v_idxs[mul_i] > dont_idxs[j]) &&
                (v_idxs[mul_i] < do_idxs[i]) &&
                (dont_idxs[j] < do_idxs[i]))
            {
                mul_i++;
            }
            else if ((v_idxs[mul_i] < dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] > do_idxs[i]))
            {
                result += multiply_from_string(v[mul_i]);
                // cout << "i last: " << v[mul_i] << endl;
                mul_i++;
            }
            else if ((v_idxs[mul_i] < dont_idxs[j]) &&
                     (v_idxs[mul_i] < do_idxs[i]) &&
                     (dont_idxs[j] < do_idxs[i]))
            {
                result += multiply_from_string(v[mul_i]);
                // cout << "before dont: " << v[mul_i] << endl;
                mul_i++;
            }
            else if ((v_idxs[mul_i] > dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] < do_idxs[i]))
            {
                j++;
            }
            else if ((v_idxs[mul_i] > dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] > do_idxs[i]))
            {
                mul_i++;
            }
            else
            {
                // Nothing
            }
        }
        else
        {
            if ((v_idxs[mul_i] > dont_idxs[j]) &&
                (v_idxs[mul_i] < do_idxs[i]) &&
                (dont_idxs[j] < do_idxs[i]))
            {
                mul_i++;
            }
            else if ((v_idxs[mul_i] < dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] > do_idxs[i]))
            {
                result += multiply_from_string(v[mul_i]);
                // cout << "between do and dont: " << v[mul_i] << endl;
                mul_i++;
            }
            else if ((v_idxs[mul_i] < dont_idxs[j]) &&
                     (v_idxs[mul_i] < do_idxs[i]))
            {
                result += multiply_from_string(v[mul_i]);
                // cout << "before dont: " << v[mul_i] << endl;
                mul_i++;
            }
            else if ((v_idxs[mul_i] > dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] < do_idxs[i]))
            {
                j++;
            }
            else if ((v_idxs[mul_i] > dont_idxs[j]) &&
                     (v_idxs[mul_i] > do_idxs[i]) &&
                     (dont_idxs[j] > do_idxs[i]))
            {
                i++;
            }
            else
            {
                // Nothing
            }
        }
    }

    return result;
}

int main()
{
    // string file_path = "./input_sample_d3p2_copy.txt";
    string file_path = "./d3p1.txt";

    regex e ("mul\\(\\d{1,3},\\d{1,3}\\)");

    fileOperations<string> fOp(file_path);
    vector<string> v;

    fOp.get_data_from_file(v, e);

    int result = multiply_from_text_plain(v);

    cout << "Result - part 1: " << result << endl;

    vector<string> v2;
    vector<int> v2_idxs;
    vector<int> do_idxs;
    vector<int> dont_idxs;

    fOp.get_data_from_file(v2, v2_idxs, e);

    regex do_reg ("do\\(\\)");
    regex dont_reg ("don\\'t\\(\\)");

    fOp.get_indexes(do_idxs, do_reg);
    fOp.get_indexes(dont_idxs, dont_reg);

    int result_2 = multiply_only_enabled_functions(v2, v2_idxs, do_idxs, dont_idxs);
    cout << "Result - part 2: " << result_2 << endl;

    return 0;
}