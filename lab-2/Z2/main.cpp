#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include <string>
#include "routeCipher.h"
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
    string columnInput;
    string messageInput;
    unsigned operationChoice;

    cout << "Введите количество столбцов таблицы: ";
    getline(cin, columnInput);

    try {
        int columnCount = stoi(columnInput);
        RouteTable cipher(columnCount);
        cout << "Таблица для шифрования создана." << endl;

        do {
            cout << "Выберите режим:\n";
            cout << "0 — выход\n";
            cout << "1 — шифрование\n";
            cout << "2 — расшифрование\n";
            if (!(cin >> operationChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (operationChoice > 2) {
                cout << "Неверный выбор операции." << endl;
            } else if (operationChoice > 0) {
                cout << "Введите текст: ";
                getline(cin, messageInput);

                try {
                    if (operationChoice == 1) {
                        wstring encodedText = cipher.encodeText(convertStringToWide(messageInput));
                        cout << "Результат шифрования: " << convertWideToString(encodedText) << endl;
                    } else {
                        wstring decodedText = cipher.decodeText(convertStringToWide(messageInput));
                        cout << "Результат расшифровки: " << convertWideToString(decodedText) << endl;
                    }
                } catch (const CipherException& error) {
                    cerr << "Ошибка обработки текста: " << error.what() << endl;
                }
            }
        } while (operationChoice != 0);
        
    } catch (const CipherException& error) {
        cerr << "Ошибка инициализации шифратора: " << error.what() << endl;
        return 1;
    } catch (const exception& error) {
        cerr << "Ошибка: " << error.what() << endl;
        return 1;
    }
    
    return 0;
}