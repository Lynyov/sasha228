#include <iostream>
#include <locale>
#include <codecvt>
#include "routeCipher.h"
using namespace std;

wstring convertToWide(const string& narrow_str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(narrow_str);
}

string convertToNarrow(const wstring& wide_str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}

wstring makeUpperCase(const wstring& input_str)
{
    wstring output_str = input_str;
    wstring lower_case = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper_case = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto& character : output_str) {
        size_t position = lower_case.find(character);
        if (position != wstring::npos) {
            character = upper_case[position];
        }
    }
    return output_str;
}

bool checkCyrillicOnly(const wstring& test_str)
{
    wstring cyrillic_chars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto character : test_str) {
        if (cyrillic_chars.find(character) == wstring::npos) {
            return false;
        }
    }
    return true;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string column_input;
    string user_input;
    unsigned choice;
    cout << "Введите количество столбцов: ";
    getline(cin, column_input);
    
    try {
        int column_count = stoi(column_input);
        
        if (column_count <= 0) {
            cerr << "Неверное значение ключа!" << endl;
            return 1;
        }
        
        cout << "Ключ принят" << endl;
        TableCipher processor(column_count);
        
        do {
            cout << "Выберите действие (0-выход, 1-шифровать, 2-расшифровать): ";
            cin >> choice;
            cin.ignore();
            
            if (choice > 2) {
                cout << "Недопустимая операция" << endl;
            } else if (choice > 0) {
                cout << "Введите текст: ";
                getline(cin, user_input);
                
                wstring processed_text = convertToWide(user_input);
                processed_text = makeUpperCase(processed_text);
                
                if (checkCyrillicOnly(processed_text)) {
                    if (choice == 1) {
                        wstring coded_text = processor.encrypt(processed_text);
                        cout << "Результат шифрования: " << convertToNarrow(coded_text) << endl;
                    } else {
                        wstring decoded_text = processor.decrypt(processed_text);
                        cout << "Результат расшифровки: " << convertToNarrow(decoded_text) << endl;
                    }
                } else {
                    cout << "Допустимы только русские буквы." << endl;
                }
            }
        } while (choice != 0);
        
    } catch (const exception& error) {
        cerr << "Произошла ошибка: " << error.what() << endl;
        return 1;
    }
    
    return 0;
}
