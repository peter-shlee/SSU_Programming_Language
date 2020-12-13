#ifndef __MAIN_H__
#define __MAIN_H__

#pragma warning (disable:4996)
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <stack>
using namespace std;

#define VARIABLE "VARIABLE"
#define INTEGER "INTEGER"
#define INTERMEDIATE_CODE_FILENAME "pl_hw2_20160548.obj"
enum {
    MINUS, IF, UNDEFINED
};
const char* FUN[];

struct  termNode {
    const char* type;
    termNode* param1;
    termNode* param2;
    int value;

    termNode() :type(NULL), param1(NULL), param2(NULL), value(0) {}

    void print() {
        cout << "type : " << type << endl;
        cout << "param1 : " << param1 << endl;
        cout << "param2 : " << param2 << endl;
        if (!strcmp(INTEGER, type))
            cout << "value : " << value << endl;
    }
};

#endif