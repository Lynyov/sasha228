#include "routeCipher.h"
#include <vector>
#include <algorithm>
using namespace std;

TableCipher::TableCipher(int col_num)
{
    if (col_num <= 0) {
        throw route_cipher_error("Количество столбцов должно быть положительным");
    }
    columns = col_num;
}

wstring TableCipher::getValidText(const wstring& s)
{
    if (s.empty()) {
        throw route_cipher_error("Пустой текст не допускается");
    }
    
    wstring tmp;
    wstring cyrillic_chars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    
    for (auto c : s) {
        if (cyrillic_chars.find(c) != wstring::npos) {
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
        throw route_cipher_error("Текст не содержит русских букв");
    }
    
    return tmp;
}

wstring TableCipher::decrypt(const wstring& encrypted_data)
{
    wstring valid_text = getValidText(encrypted_data);
    
    int data_length = valid_text.length();
    int row_count = data_length / columns;
    
    if (data_length % columns != 0) {
        throw route_cipher_error("Неверный размер зашифрованных данных");
    }
    
    vector<vector<wchar_t>> decoding_grid(row_count, vector<wchar_t>(columns, L' '));
    int current_pos = 0;
    
    for (int col_index = columns - 1; col_index >= 0; col_index--) {
        for (int row_index = 0; row_index < row_count; row_index++) {
            decoding_grid[row_index][col_index] = valid_text[current_pos++];
        }
    }
    
    wstring final_result;
    for (int row_index = 0; row_index < row_count; row_index++) {
        for (int col_index = 0; col_index < columns; col_index++) {
            final_result += decoding_grid[row_index][col_index];
        }
    }

    while (!final_result.empty() && final_result.back() == L'Я') {
        final_result.pop_back();
    }
    
    return final_result;
}

wstring TableCipher::encrypt(const wstring& original_text)
{
    wstring valid_text = getValidText(original_text);
    
    int text_size = valid_text.length();
    int row_count = (text_size + columns - 1) / columns;
    vector<vector<wchar_t>> encoding_grid(row_count, vector<wchar_t>(columns, L' '));
    int current_index = 0;
    
    for (int row_index = 0; row_index < row_count; row_index++) {
        for (int col_index = 0; col_index < columns; col_index++) {
            if (current_index < text_size) {
                encoding_grid[row_index][col_index] = valid_text[current_index++];
            } else {
                encoding_grid[row_index][col_index] = L'Я';
            }
        }
    }
    
    wstring encrypted_result;
    for (int col_index = columns - 1; col_index >= 0; col_index--) {
        for (int row_index = 0; row_index < row_count; row_index++) {
            encrypted_result += encoding_grid[row_index][col_index];
        }
    }
    
    return encrypted_result;
}
