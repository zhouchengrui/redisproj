#pragma once
#include <string>                  //仅在输入文件名并创建文件时使用
#include <compare>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include "idx.h"
using namespace std;
#define HASH_SIZE  20000003 //size of hash table

class DB {
public:
    DB(string fileName);			                    //initialize db, create files
    unsigned int hash(const char* key);                 //hash function
    bool open();												//open db files. Load last_idx_off and last_value_off
    bool close();                                             //close db files. store last_idx_off and last_value_off.
    bool insert_file(string file_name, int num);                      //insert data with csv/tsv files, num is the max rows
    vector<string> find(const char* key);                    //look for the value. return null if not found
    bool del(char* key);								  //delete key
    bool insert(char* key, vector<string> val);           //insert key-value. if already exists, return false.
    bool replace(char* key, vector<string> val);          //update value
    vector<string> query(int k, string val);               //use given value to search for key.
    void clear();										 //delete db files.
private:
    Idx* find_key(const char*key);                   //find corresponding idx struct of key
    unsigned int last_idx_off;                       //total offset of idx file
    unsigned int last_dat_off;                       //total offset of data file
    string fileName;                                 //input csv/tsv file name
    string idxName, datName;                         //name of idx file and data file
    FILE* fp1;                                       //idx file stream
    FILE* fp2;                                        //data file stream
};