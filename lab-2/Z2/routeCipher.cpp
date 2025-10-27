#include "routeCipher.h"
#include <algorithm>
#include <vector>
#include <cwctype>  
using namespace std;

int RouteTable::validateTableKey(const int key)
{
    if (key <= 0)
        throw CipherException("Некорректный ключ: количество столбцов должно быть положительным числом");
    return key;
}

wstring RouteTable::validateInputText(const wstring& text)
{
    wstring processedText;
    wstring lowercaseLetters = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring uppercaseLetters = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto character : text) {
        if (!iswspace(character)) {
            if (uppercaseLetters.find(character) != wstring::npos) {
                processedText.push_back(character);
            } else {
                size_t position = lowercaseLetters.find(character);
                if (position != wstring::npos) {
                    processedText.push_back(uppercaseLetters[position]);
                }
            }
        }
    }
    
    if (processedText.empty())
        throw CipherException("Текст для обработки не может быть пустым");
        
    return processedText;
}

wstring RouteTable::validateOutputText(const wstring& text)
{
    wstring processedText;
    for (auto character : text) {
        if (!iswspace(character)) {
            processedText.push_back(character);
        }
    }
    
    if (processedText.empty())
        throw CipherException("Зашифрованный текст не может быть пустым");
    
    wstring uppercaseLetters = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto character : processedText) {
        if (uppercaseLetters.find(character) == wstring::npos)
            throw CipherException("Зашифрованный текст содержит недопустимые символы");
    }
    
    return processedText;
}

RouteTable::RouteTable(int key)
{
    columnCount = validateTableKey(key);
}

wstring RouteTable::encodeText(const wstring& plainText)
{
    wstring validText = validateInputText(plainText);
    int textLength = static_cast<int>(validText.length());
    int rowCount = (textLength + columnCount - 1) / columnCount;

    vector<vector<wchar_t>> characterGrid(rowCount, vector<wchar_t>(columnCount, L' '));
    int currentPosition = 0;

    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            if (currentPosition < textLength) {
                characterGrid[row][column] = validText[currentPosition++];
            }
        }
    }

    wstring resultText;
    resultText.reserve(textLength);
    
    for (int column = columnCount - 1; column >= 0; --column) {
        for (int row = 0; row < rowCount; ++row) {
            if (characterGrid[row][column] != L' ') {
                resultText += characterGrid[row][column];
            }
        }
    }
    
    return resultText;
}

wstring RouteTable::decodeText(const wstring& cipherText)
{
    wstring validText = validateOutputText(cipherText);
    int textLength = static_cast<int>(validText.length());
    int rowCount = (textLength + columnCount - 1) / columnCount;
    
    int fullColumns = textLength % columnCount;
    if (fullColumns == 0) fullColumns = columnCount;

    vector<vector<wchar_t>> characterGrid(rowCount, vector<wchar_t>(columnCount, L' '));
    int currentPosition = 0;

    for (int column = columnCount - 1; column >= 0; --column) {
        int currentHeight = rowCount;
        if (column >= fullColumns) {
            currentHeight = rowCount - 1;
        }
        for (int row = 0; row < currentHeight; ++row) {
            if (currentPosition < textLength) {
                characterGrid[row][column] = validText[currentPosition++];
            }
        }
    }

    wstring resultText;
    resultText.reserve(textLength);
    
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            if (characterGrid[row][column] != L' ') {
                resultText += characterGrid[row][column];
            }
        }
    }
    
    return resultText;
}