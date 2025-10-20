#include "modAlphaCipher.h"
using namespace std;

vector<int> modAlphaCipher::convert_to_numbers(const wstring& s)
{
    vector<int> result;
    result.reserve(s.size());
    for (auto sym : s) {
        result.push_back(charToIndex[sym]);
    }
    return result;
}

wstring modAlphaCipher::convert_to_string(const vector<int>& v)
{
    wstring result;
    result.reserve(v.size());
    for (auto idx : v) {
        result.push_back(alphabet[idx]);
    }
    return result;
}

modAlphaCipher::modAlphaCipher(const wstring& keyStr)
{
    for (unsigned k = 0; k < alphabet.size(); ++k) {
        charToIndex[alphabet[k]] = k;
    }
    keySequence = convert_to_numbers(keyStr);
}

wstring modAlphaCipher::encrypt(const wstring& plain)
{
    vector<int> tmp = convert_to_numbers(plain);
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + keySequence[p % keySequence.size()]) % alphabet.size();
    }
    return convert_to_string(tmp);
}

wstring modAlphaCipher::decrypt(const wstring& cipher)
{
    vector<int> tmp = convert_to_numbers(cipher);
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + alphabet.size() - keySequence[p % keySequence.size()]) % alphabet.size();
    }
    return convert_to_string(tmp);
}
