#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    for (unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidText(skey));
}

wstring modAlphaCipher::getValidText(const wstring& s)
{
    wstring tmp;
    locale loc("ru_RU.UTF-8");
    
    for (auto c : s) {
        if (isalpha(c, loc)) {
            tmp += toupper(c, loc);
        }
    }
    return tmp;
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
    vector<int> work = convert(getValidText(open_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    vector<int> work = convert(getValidText(cipher_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}
