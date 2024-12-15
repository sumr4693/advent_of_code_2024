#include <iostream>
#include <string>
#include <algorithm>

#include "file_operations.hpp"

using namespace std;

int main()
{
    string file_name = "./input_d1p1.txt";
    // string file_name = "./input_sample_d1p1.txt";
    vector<int> v1, v2, diff_vec;

    fileOperations<int> fOp(file_name);

    fOp.get_data_from_file(v1, v2);

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    int result1 = 0;
    int result2 = 0;

    int element_freq = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        diff_vec.push_back(abs(v1[i] - v2[i]));
        result1 += diff_vec[i];

        element_freq = count(v2.begin(), v2.end(), v1[i]);
        result2 += element_freq * v1[i];
    }

    cout << "Results 1 and 2: " << result1 << " " << result2 << endl;

    return 0;
}