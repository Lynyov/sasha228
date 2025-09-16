#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class modAlphaCipher
{
private:
    wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> alphaNum;
    vector<int> key;
    vector<int> convert(const wstring& s);
    wstring convert(const vector<int>& v);
    wstring getValidText(const wstring& s);
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& skey);
    wstring encrypt(const wstring& open_text);
    wstring decrypt(const wstring& cipher_text);
};

#endif
