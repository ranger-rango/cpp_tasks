#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;
// include -lcrypto for the linker to locate the openssl crypto library during compilation

string dep_sha256(const string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *> (str.c_str()), str.size(), hash); // deprecated
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string sha256(const string& str)
{
    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if (!context) throw runtime_error("Failed to create EVP_MD_CTX");

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1)
    {
        throw runtime_error("EVP_DigestInit_ex failed");
    }
    if (EVP_DigestUpdate(context, str.c_str(), str.size()) != 1)
    {
        throw runtime_error("EVP_DigestUpdate failed");
    }
    if (EVP_DigestFinal_ex(context, hash, &length) != 1)
    {
        throw runtime_error("EVP_DigestFinal_ex failed");
    }
    EVP_MD_CTX_free(context);

    stringstream ss;
    for (unsigned int i = 0; i < length; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

string md5(const string& str)
{
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) throw runtime_error("Failed to create EVP_MD_CTX");

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestInit_ex(context, EVP_md5(), nullptr) != 1)
    {
        throw runtime_error("EVP_DigestInt_ex failed");
    }
    if (EVP_DigestUpdate(context, str.c_str(), str.size()) != 1)
    {
        throw runtime_error("EVP_DigestUpdate failed");
    }
    if (EVP_DigestFinal_ex(context, hash, &length) != 1)
    {
        throw runtime_error("EVP_DigestFinal_ex failed");
    }
    EVP_MD_CTX_free(context);

    stringstream ss;

    for (unsigned int i = 0; i < length; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}


int main()
{
    string data = "Hello World";
    string depSha_256 = dep_sha256(data);
    string sha_256 = sha256(data);
    string md5_ = md5(data);
    cout << "SHA-256: " << depSha_256 << endl;
    cout << "SHA-256: " << sha_256 << endl;
    cout << "MD5: " << md5_ << endl;
    return 0;
}