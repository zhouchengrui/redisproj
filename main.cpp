#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>					 //测试时存储随机字符串
#include "db.h"
#include "exception.h"
#define TEST_NUM 10
using namespace std;

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

bool findDatabase(string s){
    string idxName = s + ".idx";
    string datName = s + ".dat";
//    cout << "idxName : " << idxName << endl;
    string idxPath =  "./" + s + ".idx";
    string datPath = "./" + s + ".dat";
    FILE* fp1 = fopen(idxPath.c_str(), "rb");
    FILE* fp2 = fopen(datPath.c_str(), "rb");
    if ((fp1 == NULL) || (fp2 == NULL)){
        return false;
    }
    fclose(fp1);
    fclose(fp2);
    return true;
}

void run() {
    cout << "Enter the name of db. if you want to delete a db, please enter 'delete-' + name of db:" << endl;
    string name;
    cin >> name;
    bool flag = true;
    // cout << name.substr(0, 7).compare("delete-") << endl;
    int compareValue = name.substr(0, 7).compare("delete-");
    if (compareValue == 0){
        name = name.substr (7, name.length());
        flag = false;
    }
    if (flag == false) {
        if (findDatabase(name) == false) {
            cout << "database not found" << endl;
            return;
        }
        else {
            DB db(name);
            db.clear();
            cout << "database-" << name << " has been successfully cleared!\n";
        }
    }
    else {
        DB db(name);
        db.open();
        cout << "Enter 1 to insert, enter 2 to search, enter 3 to delete, enter 4 to query, enter -1 to return to the previous menu" << endl;
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
            if(n < -1 || n > 4){
                throw InvalidNumber{};
            }
        }
        catch (const Myexcept &e)
        {
            cerr << e.what() << '\n' << endl;
        }
        while (n != -1) {
            if (n == 1) {
                char key[KEYSIZE_MAX];
                vector<string> val;
                cout << "-------------------------insert operation-----------------------" << endl;
                cout << "insert key : " << endl;
                cin >> key;
                cout << "insert value (use comma to split value):" << endl;
                string value="";
                cin >> value;
                vector<string> values;
                SplitString(value,values);
                if (db.insert(key, values))
                    cout << "Insert operation complete." << endl;
                else
                    cout << "Could not insert!" << endl;
            }
            if (n == 2) {
                char key[KEYSIZE_MAX];
                cout << "-------------------------find operation-----------------------" << endl;
                cout << "enter key : " << endl;
                cin >> key;
                vector<string> val = db.find(key);
                if (val.size() != 0) {
                    for (int i = 0; i < val.size(); i++) {
                        if (val[i].length() != 0)
                            cout << "the " << i << "th  value is: " << val[i] << endl;
                        else
                            break;
                    }
                    cout << "Find operation complete." << endl;
                }
                else
                    cout<<"No corresponding value found!" << endl;

            }
            if (n == 3) {
                char key[KEYSIZE_MAX];
                cout << "-------------------------delete operation-----------------------" << endl;
                cout << "enter key : " << endl;
                cin >> key;
                if ( db.del(key)) {
                    cout << "delete operation complete." << endl;
                }
                else
                    cout << "Could not find the key!" << endl;
            }
            if (n == 4) {
                cout << "-------------------------query operation-----------------------" << endl;
                int k;
                string val;
                cout << "enter the value : " << endl;
                cin >> val;
                cout << "enter the position of value : " << endl;
                cin >> k;
                vector<string> ans = db.query(k, val);
                cout << "The query operation is complete." << endl;
            }

            cout << "Enter 1 to insert, enter 2 to search, enter 3 to delete, enter 4 to query, enter -1 to return to the previous menu" << endl;
            cin >> n;

        }
        db.close();
    }
}

int main() {
    cout << "Welcome to MicroDB!" << endl;
    int num = 0;
    while (num != -1) {
        run();
        cout << "Enter 1 to continue the program. Enter -1 to exit the program" << endl;
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
            if(num != -1 && num != 1){
                throw InvalidNumber{};
            }
        }
        catch (const Myexcept &e)
        {
            cerr << e.what() << '\n' << endl;
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
}