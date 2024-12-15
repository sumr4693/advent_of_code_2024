#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>

#include "file_operations.hpp"

using namespace std;

int multiply_from_text_plain(vector<string> v)
{
    int result = 0;

    for (int i = 0; i < v.size(); i++)
    {
        int idx1 = v[i].find('(');
        int idx2 = v[i].find(',');
        int idx3 = v[i].find(')');

        int first = stoi(v[i].substr(idx1+1, idx2-idx1-1));
        int second = stoi(v[i].substr(idx2+1, idx3-idx2-1));

        result += (first*second);

        // cout << "first: " << v[i].substr(idx1+1, idx2-idx1-1) << endl;
        // cout << "second: " << v[i].substr(idx2+1, idx3-idx2-1) << endl;
    }

    return result;
}

int main()
{
    // string file_path = "./input_sample_d3p1.txt";
    string file_path = "./d3p1.txt";

    regex e ("mul\\(\\d{1,3},\\d{1,3}\\)");

    fileOperations<string> fOp(file_path);
    vector<string> v;

    fOp.get_data_from_file(v, e);

    int result = multiply_from_text_plain(v);

    cout << "Result - part 1: " << result << endl;

    return 0;
}