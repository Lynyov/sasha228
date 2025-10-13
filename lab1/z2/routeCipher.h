#ifndef TABLE_CIPHER_H
#define TABLE_CIPHER_H

#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class TableCipher
{
private:
    int columns;
public:
    TableCipher(int col_num);
    wstring encrypt(const wstring& original_text);
    wstring decrypt(const wstring& encrypted_data);
};

#endif
