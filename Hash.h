#pragma once
#include <string>                  //仅在输入文件名并创建文件时使用
#include <compare>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
using namespace std;
#define KEYSIZE_MAX 8        //索引字符串的最大长度
#define HASH_SIZE  20000003 //散列表的大小
struct Idx {
    char key[KEYSIZE_MAX];     //索引字符串
    unsigned int value_off;		  //数据在数据文件中偏移量
    unsigned int off;				  //索引在索引文件中偏移量
    unsigned int off_next;         //索引链表中当前索引的下一跳索引的偏移量
    int len_key;						  //索引字符串的长度
    int len_value[10];                      //数据字符串的长度
    bool isDelete;
};

bool findDatabase(string s);
inline const int VALUENUM_MAX = 5;  //每个key对应的value的最大数量

class DB {
public:
    DB(string fileName);			                    //创建两个文件且初始化数据库，若文件已存在则什么都不做
    unsigned int hash(const char* key);      //散列函数
    int open();												//打开数据库的两个文件，并载入last_idx_off和last_value_off两个值
    int close();                                             //保存关闭数据库的两个文件，并存入last_idx_off和last_value_off的值到文件
    int insert_file(string file_name);
    vector<string> find(const char* key);                   //寻找key对应的value，返回value字符串指针,否则返回NULL
    bool del(char* key);								//删除key，返回true，否则返回false
    int insert(char* key, vector<string> val);           //插入key及value，返回1，若已存在该key，返回0
    bool replace(char* key, vector<string> val);      //替换key对应的value，找不到key返回false，替换成功返回true
    vector<string> query(int k, string val);                                      //打印数据库所有key及对应value
    void clear();										 //删除数据库的两个文件
private:
    Idx* find_key(const char*key);                //寻找key对应索引结构体,返回结构体的指针，否则返回NULL
    unsigned int last_idx_off;                       //索引文件总偏移量
    unsigned int last_dat_off;                       //数据文件总偏移量
    string fileName;                                     //数据库名
    string idxName, datName;                      //两个文件名
    FILE* fp1;                                                //索引文件流
    FILE* fp2;                                               //数据文件流
    errno_t err1, err2;                                    //fopen_s 的返回值，用于判断两文件是否存在
};