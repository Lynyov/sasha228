#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

class cipher_error : public invalid_argument {
public:
    explicit cipher_error(const string& what_arg) : invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : invalid_argument(what_arg) {}
};

class modAlphaCipher
{
private:
    wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> alphaNum;
    vector<int> key;
    
    vector<int> convert(const wstring& s);
    wstring convert(const vector<int>& v);
    wstring getValidKey(const wstring& s);
    wstring getValidOpenText(const wstring& s);
    wstring getValidCipherText(const wstring& s);
    bool isRussianAlpha(wchar_t c);
    
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& skey);
    wstring encrypt(const wstring& open_text);
    wstring decrypt(const wstring& cipher_text);
};

#endif
