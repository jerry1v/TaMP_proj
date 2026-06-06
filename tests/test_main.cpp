#include <cassert>
#include <iostream>

#include "../backend_server/aes.h"
#include "../backend_server/sha1.h"
#include "../backend_server/newton.h"

using namespace std;

void testAES()
{
    string text = "hello";

    string encrypted =
        aesEncrypt(text);

    assert(encrypted != text);

    cout << "[PASS] AES" << endl;
}

void testSHA1()
{
    string hash1 =
        sha1Hash("test");

    string hash2 =
        sha1Hash("test");

    assert(hash1 == hash2);

    cout << "[PASS] SHA1" << endl;
}

void testNewton()
{
    string result =
        newtonMethod(25);

    double value =
        stod(result);

    assert(value > 4.9);
    assert(value < 5.1);

    cout << "[PASS] NEWTON" << endl;
}

int main()
{
    testAES();
    testSHA1();
    testNewton();

    cout << endl;
    cout << "ALL TESTS PASSED"
         << endl;

    return 0;
}