#include <UnitTest++/UnitTest++.h>
#include <string>
#include <locale>
#include <codecvt>
#include "table.h"

using namespace std;

string wideToUtf8(const wstring& ws) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

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

struct Key3Fixture {
    Table* cipher;
    
    Key3Fixture() {
        cipher = new Table(2);
    }
    
    ~Key3Fixture() {
        delete cipher;
    }
};

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
    
    TEST(KeyEqualsOne) {
        Table cipher(1);
        CHECK_WIDE_EQUAL(L"САША", cipher.encrypt(L"САША"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithPunctuation) {
        CHECK_WIDE_EQUAL(L"ААЯСШМУ", cipher->encrypt(L"САША,МЯУ"));
    }
}

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
    
    TEST(KeyEqualsOneDecrypt) {
        Table cipher(1);
        CHECK_WIDE_EQUAL(L"САША", cipher.decrypt(L"САША"));
    }
    
    TEST_FIXTURE(Key3Fixture, ValidCipherText) {
        CHECK_THROW(cipher->decrypt(L"САША,МЯУ"), cipher_error);
    }
}

int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
