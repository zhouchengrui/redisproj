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
//    try
//    {
//        if(num < -1 || num > 2){
//            throw InvalidNumber{};
//        }
//    }
//    catch (const Myexcept &e)
//    {
//        cerr << e.what() << '\n' << endl;
//    }
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

/*
void test_performance() {
    string name = "test_performance";
    DB db(name);
    srand(time(NULL));
    map<char*, char*> m;
    map<char*, char*>::iterator it;
    for (int i = 0; i < TEST_NUM; i++) {
        char*key = randomString(KEYSIZE_MAX - 1);
        char*value = randomString(VALUESIZE_MAX - 1);
        m.insert(map<char*, char*>::value_type(key, value));
    }
    clock_t start;
    clock_t finish;
    cout << "插入测试\n";
    cout << "Test number: " << TEST_NUM << endl;
    start = clock();
    // insert test start
    db.open();
    for (it = m.begin(); it != m.end(); it++) {
        db.insert(it->first, it->second);
    }
    db.close();
    finish = clock();

    // insert test end
    cout << "time for " << TEST_NUM << " insertion is: " << (double)(finish - start) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "查询测试\n";
    cout << "Test number: " << TEST_NUM << endl;
    start = clock();
    // find test start
    db.open();
    for (it = m.begin(); it != m.end(); it++) {
        db.find(it->first);
    }
    db.close();
    finish = clock();
    // find test end
    cout << "time for " << TEST_NUM << " finding is: " << (double)(finish - start) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "修改测试\n";
    cout << "Test number: " << TEST_NUM << endl;
    start = clock();
    //replace test
    db.open();
    for (it = m.begin(); it != m.end(); it++) {
        char*value = randomString(VALUESIZE_MAX - 1);
        db.replace(it->first, value);
    }
    db.close();
    finish = clock();
    // replace test end
    cout << "time for " << TEST_NUM << " replacement is: " << (double)(finish - start) / CLOCKS_PER_SEC << " seconds" << endl;
    if (HASH_SIZE / TEST_NUM <= 100) {
        db.clear();
        return;
    }
    cout << "删除测试\n";
    cout << "Test number: " << TEST_NUM << endl;
    start = clock();
    //delete test
    db.open();
    for (it = m.begin(); it != m.end(); it++) {
        db.del(it->first);
    }
    db.traversal();
    db.close();
    finish = clock();
    // delete test end
    cout << "time for " << TEST_NUM << " deletion is: " << (double)(finish - start) / CLOCKS_PER_SEC << " seconds" << endl;
    db.clear();
}
*/


//void test_correct() {
//    string name = "test_correct";
//    DB db(name);
//    db.open();
//    cout << "插入three-3,two-2,four-4三对键值对到数据库\n";
//    db.insert("three", "3");
//    db.insert("two", "2");
//    db.insert("four", "4");
//    cout << "打印所有键值对\n";
//    db.traversal();
//    cout << "查找two和three对应的value\n";
//    if (char* value = db.find("two"))
//        cout << "value is: " << value << endl;
//    if (char* value = db.find("three"))
//        cout << "value is: " << value << endl;
//    cout << "替换two对应的value为er\n";
//    if (db.replace("two", "er")) {
//        if (char* value = db.find("two"))
//            cout << "value is: " << value << endl;
//    }
//    cout << "删除four键值对\n";
//    if (db.del("four")) {
//        if (!db.find("four"))
//            cout << "four已被删除\n";
//    }
//    cout << "打印所有键值对\n";
//    db.traversal();
//    cout << "插入four-4键值对\n";
//    db.insert("four", "4");
//    cout << "查找four对应的value\n";
//    if (char* value = db.find("four"))
//        cout << "value is: " << value << endl;
//    cout << "打印所有键值对\n";
//    db.traversal();
//    db.close();
//    db.clear();
//}


void run() {
    string name;
    cout << "Enter name for db:\n";
    cin >> name;
    DB db(name);
    db.open();
    cout << "Enter 0 to initialize random strings, enter 1 to insert, enter 2 to search, enter 3 to replace, enter 4 to delete, enter 5 to print the entire database, and -1 to return to the previous menu\n";
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
        if(n < -1 || n > 5){
            throw InvalidNumber{};
        }
    }
    catch (const Myexcept &e)
    {
        cerr << e.what() << '\n' << endl;
        cout << e.what() << '\n' << endl;
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
            cout << "insert value (enter # at the end of value):\n";
            string value;
            int k = 0;
            while(1) {
                k++;
                if (k == 10) {
                    cout << "value cannot be more than ten items!" << endl;
                    break;
                }
                cin >> value;
                if (value == "#" )
                    break;
                else
                    val.push_back(value);
            }
            if (db.insert(key, val))
                cout << "Insert operation complete\n";
            else
                cout << "can't insert\n";
        }
        if (n == 2) {
            char key[KEYSIZE_MAX];
            cout << "find operation\n";
            cout << "insert key:\n";
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

        }
        /*
        if (n == 3) {
            char key[KEYSIZE_MAX];
            char value[VALUESIZE_MAX];
            cout << "replace operation\n";
            cout << "insert key:\n";
            cin >> key;
            cout << "insert value:\n";
            cin >> value;
            if(db.replace(key, value))
                cout << "Replace operation complete\n";
            else
                cout << "cannot be replaced\n";
        }
         */
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
        /*
        if (n == 5) {
            cout << "print operation\n";
            db.traversal();
            cout << "The print operation is complete\n";
        }
         */
        cout << "Enter 1 for insert operation, enter 2 for search operation, enter 3 for replacement operation, enter 4 for delete operation, enter 5 to print the entire database, enter -1 to return to the previous menu\n";
        cin >> n;

    }
    db.close();
}