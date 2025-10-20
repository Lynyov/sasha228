#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
using namespace std;

class modAlphaCipher
{
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> charToIndex;
    vector<int> keySequence;
    vector<int> convert_to_numbers(const wstring& s);
    wstring convert_to_string(const vector<int>& v);
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& keyStr);
    wstring encrypt(const wstring& plain);
    wstring decrypt(const wstring& cipher);
};
