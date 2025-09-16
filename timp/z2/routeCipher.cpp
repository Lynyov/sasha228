#include "routeCipher.h"
#include <vector>
#include <algorithm>
using namespace std;

TableCipher::TableCipher(int col_num)
{
    if (col_num <= 0) {
        throw invalid_argument("Количество столбцов должно быть положительным");
    }
    columns = col_num;
}

wstring TableCipher::decrypt(const wstring& encrypted_data)
{
    if (encrypted_data.empty()) {
        throw invalid_argument("Пустой текст не допускается");
    }
    
    int data_length = encrypted_data.length();
    int row_count = data_length / columns;
    
    if (data_length % columns != 0) {
        throw invalid_argument("Неверный размер зашифрованных данных");
    }
    
    vector<vector<wchar_t>> decoding_grid(row_count, vector<wchar_t>(columns, L' '));
    int current_pos = 0;
    
    for (int col_index = columns - 1; col_index >= 0; col_index--) {
        for (int row_index = 0; row_index < row_count; row_index++) {
            decoding_grid[row_index][col_index] = encrypted_data[current_pos++];
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
    if (original_text.empty()) {
        throw invalid_argument("Пустой текст не допускается");
    }
    
    int text_size = original_text.length();
    int row_count = (text_size + columns - 1) / columns;
    vector<vector<wchar_t>> encoding_grid(row_count, vector<wchar_t>(columns, L' '));
    int current_index = 0;
    
    for (int row_index = 0; row_index < row_count; row_index++) {
        for (int col_index = 0; col_index < columns; col_index++) {
            if (current_index < text_size) {
                encoding_grid[row_index][col_index] = original_text[current_index++];
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
