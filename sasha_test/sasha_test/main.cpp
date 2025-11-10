#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;

wstring convertStringToWide(const string& inputString)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(inputString);
}

string convertWideToString(const wstring& wideString)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wideString);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string userKey;
    string userMessage;
    unsigned userChoice;

    cout << "Введите ключ шифрования: ";
    getline(cin, userKey);

    try {
        AlphaCipher cipher(convertStringToWide(userKey));
        cout << "Ключ успешно установлен." << endl;

        do {
            cout << "Выберите режим:\n";
            cout << "0 — выход\n";
            cout << "1 — шифрование\n";
            cout << "2 — расшифрование\n";
            if (!(cin >> userChoice))
                return 0;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (userChoice > 2) {
                cout << "Неверный выбор операции." << endl;
            } else if (userChoice > 0) {
                cout << "Введите текст: ";
                getline(cin, userMessage);

                try {
                    if (userChoice == 1) {
                        wstring encodedText = cipher.encodeText(convertStringToWide(userMessage));
                        cout << "Результат шифрования: " << convertWideToString(encodedText) << endl;
                    } else {
                        wstring decodedText = cipher.decodeText(convertStringToWide(userMessage));
                        cout << "Результат расшифровки: " << convertWideToString(decodedText) << endl;
                    }
                } catch (const CipherException& error) {
                    cerr << "Ошибка обработки текста: " << error.what() << endl;
                }
            }
        } while (userChoice != 0);
        
    } catch (const CipherException& error) {
        cerr << "Ошибка инициализации шифратора: " << error.what() << endl;
        return 1;
    }

    return 0;
}