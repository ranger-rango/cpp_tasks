//
// Created by DO on 28/08/2025.
//
// crypto_tool.cpp
// Compile: g++ -std=c++17 crypto_tool.cpp -o crypto_tool -lcrypto

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <memory>

using byte = unsigned char;

// Helper: read whole file
static std::vector<byte> readFile(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) throw std::runtime_error("Cannot open file: " + path);
    return std::vector<byte>(std::istreambuf_iterator<char>(ifs), {});
}

// Helper: write whole file
static void writeFile(const std::string &path, const std::vector<byte> &data) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) throw std::runtime_error("Cannot write file: " + path);
    ofs.write((char*)data.data(), data.size());
}

// Print OpenSSL errors and throw
[[noreturn]] static void fail_openssl(const std::string &msg) {
    unsigned long e;
    std::string buf;
    while ((e = ERR_get_error()) != 0) {
        char buf2[256];
        ERR_error_string_n(e, buf2, sizeof(buf2));
        buf += buf2;
        buf += "\n";
    }
    throw std::runtime_error(msg + "\n" + buf);
}

// Hash (MD5/SHA1/SHA256) using EVP
static std::vector<byte> compute_hash(const std::string &alg, const std::vector<byte> &data) {
    const EVP_MD *md = nullptr;
    if (alg == "md5") md = EVP_md5();
    else if (alg == "sha1") md = EVP_sha1();
    else if (alg == "sha256") md = EVP_sha256();
    else throw std::runtime_error("Unsupported hash: " + alg);

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) fail_openssl("EVP_MD_CTX_new failed");
    if (1 != EVP_DigestInit_ex(ctx, md, nullptr)) fail_openssl("DigestInit failed");
    if (1 != EVP_DigestUpdate(ctx, data.data(), data.size())) fail_openssl("DigestUpdate failed");
    unsigned int len = EVP_MD_size(md);
    std::vector<byte> out(len);
    if (1 != EVP_DigestFinal_ex(ctx, out.data(), &len)) fail_openssl("DigestFinal failed");
    out.resize(len);
    EVP_MD_CTX_free(ctx);
    return out;
}

// AES-CBC encrypt/decrypt using EVP (key length 16 or 32, iv length 16)
static std::vector<byte> aes_crypt(bool encrypt, int keybits, const std::vector<byte> &key, const std::vector<byte> &iv, const std::vector<byte> &in) {
    if (!(keybits == 128 || keybits == 256)) throw std::runtime_error("keybits must be 128 or 256");
    if (key.size() != (size_t)keybits/8) throw std::runtime_error("Key has wrong length");
    if (iv.size() != 16) throw std::runtime_error("IV must be 16 bytes");

    const EVP_CIPHER *cipher = (keybits == 128) ? EVP_aes_128_cbc() : EVP_aes_256_cbc();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) fail_openssl("EVP_CIPHER_CTX_new failed");

    if (1 != EVP_CipherInit_ex(ctx, cipher, nullptr, nullptr, nullptr, encrypt ? 1 : 0)) fail_openssl("CipherInit failed");
    if (1 != EVP_CIPHER_CTX_set_key_length(ctx, key.size())) fail_openssl("Set key length failed");
    if (1 != EVP_CipherInit_ex(ctx, nullptr, nullptr, key.data(), iv.data(), -1)) fail_openssl("CipherInit key/iv failed");

    std::vector<byte> out;
    out.resize(in.size() + EVP_CIPHER_block_size(cipher));
    int outlen1 = 0;
    if (1 != EVP_CipherUpdate(ctx, out.data(), &outlen1, in.data(), (int)in.size())) fail_openssl("CipherUpdate failed");
    int outlen2 = 0;
    if (1 != EVP_CipherFinal_ex(ctx, out.data() + outlen1, &outlen2)) fail_openssl("CipherFinal failed");
    out.resize(outlen1 + outlen2);
    EVP_CIPHER_CTX_free(ctx);
    return out;
}

// Generate random bytes
static std::vector<byte> random_bytes(size_t n) {
    std::vector<byte> v(n);
    if (1 != RAND_bytes(v.data(), (int)n)) fail_openssl("RAND_bytes failed");
    return v;
}

