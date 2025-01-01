#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

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
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

int find_index(vector<int> v, int ele)
{
    int index = -1;
    auto it = find(v.begin(), v.end(), ele);
    index = it - v.begin();

    return index;
}

void create_predecessors_successors_from_rules(vector<int> &successors, vector<vector<int>> &predecessors_2d,
                                               vector<int> &predecessors, vector<vector<int>> &successors_2d,
                                               vector<vector<int>> const page_order_rules)
{
    //Store the 1st column numbers without repetition in a vector
    for (int i = 0; i < page_order_rules.size(); i++)
    {
        int predecessor = page_order_rules[i][0];
        if (count(predecessors.begin(), predecessors.end(), predecessor) == 0)
        {
            predecessors.push_back(predecessor);
        }
    }

    //Store the 2nd column numbers without repetition in a vector
    for (int i = 0; i < page_order_rules.size(); i++)
    {
        int successor = page_order_rules[i][1];
        if (count(successors.begin(), successors.end(), successor) == 0)
        {
            successors.push_back(successor);
        }
    }

    // For each successor element, store the corresponding predecessors
    for (int i = 0; i < successors.size(); i++)
    {
        vector<int> preds_for_successor;
        for (int j = 0; j < page_order_rules.size(); j++)
        {
            if (successors[i] == page_order_rules[j][1])
            {
                int predecessor = page_order_rules[j][0];
                if (count(preds_for_successor.begin(), preds_for_successor.end(), predecessor) == 0)
                {
                    preds_for_successor.push_back(predecessor);
                }
            }
        }
        predecessors_2d.push_back(preds_for_successor);
    }

    // For each predecessor element, store the corresponding successors
    for (int i = 0; i < predecessors.size(); i++)
    {
        vector<int> sucs_for_predecessor;
        for (int j = 0; j < page_order_rules.size(); j++)
        {
            if (predecessors[i] == page_order_rules[j][0])
            {
                int successor = page_order_rules[j][1];
                if (count(sucs_for_predecessor.begin(), sucs_for_predecessor.end(), successor) == 0)
                {
                    sucs_for_predecessor.push_back(successor);
                }
            }
        }
        successors_2d.push_back(sucs_for_predecessor);
    }

    // cout << "Predecessors:" << endl;
    // print_vector<int>(predecessors);
    // cout << "Successors 2D: " << endl;
    // print_vector<int>(successors_2d);
    // cout << "Successors: " << endl;
    // print_vector<int>(successors);
    // cout << "Predecessors 2D:" << endl;
    // print_vector<int>(predecessors_2d);
}

int find_relevant_updates(vector<int> &successors, vector<vector<int>> &predecessors_2d, vector<int> &inordered_idxs, vector<vector<int>> const updates)
{
    int result = 0;

    for (int i = 0; i < updates.size(); i++)
    {
        int valid_update_count = 0;
        for (int j = 0; j < updates[i].size(); j++)
        {
            if (count(successors.begin(), successors.end(), updates[i][j]) > 0)
            {
                int suc_pre_idx = find_index(successors, updates[i][j]);
                // cout << "s-p: " << suc_pre_idx << endl;
                int k = 0;
                for (k = 0; k < predecessors_2d[suc_pre_idx].size(); k++)
                {
                    if (count(updates[i].begin() + j, updates[i].end(), predecessors_2d[suc_pre_idx][k]) > 0)
                    {
                        break;
                    }
                }

                if (k == predecessors_2d[suc_pre_idx].size())
                {
                    valid_update_count++;
                }
                else
                {
                    inordered_idxs.push_back(i);
                    break;
                }
            }
            else
            {
                valid_update_count++;
            }
        }

        if (valid_update_count == updates[i].size())
        {
            int middle_idx = (valid_update_count-1)/2;
            result += updates[i][middle_idx];
        }
    }
    return result;
}

