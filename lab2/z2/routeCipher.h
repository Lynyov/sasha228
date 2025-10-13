#ifndef TABLE_CIPHER_H
#define TABLE_CIPHER_H

#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class route_cipher_error : public invalid_argument {
public:
    explicit route_cipher_error(const string& what_arg) : invalid_argument(what_arg) {}
    explicit route_cipher_error(const char* what_arg) : invalid_argument(what_arg) {}
};

class TableCipher
{
private:
    int columns;
    wstring getValidText(const wstring& s);
    
public:
    TableCipher(int col_num);
    wstring encrypt(const wstring& original_text);
    wstring decrypt(const wstring& encrypted_data);
};

#endif
