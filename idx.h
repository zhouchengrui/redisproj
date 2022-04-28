#include <string>
#include <compare>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
using namespace std;
#define KEYSIZE_MAX 20

struct Idx {
    char key[KEYSIZE_MAX];        //key
    unsigned int value_off;		  //the offset of data in dat file
    unsigned int off;			  //the offset of key in idx file
    unsigned int off_next;        //the next offset in the key idx linked list
    int len_key;				  //the length of the key
    int len_value[10];            //the length of each value data
    bool isDelete;                //if the key has already been deleted
};

