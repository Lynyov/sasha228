#include <UnitTest++/UnitTest++.h>
#include <string>
#include "modAlphaCipher.h"
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
        modAlphaCipher cipher(L"САША");
        CHECK_WIDE_EQUAL(L"СБЪГХ", cipher.encrypt(L"АБВГД"));
    }
    
    TEST(LongKey) {
        modAlphaCipher cipher(L"САШАМОЛОДЕЦ");
        CHECK_WIDE_EQUAL(L"СБЪГР", cipher.encrypt(L"АБВГД"));
    }
    
    TEST(LowCaseKey) {
        modAlphaCipher cipher(L"абв");
        CHECK_WIDE_EQUAL(L"АВДГЕ", cipher.encrypt(L"АБВГД"));
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"А1"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"А, Б"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"А Б"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
    
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cipher(L"БББ"), cipher_error);
    }
}

struct KeyBFixture {
    modAlphaCipher* cipher;
    
    KeyBFixture() {
        cipher = new modAlphaCipher(L"В");
    }
    
    ~KeyBFixture() {
        delete cipher;
    }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyBFixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"УВЪВ", cipher->encrypt(L"САША"));
    }
    
    TEST_FIXTURE(KeyBFixture, LowerCaseString) {
        CHECK_WIDE_EQUAL(L"УВЪВ", cipher->encrypt(L"саша"));
    }
    
    TEST_FIXTURE(KeyBFixture, StringWithWhitespaceAndPunctuation) {
        CHECK_WIDE_EQUAL(L"УВЪВОРНРЁЖШ", cipher->encrypt(L"САША МОЛОДЕЦ!"));
    }
    
    TEST_FIXTURE(KeyBFixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"БДУСКУМЖ", cipher->encrypt(L"Я 18 в списке"));
    }
    
    TEST_FIXTURE(KeyBFixture, EmptyString) {
        CHECK_THROW(cipher->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, NoAlphaString) {
        CHECK_THROW(cipher->encrypt(L"123"), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        CHECK_WIDE_EQUAL(L"РЯЧЯ", cipher.encrypt(L"САША"));
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(KeyBFixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"САША", cipher->decrypt(L"УВЪВ"));
    }
    
    TEST_FIXTURE(KeyBFixture, LowerCaseString) {
        CHECK_THROW(cipher->decrypt(L"Саша"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, WhitespaceString) {
        CHECK_THROW(cipher->decrypt(L"САША МОЛОДЕЦ"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, DigitsString) {
        CHECK_THROW(cipher->decrypt(L"САША1"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, PunctuationString) {
        CHECK_THROW(cipher->decrypt(L"САША,МЯУ"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, EmptyString) {
        CHECK_THROW(cipher->decrypt(L""), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        CHECK_WIDE_EQUAL(L"САША", cipher.decrypt(L"РЯЧЯ"));
    }
}

int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
