#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <stdint.h>

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

void get_num_data_from_file(vector<T1> &v, char ch)
{
    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string str_num;

        size_t char_found = line.find(ch);

        if (char_found != string::npos)
        {
            while (getline(ss, str_num, ch))
            {
                v.push_back((T1) stoll(str_num));
            }
        }
    }
}

void get_num_data_from_file(vector<vector<T1>> &v, char ch)
{
    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    int no_of_lines = 0;
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string str_num;
        vector<T1> temp_row_vec;

        size_t char_found = line.find(ch);

        if (char_found != string::npos)
        {
            while (getline(ss, str_num, ch))
            {
                temp_row_vec.emplace_back((T1) stoll(str_num));
            }
            v.push_back(temp_row_vec);
            no_of_lines++;
        }
    }

    cout << "Number of lines: " << no_of_lines << endl;
}

void get_num_data_from_file(vector<vector<T1>> &v)
{
    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    int no_of_lines = 0;
    string line;

    while (getline(file, line))
    {
        vector<T1> temp_row_vec;

        for (char ch : line)
        {
            temp_row_vec.emplace_back((T1) ch - '0');
        }
        v.push_back(temp_row_vec);
        no_of_lines++;
    }

    cout << "Number of lines: " << no_of_lines << endl;
}

void get_char_data_from_file(vector<vector<T1>> &v)
{
    string line;

    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    while(getline(file, line))
    {
        vector<char> ch_vec;

        for (char& c : line)
        {
            ch_vec.push_back(c);
        }

        v.push_back(ch_vec);
    }
}

void get_data_from_file(vector<T1> &v, regex e)
{
    string line;
    // smatch res;

    // Point to beginning of the file
    file.clear();
    file.seekg(0);

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
}

void get_data_from_file(vector<string> &v)
{
    string line;

    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    while (getline(file, line))
    {
        v.push_back(line);
    }
}

void get_data_from_file(vector<T1>& v, vector<int>& v_idxs, regex e)
{
    string line;
    int total_len = 0;

    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    regex_token_iterator<string::iterator> rend;
    while (getline(file, line))
    {
        size_t pattern_idx = 0;
        regex_token_iterator<string::iterator> a (line.begin(), line.end(), e);
        while (a != rend)
        {
            string tmp_str = *a;
            int total_idx = 0;
            // cout << tmp_str << ", before finding str " << pattern_idx << endl;
            pattern_idx = line.find(tmp_str, pattern_idx);
            // cout << "Finding str: " << pattern_idx << endl;
            v.push_back(tmp_str);
            total_idx = (int) pattern_idx + total_len;
            v_idxs.push_back(total_idx);
            // cout << tmp_str.length() << " " << total_idx << endl;
            pattern_idx += (size_t) tmp_str.length();
            *a++;
        }

        total_len += line.length();
    }

    // cout << "Printing strings and indexes: " << endl;

    // for (auto i = 0; i < v.size(); i++)
    // {
    //     cout << v[i] << " " << v_idxs[i] << endl;
    // }
}

void get_indexes(vector<int>& v_idxs, regex match_expr)
{
    string line;
    int total_len = 0;

    // Point to beginning of the file
    file.clear();
    file.seekg(0);

    regex_token_iterator<string::iterator> rend;
    while (getline(file, line))
    {
        size_t pattern_idx = 0;
        regex_token_iterator<string::iterator> a (line.begin(), line.end(), match_expr);
        while (a != rend)
        {
            string tmp_str = *a;
            pattern_idx = line.find(tmp_str, pattern_idx);
            v_idxs.push_back((int) pattern_idx + total_len);
            pattern_idx += tmp_str.length();
            *a++;
        }

        total_len += line.length();
    }

    // for (auto i = 0; i < v_idxs.size(); i++)
    // {
    //     cout << v_idxs[i] << endl;
    // }
}

void print_vector(vector<vector<T1>> const v)
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

void print_vector(vector<T1> v)
{
    cout << "Printing elements in 1D vector: " << endl;

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }

    cout << endl;
}

void write_to_file(string s, string filepath)
{
    ofstream out (filepath);
    out << s;
    out.close();
}

void write_to_file(vector<string> s, string filepath)
{
    ofstream out (filepath);

    for (auto ele : s)
    {
        out << ele;
    }
    out.close();
}

};