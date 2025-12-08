#include <UnitTest++/UnitTest++.h>
#include <string>
#include <locale>
#include <codecvt>
#include "table.h"

using namespace std;

/**
 * @brief Конвертирует широкую строку в UTF-8
 * @param ws Широкая строка
 * @return Строка в UTF-8
 */
string wideToUtf8(const wstring& ws) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

/**
 * @brief Конвертирует строку UTF-8 в широкую строку
 * @param s Строка в UTF-8
 * @return Широкая строка
 */
wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

/**
 * @brief Тесты конструктора класса Table
 */
SUITE(ConstructorTest)
{
    TEST(ValidKey) {
        Table cipher(2);
        CHECK_WIDE_EQUAL(L"ААСШ", cipher.encrypt(L"САША"));
    }
    
    TEST(LongKey) {
        Table cipher(9);
        CHECK_WIDE_EQUAL(L"КЕНАС", cipher.encrypt(L"САНЕК"));
    }
    
    TEST(KeyEqualsMessageLength) {
        Table cipher(4);
        CHECK_WIDE_EQUAL(L"АШАС", cipher.encrypt(L"САША"));
    }
    
    TEST(NegativeKey) {
        CHECK_THROW(Table cipher(-4), cipher_error);
    }
    
    TEST(ZeroKey) {
        CHECK_THROW(Table cipher(0), cipher_error);
    }
}

/**
 * @brief Фикстура для тестов с ключом 2
 */
struct Key3Fixture {
    Table* cipher;
    
    Key3Fixture() {
        cipher = new Table(2);
    }
    
    ~Key3Fixture() {
        delete cipher;
    }
};

/**
 * @brief Тесты шифрования класса Table
 */
SUITE(EncryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ААСШ", cipher->encrypt(L"САША"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_WIDE_EQUAL(L"ААСШ", cipher->encrypt(L"саша"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithWhitespace) {
        CHECK_WIDE_EQUAL(L"ААСШЯ", cipher->encrypt(L"САША Я"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"ААСШ", cipher->encrypt(L"САША1"));
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLetters) {
        CHECK_THROW(cipher->encrypt(L"111"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithPunctuation) {
        CHECK_WIDE_EQUAL(L"ААЯСШМУ", cipher->encrypt(L"САША,МЯУ"));
    }
    
    TEST_FIXTURE(Key3Fixture, ShortString) {
        CHECK_WIDE_EQUAL(L"С", cipher->encrypt(L"С"));
    }
    
    TEST(NonMultipleKeyLength) {
        Table cipher(4);
        CHECK_WIDE_EQUAL(L"КДЕНЛААСР", cipher.encrypt(L"АЛЕКСАНДР"));
    }
    
    TEST(NonMultipleKeyLength2) {
        Table cipher(10);
        CHECK_WIDE_EQUAL(L"РДНАСКЕЛА", cipher.encrypt(L"АЛЕКСАНДР"));
    }
}

/**
 * @brief Тесты расшифрования класса Table
 */
SUITE(DecryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"САША", cipher->decrypt(L"ААСШ"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_THROW(cipher->decrypt(L"Саша"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, WhitespaceString) {
        CHECK_THROW(cipher->decrypt(L"САША Я"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, DigitsString) {
        CHECK_THROW(cipher->decrypt(L"САША1"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->decrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLettersDecrypt) {
        CHECK_THROW(cipher->decrypt(L"111"), cipher_error);
    }
    
    
    TEST_FIXTURE(Key3Fixture, ValidCipherText) {
        CHECK_THROW(cipher->decrypt(L"САША,МЯУ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, ShortString) {
        CHECK_WIDE_EQUAL(L"С", cipher->decrypt(L"С"));
    }
    
    TEST(NonMultipleKeyLengthDecrypt) {
        Table cipher(4);
        CHECK_WIDE_EQUAL(L"АЛЕКСАНДР", cipher.decrypt(L"КДЕНЛААСР"));
    }
    
    TEST(NonMultipleKeyLengthDecrypt2) {
        Table cipher(10);
        CHECK_WIDE_EQUAL(L"АЛЕКСАНДР", cipher.decrypt(L"РДНАСКЕЛА"));
    }
}

/**
 * @brief Точка входа для запуска тестов
 * @param argc Количество аргументов командной строки
 * @param argv Аргументы командной строки
 * @return Код завершения тестов
 */
int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