// Generate RSA keypair and write to PEM files
static void generate_rsa(int bits, const std::string &privPath, const std::string &pubPath) {
    RSA *rsa = RSA_new();
    BIGNUM *bn = BN_new();
    if (!BN_set_word(bn, RSA_F4)) fail_openssl("BN_set_word failed");
    if (!RSA_generate_key_ex(rsa, bits, bn, nullptr)) fail_openssl("RSA_generate_key_ex failed");

    // Write private
    FILE *fp = fopen(privPath.c_str(), "wb");
    if (!fp) throw std::runtime_error("Cannot open priv file");
    if (!PEM_write_RSAPrivateKey(fp, rsa, nullptr, nullptr, 0, nullptr, nullptr)) fail_openssl("PEM_write_RSAPrivateKey failed");
    fclose(fp);

    // Write public
    fp = fopen(pubPath.c_str(), "wb");
    if (!fp) throw std::runtime_error("Cannot open pub file");
    if (!PEM_write_RSA_PUBKEY(fp, rsa)) fail_openssl("PEM_write_RSA_PUBKEY failed");
    fclose(fp);

    RSA_free(rsa);
    BN_free(bn);
}

// RSA encrypt (PKCS#1 v1.5) small data
static std::vector<byte> rsa_encrypt_with_pubkey_pem(const std::string &pubPemPath, const std::vector<byte> &plain) {
    FILE *fp = fopen(pubPemPath.c_str(), "rb");
    if (!fp) throw std::runtime_error("Cannot open pub PEM");
    RSA *rsa = PEM_read_RSA_PUBKEY(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    if (!rsa) fail_openssl("PEM_read_RSA_PUBKEY failed");
    int rsaSize = RSA_size(rsa);
    std::vector<byte> out(rsaSize);
    int len = RSA_public_encrypt((int)plain.size(), plain.data(), out.data(), rsa, RSA_PKCS1_PADDING);
    if (len < 0) fail_openssl("RSA_public_encrypt failed");
    out.resize(len);
    RSA_free(rsa);
    return out;
}

// RSA decrypt with private PEM
static std::vector<byte> rsa_decrypt_with_privkey_pem(const std::string &privPemPath, const std::vector<byte> &cipher) {
    FILE *fp = fopen(privPemPath.c_str(), "rb");
    if (!fp) throw std::runtime_error("Cannot open priv PEM");
    RSA *rsa = PEM_read_RSAPrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    if (!rsa) fail_openssl("PEM_read_RSAPrivateKey failed");
    int rsaSize = RSA_size(rsa);
    std::vector<byte> out(rsaSize);
    int len = RSA_private_decrypt((int)cipher.size(), cipher.data(), out.data(), rsa, RSA_PKCS1_PADDING);
    if (len < 0) fail_openssl("RSA_private_decrypt failed");
    out.resize(len);
    RSA_free(rsa);
    return out;
}

// Hybrid encrypt: generate AES key+iv, AES-encrypt file, RSA-encrypt AES key, then write out a container:
// [4-byte BE len of encrypted AES key][encrypted AES key][16-byte IV][AES-ciphertext]
static void hybrid_encrypt(const std::string &pubPem, const std::string &inFile, const std::string &outFile, int aesBits=256) {
    auto plain = readFile(inFile);
    auto key = random_bytes(aesBits/8);
    auto iv = random_bytes(16);
    auto ct = aes_crypt(true, aesBits, key, iv, plain);
    auto encKey = rsa_encrypt_with_pubkey_pem(pubPem, key);

    std::vector<byte> out;
    uint32_t enklen = (uint32_t)encKey.size();
    // big-endian length
    out.push_back((enklen>>24)&0xFF);
    out.push_back((enklen>>16)&0xFF);
    out.push_back((enklen>>8)&0xFF);
    out.push_back((enklen)&0xFF);
    out.insert(out.end(), encKey.begin(), encKey.end());
    out.insert(out.end(), iv.begin(), iv.end());
    out.insert(out.end(), ct.begin(), ct.end());
    writeFile(outFile, out);
}

// Hybrid decrypt: parse container and reverse
static void hybrid_decrypt(const std::string &privPem, const std::string &inFile, const std::string &outFile, int aesBits=256) {
    auto data = readFile(inFile);
    if (data.size() < 4+1+16) throw std::runtime_error("Input too small for hybrid format");
    uint32_t enklen = (uint32_t)data[0]<<24 | (uint32_t)data[1]<<16 | (uint32_t)data[2]<<8 | (uint32_t)data[3];
    if (data.size() < 4 + enklen + 16) throw std::runtime_error("Input truncated");
    std::vector<byte> encKey(data.begin()+4, data.begin()+4+enklen);
    std::vector<byte> iv(data.begin()+4+enklen, data.begin()+4+enklen+16);
    std::vector<byte> ct(data.begin()+4+enklen+16, data.end());
    auto key = rsa_decrypt_with_privkey_pem(privPem, encKey);
    if (key.size() != (size_t)aesBits/8) {
        // allow graceful mismatch if RSA decrypt yields different size (could be leading zeros), but prefer exact
        if (key.size() > (size_t)aesBits/8) key.resize(aesBits/8);
        else throw std::runtime_error("Decrypted AES key size mismatch");
    }
    auto plain = aes_crypt(false, aesBits, key, iv, ct);
    writeFile(outFile, plain);
}

// Main CLI
int main(int argc, char **argv) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    try {
        if (argc < 2) throw std::runtime_error("Usage: see source header");

        std::string cmd = argv[1];

        if (cmd == "hash") {
            if (argc != 4) throw std::runtime_error("Usage: hash <md5|sha1|sha256> <file>");
            std::string alg = argv[2];
            auto data = readFile(argv[3]);
            auto dg = compute_hash(alg, data);
            // print hex
            std::ios::fmtflags f( std::cout.flags() );
            std::cout << std::hex;
            for (byte b : dg) {
                std::cout.width(2); std::cout.fill('0');
                std::cout << (int)b;
            }
            std::cout << std::endl;
            std::cout.flags( f );
            return 0;
        }

        if (cmd == "aes-encrypt") {
            if (argc != 7) throw std::runtime_error("Usage: aes-encrypt <128|256> keyfile ivfile infile outfile");
            int kb = std::stoi(argv[2]);
            auto key = readFile(argv[3]);
            auto iv = readFile(argv[4]);
            auto in = readFile(argv[5]);
            auto out = aes_crypt(true, kb, key, iv, in);
            writeFile(argv[6], out);
            std::cout << "AES encrypted -> " << argv[6] << std::endl;
            return 0;
        }

        if (cmd == "aes-decrypt") {
            if (argc != 7) throw std::runtime_error("Usage: aes-decrypt <128|256> keyfile ivfile infile outfile");
            int kb = std::stoi(argv[2]);
            auto key = readFile(argv[3]);
            auto iv = readFile(argv[4]);
            auto in = readFile(argv[5]);
            auto out = aes_crypt(false, kb, key, iv, in);
            writeFile(argv[6], out);
            std::cout << "AES decrypted -> " << argv[6] << std::endl;
            return 0;
        }

        if (cmd == "gen-rsa") {
            if (argc != 5) throw std::runtime_error("Usage: gen-rsa <bits> priv.pem pub.pem");
            int bits = std::stoi(argv[2]);
            generate_rsa(bits, argv[3], argv[4]);
            std::cout << "RSA generated: priv=" << argv[3] << " pub=" << argv[4] << std::endl;
            return 0;
        }

        if (cmd == "rsa-encrypt") {
            if (argc != 5) throw std::runtime_error("Usage: rsa-encrypt pub.pem infile outfile");
            auto in = readFile(argv[3]);
            auto out = rsa_encrypt_with_pubkey_pem(argv[2], in);
            writeFile(argv[4], out);
            std::cout << "RSA encrypted -> " << argv[4] << std::endl;
            return 0;
        }

        if (cmd == "rsa-decrypt") {
            if (argc != 5) throw std::runtime_error("Usage: rsa-decrypt priv.pem infile outfile");
            auto in = readFile(argv[3]);
            auto out = rsa_decrypt_with_privkey_pem(argv[2], in);
            writeFile(argv[4], out);
            std::cout << "RSA decrypted -> " << argv[4] << std::endl;
            return 0;
        }

        if (cmd == "hybrid-encrypt") {
            if (argc != 5) throw std::runtime_error("Usage: hybrid-encrypt pub.pem infile outfile");
            hybrid_encrypt(argv[2], argv[3], argv[4], 256);
            std::cout << "Hybrid encrypted -> " << argv[4] << std::endl;
            return 0;
        }

        if (cmd == "hybrid-decrypt") {
            if (argc != 5) throw std::runtime_error("Usage: hybrid-decrypt priv.pem infile outfile");
            hybrid_decrypt(argv[2], argv[3], argv[4], 256);
            std::cout << "Hybrid decrypted -> " << argv[4] << std::endl;
            return 0;
        }

        throw std::runtime_error("Unknown command");
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        ERR_print_errors_fp(stderr);
        return 2;
    }
}