int correct_inordered_pages(vector<int> const successors, vector<vector<int>> const predecessors_2d,
                            vector<int> const predecessors, vector<vector<int>> const successors_2d, 
                            vector<vector<int>> const inordered_page_updates)
{
    int result = 0;
    vector<vector<int>> temp_page_updates = inordered_page_updates;

    for (int i = 0; i < temp_page_updates.size(); i++)
    {
        for (int j = 0; j < temp_page_updates[i].size();)
        {
            int current_element = temp_page_updates[i][j];

            if (count(successors.begin(), successors.end(), current_element) > 0)
            {
                int suc_idx = find_index(successors, current_element);

                for (int k = 0; k < predecessors_2d[suc_idx].size(); k++)
                {
                    if (count(temp_page_updates[i].begin(), temp_page_updates[i].end(), predecessors_2d[suc_idx][k]) > 0)
                    {
                        int pre_idx_in_pg_upd = find_index(temp_page_updates[i], predecessors_2d[suc_idx][k]);

                        if (pre_idx_in_pg_upd > j)
                        {
                            temp_page_updates[i].erase(temp_page_updates[i].begin() + pre_idx_in_pg_upd);
                            temp_page_updates[i].insert(temp_page_updates[i].begin() + j, predecessors_2d[suc_idx][k]);   
                        }
                    }
                }
            }
            if (count(predecessors.begin(), predecessors.end(), current_element) > 0)
            {
                int pre_idx = find_index(predecessors, current_element);

                for (int k = 0; k < successors_2d[pre_idx].size(); k++)
                {
                    if (count(temp_page_updates[i].begin(), temp_page_updates[i].end(), successors_2d[pre_idx][k]) > 0)
                    {
                        int suc_idx_in_pg_upd = find_index(temp_page_updates[i], successors_2d[pre_idx][k]);

                        if (suc_idx_in_pg_upd < j)
                        {
                            temp_page_updates[i].erase(temp_page_updates[i].begin() + suc_idx_in_pg_upd);
                            temp_page_updates[i].insert(temp_page_updates[i].begin() + j + 1, successors_2d[pre_idx][k]);   
                        }
                    }
                }
            }

            if (current_element == temp_page_updates[i][j])
            {
                j++;
            }
        }

        int mid_idx = (temp_page_updates[i].size() - 1)/2;
        result += temp_page_updates[i][mid_idx];
    }

    return result;
}

int main()
{
    string file_path;
    // file_path = "./input_sample_d5.txt";
    file_path = "./d5.txt";

    fileOperations<int> fOp(file_path);
    vector<vector<int>> page_order_rules, page_updates;
    char order_rule_delimiter = '|';
    char update_delimiter = ',';

    fOp.get_num_data_from_file(page_order_rules, order_rule_delimiter);
    fOp.get_num_data_from_file(page_updates, update_delimiter);

    // cout << "Printing page order rules: " << endl;
    // fOp.print_vector(page_order_rules);

    // cout << "Printing updates: " << endl;
    // fOp.print_vector(page_updates);

    vector<int> successors;
    vector<vector<int>> predecessors_2d;
    vector<int> predecessors;
    vector<vector<int>> successors_2d;
    create_predecessors_successors_from_rules(successors, predecessors_2d, predecessors, successors_2d, page_order_rules);

    int result = 0;
    vector<int> inordered_idxs;
    result = find_relevant_updates(successors, predecessors_2d, inordered_idxs, page_updates);

    vector<vector<int>> inordered_page_updates;
    for (int i = 0; i < inordered_idxs.size(); i++)
    {
        inordered_page_updates.push_back(page_updates[inordered_idxs[i]]);
    }

    int result2 = 0;
    result2 = correct_inordered_pages(successors, predecessors_2d, predecessors, successors_2d, inordered_page_updates);

    cout << "Result - part 1: " << result << endl;
    cout << "Result - part 2: " << result2 << endl;

    return 0;
}