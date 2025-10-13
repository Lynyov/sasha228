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

int main(int argc, char **argv)
{
    locale::global(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));
    cout.imbue(locale("ru_RU.UTF-8"));
    
    string key_str;
    string text_str;
    unsigned op;
    
    try {
        cout << "Шифр Гронсфельда" << endl;
        cout << "Введите ключ (только русские буквы): ";
        getline(cin, key_str);
        
        if (key_str.empty()) {
            cerr << "Ошибка: ключ не может быть пустым" << endl;
            return 1;
        }
        
        wstring key = stringToWstring(key_str);
        modAlphaCipher cipher(key);
        cout << "Ключ загружен" << endl;
        
        do {
            cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            
            if (op > 2) {
                cout << "Неверная операция" << endl;
            } else if (op > 0) {
                cout << "Введите текст: ";
                cin.ignore();
                getline(cin, text_str);
                
                if (text_str.empty()) {
                    cout << "Ошибка: текст не может быть пустым" << endl;
                    continue;
                }
                
                try {
                    wstring text = stringToWstring(text_str);
                    
                    if (op == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка обработки: " << e.what() << endl;
                }
            }
        } while (op != 0);
        
        cout << "До свидания!" << endl;
        
    } catch (const cipher_error& e) {
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        cerr << "Ключ должен содержать только русские буквы!" << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
