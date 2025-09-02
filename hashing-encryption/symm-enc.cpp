#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

using namespace std;

vector<unsigned char> aes_enc(const string& plainText, const unsigned char* key, const unsigned char* iv)
{
    EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
    if (!context) throw runtime_error("EVP_CIPHER_CTX failed");

    if (EVP_EncryptInit_ex(context, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        throw runtime_error("EVP_EncryptInit_ex failed");
    }
    vector<unsigned char> cipherText(plainText.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    int len;
    int cipherTextLen = 0;

    if (EVP_EncryptUpdate(context, cipherText.data(), &len, reinterpret_cast<const unsigned char*> (plainText.data()), plainText.size()) != 1)
    {
        throw runtime_error("EVP_EncryptUpdate failed");
    }
    cipherTextLen = len;

    if (EVP_EncryptFinal_ex(context, cipherText.data() + len, &len) != 1)
    {
        throw runtime_error("EVP_EncryptFinal_ex failed");
    }
    cipherTextLen += len;

    EVP_CIPHER_CTX_free(context);
    cipherText.resize(cipherTextLen);
    return cipherText;
}

string aes_dec(const vector<unsigned char>& cipherText, const unsigned char* key, const unsigned char* iv)
{
    EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
    if (!context) throw runtime_error("EVP_CIPHER_CTX failed");

    if (EVP_DecryptInit_ex(context, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        throw runtime_error("EVP_DecryptInit_ex failed");
    }
    vector<unsigned char> plainText(cipherText.size());
    int len;
    int plainTextLen = 0;

    if (EVP_DecryptUpdate(context, plainText.data(), &len, cipherText.data(), cipherText.size()) != 1)
    {
        throw runtime_error("EVP_DecryptUpdate failed");
    }
    plainTextLen = len;

    if (EVP_DecryptFinal_ex(context, plainText.data() + len, &len) != 1)
    {
        throw runtime_error("EVP_DecryptFinal_ex failed");
    }
    plainTextLen += len;

    EVP_CIPHER_CTX_free(context);

    return string(reinterpret_cast<char *> (plainText.data()), plainTextLen);
}

// openssl rand -out aes.key 32  # 256-bit key
// openssl rand -out aes.iv 16   # 128-bit IV
string readFile(const string& file)
{
    ifstream infile(file);
    ostringstream oss;
    oss << infile.rdbuf();
    string fileContent = oss.str();
    infile.close();
    return fileContent;
}

void writeFile(const string& file, const string& data)
{
    ofstream outfile(file, ios::binary);
    outfile << data;
    outfile.close();

    return;
}

void writeFile(const string& file, const vector<unsigned char>& data)
{
    ofstream outfile(file, ios::binary);
    outfile.write(reinterpret_cast<const char*>(data.data()), data.size());
    outfile.close();

    return;
}

int main(int argc, char** argv)
{
    // operation file key iv 
    if (argc < 5)
    {
        cout << "Usage: ./symm-enc <aes-enc | aes-dec> <file> <key> <iv> " << endl;
        exit(1);
    }
    string operation = argv[1];
    if (operation == "aes-enc")
    {
        string filePath = argv[2];
        string plainText = readFile(filePath);
        string keyStr = readFile(argv[3]);
        string ivStr = readFile(argv[4]);
        const unsigned char* key = reinterpret_cast<const unsigned char *> (keyStr.c_str());
        const unsigned char* iv = reinterpret_cast<const unsigned char *> (ivStr.c_str());
        vector<unsigned char> cipherText = aes_enc(plainText, key, iv);
        writeFile(filePath + ".enc", cipherText);
    }
    else if (operation == "aes-dec")
    {
        string filePath = argv[2];
        string cipherTextStr = readFile(filePath);
        vector<unsigned char> cipherText(cipherTextStr.begin(), cipherTextStr.end());
        string keyStr = readFile(argv[3]);
        string ivStr = readFile(argv[4]);
        const unsigned char* key = reinterpret_cast<const unsigned char *> (keyStr.c_str());
        const unsigned char* iv = reinterpret_cast<const unsigned char *> (ivStr.c_str());
        string plainText = aes_dec(cipherText, key, iv);
        cout << plainText << endl;
        // writeFile(filePath.substr(0, filePath.size() - 4), plainText);
    }
    else
    {
        throw runtime_error("Unknown Operation Selected");
    }

    return 0;
}