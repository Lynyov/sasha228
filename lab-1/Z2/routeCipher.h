#pragma once
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class Table
{
private:
    int columns;
public:
    explicit Table(int key);
    wstring decrypt(const wstring& cipher);
    wstring encrypt(const wstring& plain);
};
