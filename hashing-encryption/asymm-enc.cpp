#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
// openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:4096 -out private.pem
// openssl pkey -in private.pem -pubout -out public.pem

vector<unsigned char> rsa_enc(EVP_PKEY* pubKey, const string& plainText)
{
    EVP_PKEY_CTX* context = EVP_PKEY_CTX_new(pubKey, nullptr);
    if (!context) throw runtime_error("EVP_PKEY_CTX failed");
    if (EVP_PKEY_encrypt_init(context) <= 0)
    {
        throw runtime_error("EVP_PKEY_encrypt_init failed");
    }

    size_t outLen;
    if (EVP_PKEY_encrypt(context, nullptr, &outLen, reinterpret_cast<const unsigned char *> (plainText.data()), plainText.size()) <= 0)
    {
        throw runtime_error("EVP_PKEY_encrypt size query failed");
    }
    vector<unsigned char> cipherText(outLen);

    if (EVP_PKEY_encrypt(context, cipherText.data(), &outLen, reinterpret_cast<const unsigned char *> (plainText.data()), plainText.size()) <= 0)
    {
        throw runtime_error("EVP_PKEY_encrypt failed");
    }
    EVP_PKEY_CTX_free(context);

    cipherText.resize(outLen);
    return cipherText;
}

string rsa_dec(EVP_PKEY* privKey, const vector<unsigned char>& cipherText)
{
    EVP_PKEY_CTX* context = EVP_PKEY_CTX_new(privKey, nullptr);
    if (!context) throw runtime_error("EVP_PKEY_CTX failed");

    if (EVP_PKEY_decrypt_init(context) <= 0)
    {
        throw runtime_error("EVP_PKEY_decrypt_init failed");
    }

    size_t outLen;
    if (EVP_PKEY_decrypt(context, nullptr, &outLen, cipherText.data(), cipherText.size()) <= 0)
    {
        throw runtime_error("EVP_PKEY_decrypt size query failed");
    }

    vector<unsigned char> plainText(outLen);

    if (EVP_PKEY_decrypt(context, plainText.data(), &outLen, cipherText.data(), cipherText.size()) <= 0)
    {
        throw runtime_error("EVP_PKEY_decrypt failed");
    }
    EVP_PKEY_CTX_free(context);
    return string(reinterpret_cast<char *>(plainText.data()), outLen);
}

vector<unsigned char> rsa_sign(EVP_PKEY* privKey, const string& unsignedData)
{
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) throw runtime_error("EVP_MD_CTX failed");

    if (EVP_DigestSignInit(context, nullptr, EVP_sha256(), nullptr, privKey) <= 0)
    {
        throw runtime_error("EVP_DigestSignInit failed");
    }

    if (EVP_DigestSignUpdate(context, unsignedData.c_str(), unsignedData.size()) <= 0)
    {
        throw runtime_error("EVP_DigestSignUpdate failed");
    }

    size_t sigLen = 0;
    if (EVP_DigestSignFinal(context, nullptr, &sigLen) <= 0)
    {
        throw runtime_error("EVP_DigestSignFinal size failed");
    }

    vector<unsigned char> signature(sigLen);

    if (EVP_DigestSignFinal(context, signature.data(), &sigLen) <= 0)
    {
        throw runtime_error("EVP_DigestSignFinal failed");
    }
    signature.resize(sigLen);

    EVP_MD_CTX_free(context);
    EVP_PKEY_free(privKey);
    EVP_cleanup();

    return signature;
}

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
    // operation file key
    if (argc < 4)
    {
        cout << "Usage: ./asymm-enc <rsa-enc | rsa-dec> <file> <public.pem | private.pem> " << endl;
        exit(1);
    }
    string operation = argv[1];
    if (operation == "rsa-enc")
    {
        string filePath = argv[2];
        string plainText = readFile(filePath);
        string pubKeyStr = readFile(argv[3]);
        BIO* bio = BIO_new_mem_buf(pubKeyStr.data(), pubKeyStr.size());
        if (!bio) throw runtime_error("Failed to create BIO");

        // Read PEM public key
        EVP_PKEY* pubKey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
        if (!pubKey) throw runtime_error("Failed to parse public key");
        vector<unsigned char> cipherText = rsa_enc(pubKey, plainText);
        BIO_free(bio);
        writeFile(filePath + ".rsa.enc", cipherText);
    }
    else if (operation == "rsa-dec")
    {
        string filePath = argv[2];
        string cipherTextStr = readFile(filePath);
        vector<unsigned char> cipherText(cipherTextStr.begin(), cipherTextStr.end());
        string privKeyStr = readFile(argv[3]);
        BIO* bio = BIO_new_mem_buf(privKeyStr.data(), privKeyStr.size());
        if (!bio) throw runtime_error("Failed to create BIO");

        // Read PEM public key
        EVP_PKEY* privKey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
        if (!privKey) throw runtime_error("Failed to parse private key");
        string plainText = rsa_dec(privKey, cipherText);
        BIO_free(bio);
        cout << plainText << endl;
        // writeFile(filePath.substr(0, filePath.size() - 4), plainText);
    }
    else if (operation == "rsa-sign")
    {
        string filePath = argv[2];
        string unsignedData = readFile(filePath);
        string privKeyStr = readFile(argv[3]);
        BIO* bio = BIO_new_mem_buf(privKeyStr.data(), privKeyStr.size());
        if (!bio) throw runtime_error("Failed to create BIO");
        EVP_PKEY* privKey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
        if (!privKey) throw runtime_error("Failed to parse private key");
        vector<unsigned char> signature = rsa_sign(privKey, unsignedData);
        BIO_free(bio);
        writeFile(filePath + ".sig", signature);
    }
    else
    {
        throw runtime_error("Unknown Operation Selected");
    }
    return 0;
}