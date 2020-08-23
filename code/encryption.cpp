#define _CRT_SECURE_NO_WARNINGS
// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#include "encryption.h"

//CLASS - BASE64

string base64::getEncoded(CryptoPP::byte* a) {
	string out;
	CryptoPP::StringSource(a, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(out)));
	return out;
}
CryptoPP::byte* base64::getDecoded(string a) {
	string out;
	CryptoPP::StringSource(a, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(out)));
	CryptoPP::byte* bout = (CryptoPP::byte*)out.c_str();
	return bout;
}

//CLASS - RSA

RSA::RSA() {
	string cwdf;
#ifdef _WIN32
	cwdf = "c:\\Zsender\\config.file";
#endif // _WIN32
#ifdef __linux__
	cwdf = "/home/user/Zsender/config.file";
#endif // __linux__
	if (fs::exists(cwdf)) {
		loadConfig(cwdf);
	}
	else {
		//ERROR
	}
}
void RSA::genRSAKeys() {
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, RSA_KEY_SIZE);
	*privkey=CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::PrivateKey(params);
	*pubkey=CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::PublicKey(params);
	generated = true;
}
void RSA::loadConfig(string path) {
	fstream read(path, ios::in | ios::binary);
	string buffer,privstr,pubstr;
	read >> buffer;
	read >> privstr;
	read >> pubstr;
	read.close();
	privkey = (CryptoPP::RSA::PrivateKey*)getDecoded(privstr);
	pubkey = (CryptoPP::RSA::PublicKey*)getDecoded(pubstr);
}
string RSA::encrypt(string msg) {
	CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Encryptor E(*pubkey);
	string out;
	CryptoPP::StringSource S(msg, true,
		new CryptoPP::PK_EncryptorFilter(rng,E,
			new CryptoPP::StringSink(out)
		));
	return out;
}
string RSA::decrypt(string msg) {
	CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Decryptor D(*privkey);
	string out;
	CryptoPP::StringSource S(msg, true,
		new CryptoPP::PK_DecryptorFilter(rng, D,
			new CryptoPP::StringSink(out)
		));
	return out;
}

//CLASS - AES_GCM 

string commanEncryptor::genEncTitle(string path) {
	try {
		fs::path f(path);
		string filename = f.filename().string();
		string encname = "";
		string password = "syIQlmMZZZUBLamI16u0lQXZuSmlVHGoctpdU44tvM9iwEDOANEO358cOh4RJLTqe8AJvEtPzvDqZ7b6UHPLY3oRgoMjiN4jvnfPwU2CqMi07OLlnNxPP3P2FBW3vrjI";
		string iv = "OJwSNTBppZbuCNnk";
		//hdkf
		CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_2K_Tables>::Encryption e;
		e.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);
		CryptoPP::StringSource f1(filename, true,
			new CryptoPP::AuthenticatedEncryptionFilter(e,
				new CryptoPP::StringSink(encname)));
		string finalout;
		CryptoPP::StringSource encode(encname, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(finalout)));
		return finalout;
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception... (13e)" << endl;
		cerr << e.what() << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
}
string commanEncryptor::genDecTitle(string path) {
	try {
		fs::path f(path);
		string filename1 = f.filename().string();
		cout << "D: " << filename1 << endl;
		string filename;
		CryptoPP::StringSource decode(filename1, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(filename)));
		string decname = "";
		string password = "syIQlmMZZZUBLamI16u0lQXZuSmlVHGoctpdU44tvM9iwEDOANEO358cOh4RJLTqe8AJvEtPzvDqZ7b6UHPLY3oRgoMjiN4jvnfPwU2CqMi07OLlnNxPP3P2FBW3vrjI";
		string iv = "OJwSNTBppZbuCNnk";
		//hdkf
		CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_2K_Tables>::Decryption d;
		d.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);
		CryptoPP::StringSource f2(filename, true,
			new CryptoPP::AuthenticatedDecryptionFilter(d,
				new CryptoPP::StringSink(decname)));
		return decname;
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception... (134)" << endl;
		cerr << e.what() << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
}

string AES_GCM::generateRandomString(int len) {
	int ri;
	string charset = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM1234567890";
	string out;
	uniform_int_distribution<int> d(0, charset.size() - 1);
	random_device rd;
	srand((int)time(0));
	for (int j = 0; j < len; j++) {
		ri = d(rd);
		out += charset.at(ri);
	}
	return out;
}
string AES_GCM::randomgeniv(string passcode,int len) {
	string out = "";
	uint64_t rseed = 0;
	for (unsigned int i = 0; i < passcode.size(); i++) {
		int a = (int)passcode.at(i);
		rseed += (uint64_t)a;
	}
	minstd_rand d(rseed % UINT_MAX);
	string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
	for (int i = 0; i < len; i++) {
		int c = d() % charset.size();
		out += charset.at(c);
	}
	return out;
}
void AES_GCM::genAESKey() {
	passcode = generateRandomString(PASSWORD_SIZE);
	try {
		string password = passcode;
		string iv = randomgeniv(password,16);
		//hdkf
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		e.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);		
		d.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (...) {
		cout << "Error generating key from password\n" << strerror(errno) << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
}
void AES_GCM::genAESKey(string passcode) {
	try {
		string password = passcode;
		string iv = randomgeniv(password,16);
		//hdkf
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		e.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
		d.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (...) {
		cout << "Error generating key from password\n" << strerror(errno) << endl;
		cout << "Press ENTER to Exit ...";
		getchar();
		exit(EXIT_FAILURE);
	}
}
string AES_GCM::EncryptFile(string path) {
	genAESKey();
	string out;
	//copy the file to the to be uploaded 
	string encname = genEncTitle(path);
	string dest;
#ifdef _WIN32
	dest = "c:\\Zsender\\Uploads\\" + encname;
#endif // _wIN32
#ifdef __linux__
	dest = "/home/user/Zsender/Uploads/"+encname;
#endif // __linux__
	CryptoPP::FileSource f(path.c_str(), true,
		new CryptoPP::AuthenticatedEncryptionFilter(e,
			new CryptoPP::FileSink(dest.c_str())));

	out = encname + " " + passcode;
	return out;
}
bool AES_GCM::DecrptFile(string msg,string fpath) {
	try {
		stringstream ss(msg);
		string encname;
		ss >> encname;
		ss >> passcode;
		genAESKey(passcode);
		string decname = genDecTitle(encname);
		string dest;
#ifdef _WIN32
		dest = "c:\\Zsender\\Dowwnloads\\" + encname;
#endif // _wIN32
#ifdef __linux__
		dest = "/home/user/Zsender/Downloads/" + encname;
#endif // __linux__
		CryptoPP::FileSource f(fpath.c_str(), true,
			new CryptoPP::AuthenticatedEncryptionFilter(e,
				new CryptoPP::FileSink(dest.c_str())));
		return true;
	}
	catch (...) {
		return false;
	}
}