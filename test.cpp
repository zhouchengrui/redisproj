#include <cstdlib>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include "db.h"
#include "exception.h"
using namespace std;
using namespace chrono;

#define TEST_NUM 10

inline const int VALUENUM_MAX = 5;  //每个key对应的value的最大数量
inline const int VALUESIZE_MAX = 8;  //数据字符串的最大长度

char* randomString(int max) {
    int len = rand() % max;
    len = ((len == 0) ? max : len);
    char* s = new char[max + 1];
    for (int j = 0; j < len; j++) {
        s[j] = 'a' + (rand() % 26);
    }
    s[len] = '\0';
    return s;
}

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

unordered_map<char*, vector<string>> random_generate() {
    srand(time(NULL));
    unordered_map<char*, vector<string>> m;
    for (int i = 0; i < 5; i++) {
        vector<string> temp;
        char*key = randomString(5);
        while(m.contains(key)){
            char*key = randomString(5);
        }
        for(auto j = 0; j < rand() % VALUENUM_MAX + 1; j++){
            string tempstr = randomString(VALUESIZE_MAX - 1);
            temp.push_back(tempstr);
            if(temp.size() > 10) break;
        }
        m.insert(unordered_map<char*, vector<string>>::value_type(key, temp));
    }
    return m;
}

void printVector(vector<string> list) {
    for (auto it = list.begin(); it != list.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

void test_correct() {
    string name = "test1";
    DB db(name);
    db.open();
    unordered_map<char*, vector<string>> m = random_generate();
    unordered_map<char*, vector<string>>::iterator it;
    for (auto i : m) {
        db.insert(i.first, i.second);
    }
    cout << "random generate key value pairs\n";
    cout << "printing all key-value pairs:\n";
    bool flag = true;
    for (const auto &[key, value] : m){
        vector<string> val = db.find(key);
        cout<<"value loaded from db:" << endl;
        if (val.size() != 0) {
            printVector(val);
        }
        else
            cout<<"No corresponding value found!" << endl;
        cout<<"generated value:" << endl;
        if (val.size() != 0) {
            printVector(value);
        }
        else
            cout<<"No corresponding value found!" << endl;
        flag = flag && (val == value);
    }

    if(flag)
        cout<<"values are the same!" << endl;
    else
        cout<<"values are not the same!" << endl;

    cout<<"Now testing delete function:" << endl;
    for (it = m.begin(); it != m.end(); it++) {
        cout << "----------deleting key : " << it->first << "----------" << endl;
        if (db.del(it->first)) {
            if (db.find("four").size() == 0)
                cout << "----------delete successfully----------" << endl;
        }
    }

    db.close();
    db.clear();
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

    cout << endl << "-----------------Now testing the correctness of the database-----------------" << endl;
    test_correct();
    return 0;
}