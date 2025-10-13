#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;

bool modAlphaCipher::isRussianAlpha(wchar_t c)
{
    return (c >= L'А' && c <= L'Я') || (c >= L'а' && c <= L'я') || c == L'Ё' || c == L'ё';
}

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    for (unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));
}

wstring modAlphaCipher::getValidKey(const wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    wstring tmp;
    for (auto c : s) {
        if (isRussianAlpha(c)) {
            if (c >= L'а' && c <= L'я') {
                tmp += (c - L'а' + L'А');
            } else if (c == L'ё') {
                tmp += L'Ё';
            } else {
                tmp += c;
            }
        } else {
            throw cipher_error("Ключ должен содержать только русские буквы");
        }
    }
    
    bool all_same = true;
    for (size_t i = 1; i < tmp.size(); i++) {
        if (tmp[i] != tmp[0]) {
            all_same = false;
            break;
        }
    }
    if (all_same) {
        throw cipher_error("Слабый ключ: все символы одинаковые");
    }
    
    return tmp;
}

wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring tmp;
    
    for (auto c : s) {
        if (isRussianAlpha(c)) {
            if (c >= L'а' && c <= L'я') {
                tmp += (c - L'а' + L'А');
            } else if (c == L'ё') {
                tmp += L'Ё';
            } else {
                tmp += c;
            }
        }
    }
    
    if (tmp.empty()) {
        throw cipher_error("Текст для шифрования должен содержать русские буквы");
    }
    
    return tmp;
}

wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой зашифрованный текст");
    }
    
    for (auto c : s) {
        if (!isRussianAlpha(c) || !(c >= L'А' && c <= L'Я') || c == L'Ё') {
            throw cipher_error("Зашифрованный текст должен содержать только русские заглавные буквы");
        }
    }
    
    return s;
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for (auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for (auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    vector<int> work = convert(getValidOpenText(open_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    vector<int> work = convert(getValidCipherText(cipher_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}
