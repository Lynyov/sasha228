#include "modAlphaCipher.h"
using namespace std;

/**
 * @brief Конструктор класса modAlphaCipher
 * @param keyStr Ключевая строка для шифрования
 */
modAlphaCipher::modAlphaCipher(const wstring& keyStr)
{
    for (unsigned k = 0; k < alphabet.size(); ++k) {
        alphaIndex[alphabet[k]] = k;
    }
    keySeq = toNums(getValidKey(keyStr));
}

/**
 * @brief Преобразует строку в вектор числовых индексов
 * @param s Исходная строка
 * @return Вектор индексов символов в алфавите
 */
vector<int> modAlphaCipher::toNums(const wstring& s)
{
    vector<int> resultNums;
    resultNums.reserve(s.size());
    for (auto sym : s) {
        resultNums.push_back(alphaIndex[sym]);
    }
    return resultNums;
}

/**
 * @brief Преобразует вектор числовых индексов обратно в строку
 * @param v Вектор индексов символов
 * @return Строка, соответствующая индексам
 */
wstring modAlphaCipher::toStr(const vector<int>& v)
{
    wstring resultStr;
    resultStr.reserve(v.size());
    for (auto idx : v) {
        resultStr.push_back(alphabet[idx]);
    }
    return resultStr;
}

/**
 * @brief Проверяет и нормализует ключевую строку
 * @param s Исходная ключевая строка
 * @return Валидированная и нормализованная ключевая строка
 * @throw cipher_error Если ключ пуст, содержит пробелы, недопустимые символы или является слабым
 */
wstring modAlphaCipher::getValidKey(const wstring& s)
{
    wstring tmp;
    for (auto c : s) {
        if (!iswspace(c)) {
            tmp.push_back(c);
        }
    }
    
    if (tmp.empty())
        throw cipher_error("Empty key");
    
    // Проверка на пробелы в исходной строке
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in key");
        }
    }
    
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto & c : tmp) {
        // Преобразование строчных в прописные
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            c = upper[pos];
        } else if (alphabet.find(c) == wstring::npos) {
            throw cipher_error("Invalid key");
        }
    }
    
    // Проверка на вырожденный ключ (все символы одинаковые)
    bool allSame = true;
    if (tmp.size() > 1) {
        wchar_t firstChar = tmp[0];
        for (size_t i = 1; i < tmp.size(); ++i) {
            if (tmp[i] != firstChar) {
                allSame = false;
                break;
            }
        }
        if (allSame) {
            throw cipher_error("Weak key");
        }
    }
    
    return tmp;
}

/**
 * @brief Подготавливает открытый текст для шифрования
 * @param s Исходный открытый текст
 * @return Очищенный и нормализованный открытый текст
 * @throw cipher_error Если текст пуст после очистки
 */
wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto c : s) {
        if (!iswspace(c)) { // Игнорируем пробелы
            if (alphabet.find(c) != wstring::npos) {
                tmp.push_back(c);
            } else {
                
                size_t pos = lower.find(c);
                if (pos != wstring::npos) {
                    tmp.push_back(upper[pos]); 
                }
            }
        }
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

/**
 * @brief Проверяет валидность зашифрованного текста
 * @param s Зашифрованный текст для проверки
 * @return Валидированный зашифрованный текст
 * @throw cipher_error Если текст пуст, содержит пробелы или недопустимые символы
 */
wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    wstring tmp;
    
    // Проверяем есть ли пробелы или недопустимые символы
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in cipher text");
        }
        if (alphabet.find(c) == wstring::npos) {
            throw cipher_error("Invalid character in cipher text");
        }
        tmp.push_back(c);
    }
    
    if (tmp.empty())
        throw cipher_error("Empty cipher text");
    
    return tmp;
}

/**
 * @brief Шифрует текст методом гаммирования
 * @param plain Открытый текст для шифрования
 * @return Зашифрованный текст
 */
wstring modAlphaCipher::encrypt(const wstring& plain)
{
    vector<int> tmp = toNums(getValidOpenText(plain));
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}

/**
 * @brief Расшифровывает текст, зашифрованный методом гаммирования
 * @param cipher Зашифрованный текст
 * @return Расшифрованный текст
 */
wstring modAlphaCipher::decrypt(const wstring& cipher)
{
    vector<int> tmp = toNums(getValidCipherText(cipher));
    for (unsigned p = 0; p < tmp.size(); ++p) {
        tmp[p] = (tmp[p] + alphabet.size() - keySeq[p % keySeq.size()]) % alphabet.size();
    }
    return toStr(tmp);
}
