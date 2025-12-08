#pragma once
#include <string>
#include <vector>
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
 * @brief Класс для реализации табличного шифра (метод перестановки)
 */
class Table
{
    
private:
    int cols; /**< Количество столбцов в таблице */
    int getValidKey(const int key);
    wstring getValidOpenText(const wstring& s);
    wstring getValidCipherText(const wstring& s);
    
public:
    explicit Table(int key);
    wstring encrypt(const wstring& plain);
    wstring decrypt(const wstring& cipher);
};
