#include "routeCipher.h"
#include <algorithm>
using namespace std;

Table::Table(int key)
{
    if (key <= 0) throw invalid_argument("Ключ должен быть > 0");
    columns = key;
}

wstring Table::decrypt(const wstring& cipher)
{
    if (cipher.empty()) throw invalid_argument("Текст не может быть пустым");

    int length = static_cast<int>(cipher.length());
    int rows = (length + columns - 1) / columns;
    int remainder = length % columns;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(columns, 0));
    int position = 0;

    for (int c = columns - 1; c >= 0; --c) {
        int height = rows;
        if (remainder != 0 && c >= remainder) height = rows - 1;
        for (int r = 0; r < height; ++r) {
            grid[r][c] = cipher[position++];
        }
    }

    wstring result;
    result.reserve(length);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (grid[r][c] != 0) result += grid[r][c];
        }
    }
    return result;
}

wstring Table::encrypt(const wstring& plain)
{
    if (plain.empty()) throw invalid_argument("Текст не может быть пустым");

    int length = static_cast<int>(plain.length());
    int rows = (length + columns - 1) / columns;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(columns, 0));
    int position = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (position < length) grid[r][c] = plain[position++];
        }
    }

    wstring result;
    result.reserve(length);
    for (int c = columns - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != 0) result += grid[r][c];
        }
    }
    return result;
}
