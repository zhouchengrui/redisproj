#include <cstdlib>
#include <iostream>
#include <chrono>
#include "db.h"
#include "exception.h"
using namespace std;
using namespace chrono;

void SplitTsv(string s, vector<string> &v){
    string temp = "";
    for(int i=0;i<s.length();++i){
        if(s[i]=='\t'){
            v.push_back(temp);
            temp = "";
        }
        else{
            temp.push_back(s[i]);
        }
    }
    v.push_back(temp);
}

vector<char*> get_all_key(string file_name, int num) {
    cout<<"File path received:"<<file_name<<endl;
    ifstream infile (file_name);
    string line;
    string temp;
    vector<char*> ans;
    char* key;
    while (std::getline(infile, line))
    {
        num--;
        if (num < 0) break;
        vector<string> row_values;
        SplitTsv(line,  row_values);
        temp = row_values.front();
        key = &temp[0];
        ans.push_back(key);
    }
    return ans;
}

vector<double> test_performance(string filename,int num) {
    string name = to_string(num);
    vector<double> time_costs;
    DB db(name);

    cout << "-------------test for insert function-----------" << endl;
    auto start = system_clock::now();
    // insert test start
    db.open();
    db.insert_file(filename, num);
    db.close();
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    double d = double(duration.count())/1000.0;
    // insert test end
    cout << "time for insert function is: " << d << " milliseconds" << endl;
    time_costs.push_back(d);

    cout << "-------------test for search function-----------" << endl;
    start = system_clock::now();
    db.open();
    vector<char*> allKeys = get_all_key(filename, num);
    for (auto key : allKeys) {
        db.find(key);
    }
    db.close();
    end = system_clock::now();
    duration = duration_cast<microseconds>(end - start);
    d = double(duration.count())/1000.0;
    cout << "time for search function is: " << d << " milliseconds" << endl;
    time_costs.push_back(d);

    cout << "-------------test for query function-----------" << endl;
    start = system_clock::now();
    db.open();
    string val = "###";
    db.query(0, val);
    db.close();
    end = system_clock::now();
    duration = duration_cast<microseconds>(end - start);
    d = double(duration.count())/1000.0;
    cout << "time for query function is: " << d << " milliseconds" << endl;
    time_costs.push_back(d);

    cout << "-------------test for delete function-----------" << endl;
    start = system_clock::now();
    db.open();
    for (auto key : allKeys) {
        db.del(key);
    }
    db.close();
    end = system_clock::now();
    duration = duration_cast<microseconds>(end - start);
    d = double(duration.count())/1000.0;
    cout << "time for delete function is: " << d << " milliseconds" << endl;
    time_costs.push_back(d);

    db.clear();
    return time_costs;
}

int main() {
    vector<vector<double>> t;
    t.push_back(test_performance("data.tsv", 100));
    t.push_back(test_performance("data.tsv", 1000));
    t.push_back(test_performance("data.tsv", 10000));
    t.push_back(test_performance("data.tsv", 100000));
    t.push_back(test_performance("data.tsv", 200000));
    cout << endl << "------------------Result-----------------" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++)
            cout << t[i][j] << " ";
        cout << endl;
    }
    return 0;
}