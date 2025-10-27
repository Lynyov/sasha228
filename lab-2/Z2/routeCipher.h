#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class CipherException : public invalid_argument {
public:
    explicit CipherException(const string& message) : invalid_argument(message) {}
    explicit CipherException(const char* message) : invalid_argument(message) {}
};

class RouteTable
{
private:
    int columnCount;
    int validateTableKey(const int key);
    wstring validateInputText(const wstring& text);
    wstring validateOutputText(const wstring& text);
    
public:
    explicit RouteTable(int key);
    wstring encodeText(const wstring& plainText);
    wstring decodeText(const wstring& cipherText);
};