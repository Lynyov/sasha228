#include "table.h"
#include <algorithm>
#include <vector>
#include <cwctype>  
using namespace std;

/**
 * @brief Проверяет валидность ключа (количества столбцов)
 * @param key Ключ - количество столбцов таблицы
 * @return Валидный ключ
 * @throw cipher_error Если ключ неположительный
 */
 
int Table::getValidKey(const int key)
{
    if (key <= 0)
        throw cipher_error("Invalid key: key must be positive");
    return key;
}

/**
 * @brief Подготавливает открытый текст для шифрования
 * @param s Исходная строка открытого текста
 * @return Очищенный и преобразованный открытый текст
 * @throw cipher_error Если текст пуст после очистки
 */
 
wstring Table::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto c : s) {
        if (!iswspace(c)) { 
            if (upper.find(c) != wstring::npos) {
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
 * @param s Строка зашифрованного текста
 * @return Очищенный зашифрованный текст
 * @throw cipher_error Если текст пуст, содержит пробелы или недопустимые символы
 */
 
wstring Table::getValidCipherText(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Empty cipher text");
    
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in cipher text");
        }
    }
    
    wstring tmp;
    for (auto c : s) {
        tmp.push_back(c);
    }
    
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";  
    for (auto c : tmp) {
        if (upper.find(c) == wstring::npos)
            throw cipher_error("Invalid cipher text");
    }
    return tmp;
}

/**
 * @brief Конструктор класса Table
 * @param key Количество столбцов в таблице
 */

Table::Table(int key)
{
    cols = getValidKey(key);
}

/**
 * @brief Шифрует текст методом табличной перестановки
 * @param plain Открытый текст для шифрования
 * @return Зашифрованный текст
 */

wstring Table::encrypt(const wstring& plain)
{
    wstring validText = getValidOpenText(plain);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    
    for (int c = cols - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}

/**
 * @brief Расшифровывает текст, зашифрованный методом табличной перестановки
 * @param cipher Зашифрованный текст
 * @return Расшифрованный текст
 */

wstring Table::decrypt(const wstring& cipher)
{
    wstring validText = getValidCipherText(cipher);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;
    
    int fullCols = n % cols;
    if (fullCols == 0) fullCols = cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    for (int c = cols - 1; c >= 0; --c) {
        int h = rows;
    
        if (c >= fullCols) {
            h = rows - 1;
        }
        for (int r = 0; r < h; ++r) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
