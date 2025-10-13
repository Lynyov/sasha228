#include <iostream>
#include <cctype>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;
wstring stringToWstring(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstringToString(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

bool isValid(const wstring& s)
{
    locale loc("ru_RU.UTF-8");
    for(auto c : s) {
        if (!isalpha(c, loc) || !isupper(c, loc)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    locale::global(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));
    cout.imbue(locale("ru_RU.UTF-8"));
    
    string key_str;
    string text_str;
    unsigned op;
    
    cout << "Шифр готов. Введите ключ: ";
    cin >> key_str;
    
    wstring key = stringToWstring(key_str);
    if (!isValid(key)) {
        cerr << "Ключ недействителен! Должны быть только русские буквы в верхнем регистре." << endl;
        return 1;
    }
    
    cout << "Ключ загружен" << endl;
    modAlphaCipher cipher(key);
    
    do {
        cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
        cin >> op;
        
        if (op > 2) {
            cout << "Неверная операция" << endl;
        } else if (op > 0) {
            cout << "Введите текст: ";
            cin.ignore();
            getline(cin, text_str);
            
            wstring text = stringToWstring(text_str);
            if (isValid(text)) {
                if (op == 1) {
                    wstring encrypted = cipher.encrypt(text);
                    cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                } else {
                    wstring decrypted = cipher.decrypt(text);
                    cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                }
            } else {
                cout << "Операция отменена: неверный текст. Используйте только русские буквы." << endl;
            }
        }
    } while (op != 0);
    
    return 0;
}
