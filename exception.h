//
// Created by zhouc on 2022/4/24.
//
#include <iostream>
#include <string>
#include <string_view>
using namespace std;

#ifndef UNTITLED1_EXCEPTION_H
#define UNTITLED1_EXCEPTION_H

#endif //UNTITLED1_EXCEPTION_H
class Myexcept
{
protected:
    string message;

public:
    Myexcept(string_view str = "There may be a problem") : message{str} {}
    virtual ~Myexcept() = default;
    virtual string_view what() const {
        return message;
    }
};

class InvalidNumber:public Myexcept    // the input value is a number but not a valid number
{
public:
    InvalidNumber(string_view str = "Please input a valid number") : Myexcept{str} {
        cout << "Please input a valid number" << '\n' << endl;
    }
};

class InvalidType:public Myexcept      // the input value is not a number
{
public:
    InvalidType(string_view str = "Please input a valid type") : Myexcept{str} {
        cout << "Please input a valid type" << '\n' << endl;
        cout << "Please input a valid number:" << '\n' << endl;
    }
};