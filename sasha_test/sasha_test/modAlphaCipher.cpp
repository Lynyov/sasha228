#include "modAlphaCipher.h"
using namespace std;

AlphaCipher::AlphaCipher(const wstring& keyText)
{
    for (unsigned index = 0; index < russianAlphabet.size(); ++index) {
        charToNumber[russianAlphabet[index]] = index;
    }
    encryptionKey = convertTextToNumbers(validateEncryptionKey(keyText));
}

vector<int> AlphaCipher::convertTextToNumbers(const wstring& text)
{
    vector<int> numberSequence;
    numberSequence.reserve(text.size());
    for (auto character : text) {
        numberSequence.push_back(charToNumber[character]);
    }
    return numberSequence;
}

wstring AlphaCipher::convertNumbersToText(const vector<int>& numbers)
{
    wstring textResult;
    textResult.reserve(numbers.size());
    for (auto number : numbers) {
        textResult.push_back(russianAlphabet[number]);
    }
    return textResult;
}

wstring AlphaCipher::validateEncryptionKey(const wstring& key)
{
    wstring processedKey;
    for (auto character : key) {
        if (!iswspace(character)) {
            processedKey.push_back(character);
        }
    }
    
    if (processedKey.empty())
        throw CipherException("Ключ не может быть пустым");
    
    wstring lowercaseLetters = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring uppercaseLetters = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto & character : processedKey) {
        size_t position = lowercaseLetters.find(character);
        if (position != wstring::npos) {
            character = uppercaseLetters[position];
        } else if (russianAlphabet.find(character) == wstring::npos) {
            throw CipherException("Ключ содержит недопустимые символы");
        }
    }
    return processedKey;
}

wstring AlphaCipher::validatePlainText(const wstring& text)
{
    wstring processedText;
    wstring lowercaseLetters = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring uppercaseLetters = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto character : text) {
        if (!iswspace(character)) {
            if (russianAlphabet.find(character) != wstring::npos) {
                processedText.push_back(character);
            } else {
                size_t position = lowercaseLetters.find(character);
                if (position != wstring::npos) {
                    processedText.push_back(uppercaseLetters[position]);
                }
            }
        }
    }
    
    if (processedText.empty())
        throw CipherException("Текст для шифрования не может быть пустым");
        
    return processedText;
}

wstring AlphaCipher::validateCipherText(const wstring& text)
{
    wstring processedText;
    for (auto character : text) {
        if (!iswspace(character)) {
            processedText.push_back(character);
        }
    }
    
    if (processedText.empty())
        throw CipherException("Зашифрованный текст не может быть пустым");
    
    for (auto character : processedText) {
        if (russianAlphabet.find(character) == wstring::npos)
            throw CipherException("Зашифрованный текст содержит недопустимые символы");
    }
    
    return processedText;
}

wstring AlphaCipher::encodeText(const wstring& plainText)
{
    vector<int> numberSequence = convertTextToNumbers(validatePlainText(plainText));
    for (unsigned position = 0; position < numberSequence.size(); ++position) {
        numberSequence[position] = (numberSequence[position] + encryptionKey[position % encryptionKey.size()]) % russianAlphabet.size();
    }
    return convertNumbersToText(numberSequence);
}

wstring AlphaCipher::decodeText(const wstring& cipherText)
{
    vector<int> numberSequence = convertTextToNumbers(validateCipherText(cipherText));
    for (unsigned position = 0; position < numberSequence.size(); ++position) {
        numberSequence[position] = (numberSequence[position] + russianAlphabet.size() - encryptionKey[position % encryptionKey.size()]) % russianAlphabet.size();
    }
    return convertNumbersToText(numberSequence);
}