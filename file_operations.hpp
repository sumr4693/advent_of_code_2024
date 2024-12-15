#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

template <class T1>
class fileOperations
{
private:
fstream file;
string file_path;
public:
fileOperations(string file_str) : file_path(file_str) {
    file.open(file_path, ios::in);

    if (!file)
    {
        cout << "Can't open " << file_path << endl;
    }
    else
    {
        cout << "File " << file_path << " found" << endl;
    }
}

void get_data_from_file(vector<T1> &v1, vector<T1> &v2)
{
    T1 v1i, v2i;
    int no_of_lines = 0;

    while (file >> v1i >> v2i)
    {
        v1.push_back(v1i);
        v2.push_back(v2i);
        no_of_lines++;
    }

    cout << "Number of lines: " << no_of_lines << endl;
}

void get_data_from_file(vector<vector<T1>> &v)
{
    vector<T1> row_vec;
    int no_of_lines = 0;

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string str_num;
        vector<T1> temp_row_vec;

        while (getline(ss, str_num, ' '))
        {
            temp_row_vec.emplace_back(stoi(str_num));
        }
        v.push_back(temp_row_vec);
        no_of_lines++;
    }

    cout << "Number of lines: " << no_of_lines << endl;
}

void get_data_from_file(vector<T1> &v, regex e)
{
    string line;
    // smatch res;

    regex_token_iterator<string::iterator> rend;
    while (getline(file, line))
    {
        regex_token_iterator<string::iterator> a (line.begin(), line.end(), e);
        while (a != rend)
        {
            v.push_back(*a++);
        }
        // while (regex_search(line, res, e)) {
        //     cout << res[0] << endl;
        //     line = res.suffix();
        // }
    }

    // for (auto ele : v)
    // {
    //     cout << ele << endl;
    // }
}
};