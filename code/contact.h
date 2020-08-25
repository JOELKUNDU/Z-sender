// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <filesystem>
#include <string>
#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include "encryption.h"
using namespace std;
namespace fs = std::filesystem;

class contacts {
private:
	sqlite3* contactDB = NULL;
	string cwdf;
public:
	contacts();
	void show();
	void add(string code);
	void remove(string id);
	bool verifyContact(string contactID);
	string getContactKey(string id);
	string getSelf();
};

#endif // !CONTACT_H
