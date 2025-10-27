#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

class CipherException : public invalid_argument {
public:
    explicit CipherException(const string& message) : invalid_argument(message) {}
    explicit CipherException(const char* message) : invalid_argument(message) {}
};

class AlphaCipher
{
private:
    wstring russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> charToNumber;
    vector<int> encryptionKey;
    
    vector<int> convertTextToNumbers(const wstring& text);
    wstring convertNumbersToText(const vector<int>& numbers);
    wstring validateEncryptionKey(const wstring& key);
    wstring validatePlainText(const wstring& text);
    wstring validateCipherText(const wstring& text);
    
public:
    AlphaCipher() = delete;
    AlphaCipher(const wstring& keyText);
    wstring encodeText(const wstring& plainText);
    wstring decodeText(const wstring& cipherText);
};