#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;

wstring convert_to_upper(const wstring& s)
{
    wstring res = s;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto& c : res) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos)
            c = upper[pos];
    }
    return res;
}

bool check_cyrillic_only(const wstring& s)
{
    wstring ABC = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto ch : s) {
        if (iswspace(ch)) continue;
        if (ABC.find(ch) == wstring::npos) return false;
    }
    return true;
}

wstring remove_spaces(const wstring& s)
{
    wstring out;
    out.reserve(s.size());
    for (wchar_t ch : s)
        if (!iswspace(ch))
            out.push_back(ch);
    return out;
}

string wide_to_utf8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

wstring utf8_to_wide(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string keyInput;
    string textInput;
    unsigned mode;

    cout << "Введите ключ: ";
    getline(cin, keyInput);
    wstring wideKey = convert_to_upper(utf8_to_wide(keyInput));

    if (!check_cyrillic_only(wideKey)) {
        cerr << "Некорректный ключ: допускаются лишь русские буквы." << endl;
        return 1;
    }
    cout << "Ключ сохранен." << endl;

    modAlphaCipher cipher(wideKey);

    do {
        cout << "Выберите режим:\n";
        cout << "0 — выход\n";
        cout << "1 — шифрование\n";
        cout << "2 — расшифрование\n";
        if (!(cin >> mode))
            return 0;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (mode > 2) {
            cout << "Неверный выбор режима." << endl;
        } else if (mode > 0) {
            cout << "Введите строку: ";
            getline(cin, textInput);

            wstring wideText = convert_to_upper(utf8_to_wide(textInput));
            wstring cleaned = remove_spaces(wideText);  

            if (check_cyrillic_only(cleaned)) {
                if (mode == 1) {
                    wstring encrypted = cipher.encrypt(cleaned);
                    cout << "Зашифровано: " << wide_to_utf8(encrypted) << endl;
                } else {
                    wstring decrypted = cipher.decrypt(cleaned);
                    cout << "Расшифровано: " << wide_to_utf8(decrypted) << endl;
                }
            } else {
                cout << "Некорректный ввод: используйте кириллицу." << endl;
            }
        }
    } while (mode != 0);

    return 0;
}
