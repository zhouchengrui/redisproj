#include "Hash.h"
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

DB::DB(string s) {
	fileName = s;
	idxName = fileName + ".idx";
	datName = fileName + ".dat";
    cout << "idxName : " << idxName << endl;
    string idxPath =  "./" + fileName + ".idx";
    string datPath = "./" + fileName + ".dat";
    /*
    fstream idx_file;
    fstream dat_file;
    idx_file.open(idxName, ios::in);
    dat_file.open(idxName, ios::in);
    if (!idx_file.is_open() || !dat_file.is_open()) {
        cout << "An empty database is built!" << endl;
        fstream new_idx_file;
        fstream new_dat_file;
        new_idx_file.open(idxName, ios::out);
        new_dat_file.open(idxName, ios::out);

        last_dat_off = 0;
        last_idx_off = HASH_SIZE;
        new_dat_file << last_dat_off;
        new_idx_file << last_idx_off;

        Idx index;
        for (int i = 0; i < KEYSIZE_MAX - 1; i++)
            index.key[i] = '\0';
        index.key[KEYSIZE_MAX - 1] = '\0';
        index.value_off = 0;
        index.off = 0;
        index.off_next = 0;
        index.len_key = 0;
        index.len_value = 0;
        index.isDelete = true;
        for (int i = 0; i < HASH_SIZE; i++)
            new_idx_file << &index;
        new_idx_file.close();
        new_dat_file.close();
    }
    else {
        cout << "Database : " << s << " has been found!" << endl;
        idx_file.close();
        dat_file.close();
    }
     */
	fp1 = fopen(idxPath.c_str(), "rb");
	fp2 = fopen(datPath.c_str(), "rb");
	if ((fp1 == NULL) || (fp2 == NULL)) {
		cout << "An empty DB will be built!\n";
		fp1 = fopen(idxPath.c_str(), "wb");
		fp2 = fopen(datPath.c_str(), "wb");
		//
		last_dat_off = 0;
		last_idx_off = HASH_SIZE;
		//
		fwrite(&last_dat_off, sizeof(int), 1, fp2);
		fwrite(&last_idx_off, sizeof(int), 1, fp1);
		Idx index;
		for (int i = 0; i < KEYSIZE_MAX - 1; i++)
			index.key[i] = '\0';
		index.key[KEYSIZE_MAX - 1] = '\0';
		index.value_off = 0;
		index.off = 0;
		index.off_next = 0;
		index.len_key = 0;
        for (int i = 0; i < 10; i++)
		    index.len_value[i] = 0;
		index.isDelete = true;
		for (int i = 0; i < HASH_SIZE; i++)
			fwrite(&index, sizeof(index), 1, fp1);
		fclose(fp1);
		fclose(fp2);
	}
	else {
		cout << "Db has been found.\n";
		fclose(fp1);
		fclose(fp2);
	}
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


int DB::open() {
	cout << "Db has been open.\n";
    string idxPath = "./" + idxName;
    string datPath = "./" + datName;
	if ((fp1 = fopen(idxPath.c_str(), "rb+")) == NULL) {
		cout << "Cannot open index file.\n";
		return 0;
	}
	if ((fp2 = fopen(datPath.c_str(), "rb+")) == NULL) {
		cout << "Cannot open data file .\n";
		return 0;
	}
	int num;
	fread(&num, sizeof(int), 1, fp1);
	last_idx_off = num;
	fread(&num, sizeof(int), 1, fp2);
	last_dat_off = num;
	cout << "last_idx_off is : " << last_idx_off << endl;
	cout << "last_dat_off is : " << last_dat_off << endl;
	return 1;
}

int DB::close() {
	cout << "last_idx_off is : " << last_idx_off << endl;
	cout << "last_dat_off is : " << last_dat_off << endl;
	rewind(fp1);
	rewind(fp2);
	fwrite(&last_dat_off, sizeof(int), 1, fp2);
	fwrite(&last_idx_off, sizeof(int), 1, fp1);
	fclose(fp1);
	fclose(fp2);
	cout << "Db has been closed.\n";
	return 1;
}


unsigned int DB::hash(const char* key) {
	unsigned int seed = 1313131;
	unsigned int h = 0;
	while (*key) {
		h = h*seed + (*key++);
	}
	return ((h & 0x7FFFFFFF) % HASH_SIZE);
}

vector<string>  DB::find(const char* key) {
	Idx* Idx_find = find_key(key);
    vector<string> val;
	if (Idx_find == NULL) {
		return val;
	}
	else {
		unsigned int n = Idx_find->value_off;
        int pos = 0;
        string v = "";
        for (int i = 0; i < 10; i++) {
            if (Idx_find->len_value[i] == 0)
                break;
            cout << "data lens : " <<  Idx_find->len_value[i] << endl;
            char* value = new char[Idx_find->len_value[i] + 1];
            fseek(fp2, sizeof(int) + Idx_find->value_off, pos);
            fread(value, sizeof(char), Idx_find->len_value[i] + 1, fp2);
            pos += Idx_find->len_value[i] + 1;
            v = v.insert(0, value);
            val.push_back(v);
            cout << "data is : " <<  value << endl;
            v = "";
        }
        /*
		char*value = new char[Idx_find->len_value + 1];
		fseek(fp2, sizeof(int) + Idx_find->value_off, 0);
		fread(value, sizeof(char), Idx_find->len_value + 1, fp2);
		return value;
         */
        return val;
	}
}

bool DB::del(char* key) {
	Idx* Idx_find = find_key(key);
	if (Idx_find == NULL) {
		return false;
	}
	else {
		Idx_find->isDelete = true;
		for (int i = 0; i < KEYSIZE_MAX; i++)
			Idx_find->key[i] = '\0';
		fseek(fp1, sizeof(int) + sizeof(Idx)*Idx_find->off, 0);
		fwrite(Idx_find, sizeof(Idx), 1, fp1);//renew index file
		fflush(fp1);
		return true;
	}
}

int DB::insert(char* key, vector<string> val) {
	if (find_key(key)) {
		return 0;
	}
	unsigned int off_new = hash(key);
	Idx Idx_new;
	fseek(fp1, sizeof(Idx)*off_new + sizeof(int), 0);
	fread(&Idx_new, sizeof(Idx), 1, fp1);
	if (Idx_new.off == 0) {
		//initialize the new Index struct
		Idx_new.len_key = strlen(key);
        int total_len = 0;
        int temp_i = 0;
        for(auto item: val){
            cout << "debug " << item << endl;
            Idx_new.len_value[temp_i] = item.length() + 1;
            total_len += item.length() + 1;
            temp_i++;
        }
		//Idx_new.len_value = strlen(value);
        last_idx_off += 1;
		Idx_new.off = off_new;
		Idx_new.isDelete = false;
		Idx_new.off_next = last_idx_off;
		Idx_new.value_off = last_dat_off;
        cout << "data :  " << Idx_new.value_off << endl;
		last_dat_off += (total_len + ) * sizeof(char);
        cout<< "length of written data:" << last_dat_off <<endl;
        cout<<"length of vector" << sizeof(val)<<endl;
		for (int i = 0; i < Idx_new.len_key; i++) {
			Idx_new.key[i] = key[i];
		}
		fseek(fp1, sizeof(Idx)*Idx_new.off + sizeof(int), 0);
		fwrite(&Idx_new, sizeof(Idx), 1, fp1);
		int pos = 0;
        temp_i=0;
        for(auto item: val){
            fseek(fp2, sizeof(int) + Idx_new.value_off, pos);
            fwrite(item.c_str(), sizeof(char), Idx_new.len_value[temp_i] + 1, fp2);
            pos += Idx_new.len_value[temp_i] + 1;
            temp_i++;
        }
		//fwrite(value, sizeof(char), Idx_new.len_value + 1, fp2);
		fflush(fp1);
		fflush(fp2);
		return 1;
	}
	if (Idx_new.isDelete) {
		Idx_new.len_key = strlen(key);
        int total_len = 0;
        for (int i = 0; i < val.size(); i++) {
            Idx_new.len_value[i] = val[i].length() + 1;
            total_len += val[i].length() + 1;
        }
		//Idx_new.len_value = strlen(value);
		Idx_new.isDelete = false;
		Idx_new.value_off = last_dat_off;
		last_dat_off += (total_len + 1) * sizeof(char);
		for (int i = 0; i < Idx_new.len_key; i++) {
			Idx_new.key[i] = key[i];
		}
		fseek(fp1, sizeof(Idx)*Idx_new.off + sizeof(int), 0);
		fwrite(&Idx_new, sizeof(Idx), 1, fp1);
        int pos = 0;
        for (int i = 0; i < val.size(); i++) {
            fseek(fp2, sizeof(int) + Idx_new.value_off, pos);
            fwrite(val[i].c_str(), sizeof(char), Idx_new.len_value[i] + 1, fp2);
            pos += Idx_new.len_value[i] + 1;
        }
		//fseek(fp2, sizeof(int) + Idx_new.value_off, 0);
		//fwrite(value, sizeof(char), Idx_new.len_value + 1, fp2);
		fflush(fp1);
		fflush(fp2);
		return 1;
	}
	else {
		while (true) {
			fseek(fp1, sizeof(Idx)*Idx_new.off_next + sizeof(int), 0);
			int flag = fread(&Idx_new, sizeof(Idx), 1, fp1);
			if (flag == 0) { //no index in this place
				Idx_new.len_key = strlen(key);
                int total_len = 0;
                for (int i = 0; i < val.size(); i++) {
                    Idx_new.len_value[i] = val[i].length() + 1;
                    total_len += val[i].length() + 1;
                }
				//Idx_new.len_value = strlen(value);
				Idx_new.off = last_idx_off;
				last_idx_off += 1;
				Idx_new.isDelete = false;
				Idx_new.off_next = last_idx_off;
				Idx_new.value_off = last_dat_off;
				last_dat_off += (total_len + 1) * sizeof(char);
				for (int i = 0; i < Idx_new.len_key; i++) {
					Idx_new.key[i] = key[i];
				}
				fseek(fp1, sizeof(Idx)*Idx_new.off + sizeof(int), 0);
				fwrite(&Idx_new, sizeof(Idx), 1, fp1);
                int pos = 0;
                for (int i = 0; i < val.size(); i++) {
                    fseek(fp2, sizeof(int) + Idx_new.value_off, pos);
                    fwrite(val[i].c_str(), sizeof(char), Idx_new.len_value[i] + 1, fp2);
                    pos += Idx_new.len_value[i] + 1;
                }
				//fseek(fp2, sizeof(int) + Idx_new.value_off, 0);
				//fwrite(value, sizeof(char), Idx_new.len_value + 1, fp2);
				fflush(fp1);
				fflush(fp2);
				return 1;
			}
			if (Idx_new.isDelete) {
				Idx_new.len_key = strlen(key);
                int total_len = 0;
                for (int i = 0; i < val.size(); i++) {
                    Idx_new.len_value[i] = val[i].length() + 1;
                    total_len += val[i].length() + 1;
                }
				//Idx_new.len_value = strlen(value);
				Idx_new.isDelete = false;
				Idx_new.value_off = last_dat_off;
				last_dat_off += (total_len + 1) * sizeof(char);
				for (int i = 0; i < Idx_new.len_key; i++) {
					Idx_new.key[i] = key[i];
				}
				fseek(fp1, sizeof(Idx)*Idx_new.off + sizeof(int), 0);
				fwrite(&Idx_new, sizeof(Idx), 1, fp1);
                int pos = 0;
                for (int i = 0; i < val.size(); i++) {
                    fseek(fp2, sizeof(int) + Idx_new.value_off, pos);
                    fwrite(val[i].c_str(), sizeof(char), Idx_new.len_value[i] + 1, fp2);
                    pos += Idx_new.len_value[i] + 1;
                }
				//fseek(fp2, sizeof(int) + Idx_new.value_off, 0);
				//fwrite(value, sizeof(char), Idx_new.len_value + 1, fp2);
				fflush(fp1);
				fflush(fp2);
				return 1;
			}
		}
	}
}

void DB::clear() {
    string idxPath = "./" + idxName;
    string datPath = "./" + datName;
    remove(idxPath.c_str());
    remove(datPath.c_str());
}


/*
bool DB::replace(char*key, char*value) {
	//cout << "replace function\n";
	Idx* Idx_find = find_key(key);
	if (Idx_find == NULL) {
		return false;
	}
	else {
		unsigned int n = Idx_find->value_off;
		Idx_find->len_value = strlen(value);
		Idx_find->value_off = last_dat_off;
		last_dat_off += (Idx_find->len_value + 1) * sizeof(char);
		fseek(fp2, sizeof(int) + Idx_find->value_off, 0);
		fwrite(value, sizeof(char), Idx_find->len_value + 1, fp2);//replace value
		fseek(fp1, sizeof(int) + sizeof(Idx)*Idx_find->off, 0);
		fwrite(Idx_find, sizeof(Idx), 1, fp1);//renew index file
		fflush(fp1);
		fflush(fp2);
		return true;
	}
}
 */

Idx* DB::find_key(const char* key) {
	int len = strlen(key);
	unsigned int off_find = hash(key);
	Idx* Idx_find = new Idx;

	while (off_find) {
		fseek(fp1, (sizeof(Idx) * off_find) + sizeof(int), 0);
		if (fread(Idx_find, sizeof(Idx), 1, fp1) == 0) {
			return NULL;

		}
		if (Idx_find->isDelete) {
			off_find = Idx_find->off_next;
			continue;
		}
		
		if (len != Idx_find->len_key) {
			off_find = Idx_find->off_next;
			continue;
		}
		bool flag = true;
		for (int i = 0; i < len; i++) {
			if (key[i] != Idx_find->key[i]) {
				flag = false;
				break;
			}
		}
		if (flag ) {
			return Idx_find;
		}
		off_find = Idx_find->off_next;
	}
	return NULL;
}

/*
void DB::traversal() {
	fseek(fp1, sizeof(int), 0);
	Idx idx;
	int i = 1;
	while (fread(&idx, sizeof(Idx), 1, fp1) != 0) {
		if ((idx.isDelete) || (idx.off == 0))
			continue;
		char*value = new char[idx.len_value + 1];
		fseek(fp2, sizeof(int) + idx.value_off, 0);
		fread(value, sizeof(char), idx.len_value + 1, fp2);
		cout <<i<< " key-------- " << idx.key << " ---------value-------- " << value << endl;
		++i;
		delete value;
	}
}
 */

