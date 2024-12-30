#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "file_operations.hpp"

using namespace std;

typedef enum {
    E_FAIL = 0,
    E_SUCCESS = 1
} E_STATUS_t;

int find_sign(int a)
{
   int sign_val = 0;

   if (a > 0)
   {
        sign_val = 1;
   }
   else if (a < 0)
   {
        sign_val = -1;
   }
   else
   {
    // Nothing
   }

   return sign_val;
}

int find_safe_reports(vector<vector<int>> vec_2d)
{
    int safe_reports = 0;
    int result2 = 0;

    int diff = 0;
    int abs_diff = 0;
    for (int i = 0; i < vec_2d.size(); i++)
    {
        int current_sign, prev_sign = 0;
        for (int j = 0; j < vec_2d[i].size() - 1; j++)
        {
            diff = vec_2d[i][j] - vec_2d[i][j+1];
            current_sign = find_sign(diff);
            if ((current_sign != 0) && (prev_sign != 0))
            {
                if (current_sign != prev_sign)
                {
                    break;
                }
            }

            abs_diff = abs(diff);
            if ((abs_diff >= 1) && (abs_diff <= 3))
            {
                if (j == vec_2d[i].size() - 2)
                {
                    safe_reports++;
                }
            }
            else
            {
                break;
            }

            prev_sign = current_sign;
        }
    }

    return safe_reports;
}

vector<bool> compute_bad_levels(vector<int> v, int& no_of_violations)
{
    int diff;

    vector<int> abs_diff_vec, sign_vec;
    vector<bool> suspects_vec(v.size(), false);

    no_of_violations = 0;

    for (auto i = 0; i < v.size() - 1; i++)
    {
        diff = v[i] - v[i+1];
        abs_diff_vec.push_back(abs(diff));
        sign_vec.push_back(find_sign(diff));

        if (i >= 1)
        {
            if ((sign_vec[i] != sign_vec[i-1]) ||
                ((abs_diff_vec[i] < 1) || (abs_diff_vec[i] > 3)))
            {
                no_of_violations++;
                suspects_vec[i-1] = true;
                suspects_vec[i] = true;
                suspects_vec[i+1] = true;
            }
        }
        else
        {
            if ((abs_diff_vec[i] < 1) || (abs_diff_vec[i] > 3))
            {
                no_of_violations++;
                suspects_vec[i] = true;
                suspects_vec[i+1] = true;
            }
        }
    }

    // cout << "Printing vec: ";
    // for (auto ele : v)
    // {
    //     cout << ele << " ";
    // }
    // cout << endl;
    // cout << "Printing suspects: ";
    // for (auto ele: suspects_vec)
    // {
    //     cout << ele << " ";
    // }
    // cout << endl;
    // cout << "Violations: " << no_of_violations << endl;

    return suspects_vec;
}

E_STATUS_t problem_dampener_app(vector<int> v)
{
    E_STATUS_t status_t = E_FAIL;

    vector<int> diff_vec;
    vector<int> sign_vec;
    vector<bool> rule_violations_info_vec;
    int num_violations = 0;

    rule_violations_info_vec = compute_bad_levels(v, num_violations);
    
    // if (num_violations <= 3)
    // {
        // if (num_violations == 0)
        // {
        //     status_t = E_SUCCESS;
        // }
        // else
        // {
            for (int i = 0; i < rule_violations_info_vec.size(); i++)
            {
                if (num_violations == 0)
                {
                    status_t = E_SUCCESS;
                    break;
                }
                if (rule_violations_info_vec[i] == true)
                {
                    vector<int> temp_vec = v;
                    vector<bool> temp_rule_violations_vec;
                    temp_vec.erase(temp_vec.begin() + i);
                    temp_rule_violations_vec = compute_bad_levels(temp_vec, num_violations);
                    if (num_violations == 0)
                    {
                        status_t = E_SUCCESS;
                        break;
                    }
                }
            }
        // }
    // }

    return status_t;
}

int find_revised_safe_reports(vector<vector<int>> vec_2d)
{
    int safe_reports = 0;
    int result2 = 0;

    int diff = 0;
    int abs_diff = 0;
    for (int i = 0; i < vec_2d.size(); i++)
    {
        if (problem_dampener_app(vec_2d[i]) == E_SUCCESS)
        {
            safe_reports++;
        }
    }

    return safe_reports;
}

int main()
{
    string file_name = "./d2p1.txt";
    // string file_name = "./input_sample_d2p2.txt";
    vector<vector<int>> vec_2d;

    fileOperations<int> fOp(file_name);

    fOp.get_num_data_from_file(vec_2d);

    int pure_safe_reports = find_safe_reports(vec_2d);
    int revised_safe_reports = find_revised_safe_reports(vec_2d);

    cout << "No of safe reports: " << pure_safe_reports << endl;
    cout << "No of revised safe reports: " << revised_safe_reports << endl;

    return 0;
}