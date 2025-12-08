#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

/**
 * @brief Класс исключений для шифра
 */
class cipher_error: public invalid_argument {
public:
    explicit cipher_error (const string& what_arg):
        invalid_argument(what_arg) {}
    explicit cipher_error (const char* what_arg):
        invalid_argument(what_arg) {}
};

/**
 * @brief Класс для реализации шифра гаммирования на основе русского алфавита
 */
class modAlphaCipher
{
    
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; /**< Русский алфавит в верхнем регистре */
    map<wchar_t, int> alphaIndex; /**< Отображение символов на их индексы в алфавите */
    vector<int> keySeq; /**< Последовательность числовых значений ключа */
    vector<int> toNums(const wstring& s);
    wstring toStr(const vector<int>& v);
    wstring getValidKey(const wstring& s);
    wstring getValidOpenText(const wstring& s);
    wstring getValidCipherText(const wstring& s);
    
public:
    modAlphaCipher() = delete; /**< Удаленный конструктор по умолчанию */
    modAlphaCipher(const wstring& keyStr);
    wstring encrypt(const wstring& plain);
    wstring decrypt(const wstring& cipher);
};
