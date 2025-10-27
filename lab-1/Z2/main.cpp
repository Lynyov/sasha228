#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "routeCipher.h"
using namespace std;

wstring convertToUpper(const wstring& s)
{
    wstring res = s;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto& c : res) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) c = upper[pos];
    }
    return res;
}

bool checkCyrillicOnly(const wstring& s)
{
    wstring ABC = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto ch : s) {
        if (iswspace(ch)) continue;
        if (ABC.find(ch) == wstring::npos) return false;
    }
    return true;
}

wstring removeSpaces(const wstring& s)
{
    wstring out;
    out.reserve(s.size());
    for (wchar_t ch : s)
        if (!iswspace(ch))
            out.push_back(ch);
    return out;
}

string wideToUtf8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

wstring utf8ToWide(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string inputKey;
    string inputText;
    unsigned mode;

    cout << "Введите число столбцов: ";
    getline(cin, inputKey);

    int columns = 0;
    try {
        columns = stoi(inputKey);
    } catch (...) {
        cerr << "Некорректное число столбцов." << endl;
        return 1;
    }
    if (columns <= 0) {
        cerr << "Число столбцов должно быть положительным." << endl;
        return 1;
    }

    Table cipher(columns);
    do {
        cout << "Выберите режим:\n";
        cout << "0 — выход\n";
        cout << "1 — шифрование\n";
        cout << "2 — расшифрование\n";
        if (!(cin >> mode)) return 0;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (mode > 2) {
            cout << "Неверный выбор режима." << endl;
        } else if (mode > 0) {
            cout << "Введите строку: ";
            getline(cin, inputText);

            wstring wideText = convertToUpper(utf8ToWide(inputText));
            wstring cleanedText = removeSpaces(wideText);

            if (checkCyrillicOnly(cleanedText)) {
                if (mode == 1) {
                    wstring encrypted = cipher.encrypt(cleanedText);
                    cout << "Зашифровано: " << wideToUtf8(encrypted) << endl;
                } else {
                    wstring decrypted = cipher.decrypt(cleanedText);
                    cout << "Расшифровано: " << wideToUtf8(decrypted) << endl;
                }
            } else {
                cout << "Можно использовать только русские буквы." << endl;
            }
        }
    } while (mode != 0);

    return 0;
}
