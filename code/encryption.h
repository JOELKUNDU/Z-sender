// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "Zsender.h"

/*
Class - Encryptor
* Will take in the filepath
* Make a temp file
* Encrypt the tempfile with 256bit AES-GCM and a 64 char random string
* Make an ecrypted file name
* generate a decryption string - Use the server class (append the IP and the PORT number;
* RSA encrypt the decryption string based on contacts RSA public key 
* ADD 1 if contact specified else 2 for neutral
* format [encrypted for a contact] [password] [filename] [server data] 

*/

class encryptor {
private:

public:
	string RSApriv(int keySize);
	string RSApub(int keySize);

};

encryptor Encryptor;

#endif // !ENCRYPTION_H
