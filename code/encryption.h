// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <iostream>
#include <random>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

#define		RSA_KEY_SIZE		4096
#define		PASSWORD_SIZE		128
//base64

class base64 {
public:
	string getEncoded(CryptoPP::byte* a);
	CryptoPP::byte* getDecoded(string a);
};
class commanEncryptor {
protected:
    string genEncTitle(string path);
    string genDecTitle(string path);
};

//rsa-code
class RSA : public virtual base64{
	CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::PrivateKey *privkey;
	CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::PublicKey *pubkey;
	CryptoPP::AutoSeededRandomPool rng;
	bool generated = false;
public:
	RSA();
	void genRSAKeys();
	void loadConfig(string path);
	string Zencrypt(string msg);
	string Zdecrypt(string msg);
	string encrypt(string msg);
	string decrypt(string msg);
	string getPrivate() {
		return getEncoded((CryptoPP::byte*) & privkey);
	}
	string getPublic() {
		return getEncoded((CryptoPP::byte*)& pubkey);
	}
};

//aes-gcm code
class AES_GCM : public virtual commanEncryptor {
private:
	CryptoPP::AutoSeededRandomPool rng;
	string passcode;
	CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Encryption e;
	CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Decryption d;
public:
	string generateRandomString(int len);
	string randomgeniv(string passcode, int len);
	void genAESKey();
	void genAESKey(string passcode);
	string getPasscode() {
		return passcode;
	}
	string EncryptFile(string path);
	bool DecrptFile(string msg,string fpath);
};

class encryptor:public RSA , public AES_GCM{
public:
	string RSApriv() {
		return getPrivate();
	}
	string RSApub() {
		return getPublic();
	}
};

#endif // !ENCRYPTION_H
