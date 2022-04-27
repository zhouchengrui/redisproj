#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>					 //测试时存储随机字符串
#include "Hash.h"
#include "Hash.cpp"
#include "exception.h"
#define TEST_NUM 1000000
using namespace std;


char* randomString(int max);//make random string  length:(1~max)
//void test_correct();
void test_performance();
void run();
int main() {
    cout << "1\n";
    cout << "menu\n";
    cout << "Enter 1 for demonstration test, enter 2 for performance test, enter 0 to enter the interactive interface, enter -1 to exit the program\n";
    int num;
    cin >> num;
    while(cin.fail()){
        try{
            throw InvalidType{};
        }catch (const Myexcept &e)
        {
            cerr << e.what() << '\n' << endl;
        }
        cin.clear();
        cin.ignore();
        cin >> num;
    }
    try
    {
        if(num < -1 || num > 2){
            throw InvalidNumber{};
        }
    }
    catch (const Myexcept &e)
    {
        cerr << e.what() << '\n' << endl;
    }
    while (num != -1) {
        if (num == 1)
//            test_correct();
            cout << "menu\n";
        /*
        if(num==2)
            test_performance();
        */
        if (num == 0)
            run();
        cout << "menu\n";
        cout << "Enter 1 for demonstration test, enter 2 for performance test, enter 0 to enter the interactive interface, enter -1 to exit the program\n";
        cin >> num;
    }
    return 0;
}

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


void SplitString(string s, vector<string> &v){
    string temp = "";
    for(int i=0;i<s.length();++i){
        if(s[i]==','){
            v.push_back(temp);
            temp = "";
        }
        else{
            temp.push_back(s[i]);
        }
    }
    v.push_back(temp);
}

void run() {
    string name;
    cout << "Enter name for db, if you want to delete a database, input delete + '-' + name of database:\n";
    cin >> name;
    bool flag = true;
    cout << name.substr(0, 7).compare("delete-") << endl;
    int compareValue = name.substr(0, 7).compare("delete-");
    if(compareValue == 0){
        cout << name.length() << endl;
        name = name.substr (7, name.length());
        cout << name << endl;
        flag = false;
    }
    if(flag == false){
        if(findDatabase(name) == false) {
            cout << "database not found" << endl;
            return;
        }else{
            DB db(name);
            db.clear();
            cout << "clear successfully!\n";
        }
    }else{
        DB db(name);
        db.open();
        cout << "Enter 0 to initialize random strings, enter 1 to insert, enter 2 to search, enter 3 to replace, enter 4 to delete, enter 5 to query, enter 6 to delete the entire database, and -1 to return to the previous menu\n";
        int n;
        cin >> n;
        while(cin.fail()){
            try{
                throw InvalidType{};
            }catch (const Myexcept &e)
            {
                cerr << e.what() << '\n' << endl;
            }
            cin.clear();
            cin.ignore();
            cin >> n;
        }
        try
        {
            if(n < -1 || n > 6){
                throw InvalidNumber{};
            }
        }
        catch (const Myexcept &e)
        {
            cerr << e.what() << '\n' << endl;
        }
        while (n != -1) {
            /*
            if (n == 0) {
                srand(time(NULL));
                map<char*, char*> m;
                map<char*, char*>::iterator it;
                for (int i = 0; i < TEST_NUM; i++) {
                    char*key = randomString(KEYSIZE_MAX - 1);
                    char*value = randomString(VALUESIZE_MAX - 1);
                    m.insert(map<char*, char*>::value_type(key, value));
                }
                for (it = m.begin(); it != m.end(); it++) {
                    db.insert(it->first, it->second);
                }
            }
             */
            if (n == 1) {
                char key[KEYSIZE_MAX];
                vector<string> val;
                cout << "insert operation\n";
                cout << "insert key:\n";
                cin >> key;
                cout << "insert value (use comma to split value):\n";
                string value="";
                cin >> value;
                vector<string> values;
                SplitString(value,values);
                if (db.insert(key, values))
                    cout << "Insert operation complete\n";
                else
                    cout << "can't insert\n";
            }
            if (n == 2) {
                char key[KEYSIZE_MAX];
                cout << "find operation\n";
                cout << "input key:\n";
                cin >> key;
                vector<string> val = db.find(key);
                if (val.size() != 0) {
                    for (int i = 0; i < val.size(); i++) {
                        if (val[i].length() != 0)
                            cout << "the " << i << "th  value is: " << val[i] << endl;
                        else
                            break;
                    }
                    cout << "Find operation complete\n";
                }
                else
                    cout<<"No corresponding value found\n";

            }/*
            if (n == 3) {
                char key[KEYSIZE_MAX];
                char value[VALUESIZE_MAX];
                int ind;
                cout << "update operation\n";
                cout << "insert key:\n";
                cin >> key;
                cout << "insert index that needs to be updated:\n";
                cin >> ind;
                cout << "Insert Value:" << endl;
                cin >> value;
                if(db.update(key,ind,value))
                    cout << "Update operation complete\n";
                else
                    cout << "cannot be updated\n";
            }*/
            if (n == 4) {
                char key[KEYSIZE_MAX];
                cout << "delete operation\n";
                cout << "insert key:\n";
                cin >> key;
                if ( db.del(key)) {
                    cout << "delete operation complete\n";
                }
                else
                    cout << "Could not find the key\n";
            }

            if (n == 5) {
                int k;
                string val;
                cout << "the position of value : " << endl;
                cin >> k;
                cout << "the value is : " << endl;
                cin >> val;
                vector<string> ans = db.query(k, val);
                cout << "The query operation is complete\n";
            }

            if (n == 6) {
                cout << "Are you sure to delete the database? \n" << endl;
                cout << "Press 1 to delete, press 0 to return \n" << endl;
                int temp;
                cin >> temp;
                while(cin.fail()){
                    try{
                        throw InvalidType{};
                    }catch (const Myexcept &e)
                    {
                        cerr << e.what() << '\n' << endl;
                    }
                    cin.clear();
                    cin.ignore();
                    cin >> temp;
                }
                if(temp == 1){
                    db.clear();
                }
            }
            cout << "Enter 1 for insert operation, enter 2 for search operation, enter 3 for replacement operation, enter 4 for delete operation, enter 5 to query, enter -1 to return to the previous menu\n";
            cin >> n;

        }
        db.close();
    }
}
