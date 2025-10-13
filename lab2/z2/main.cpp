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
        TableCipher processor(column_count);
        cout << "Ключ принят" << endl;
        
        do {
            cout << "Выберите действие (0-выход, 1-шифровать, 2-расшифровать): ";
            cin >> choice;
            cin.ignore();
            
            if (choice > 2) {
                cout << "Недопустимая операция" << endl;
            } else if (choice > 0) {
                cout << "Введите текст: ";
                getline(cin, user_input);
                
                try {
                    wstring processed_text = convertToWide(user_input);
                    
                    if (choice == 1) {
                        wstring coded_text = processor.encrypt(processed_text);
                        cout << "Результат шифрования: " << convertToNarrow(coded_text) << endl;
                    } else {
                        wstring decoded_text = processor.decrypt(processed_text);
                        cout << "Результат расшифровки: " << convertToNarrow(decoded_text) << endl;
                    }
                } catch (const route_cipher_error& e) {
                    cout << "Ошибка обработки текста: " << e.what() << endl;
                }
            }
        } while (choice != 0);
        
    } catch (const route_cipher_error& e) {
        cerr << "Ошибка инициализации: " << e.what() << endl;
        return 1;
    } catch (const invalid_argument& e) {
        cerr << "Неверное значение ключа: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
