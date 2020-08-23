// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <filesystem>
#include <string>
#include <sqlite3.h>
#include "Zsender.h"
using namespace std;
namespace fs = std::filesystem;



class contacts {
private:
	sqlite3* contactDB = NULL;
	config Config;
public:
	contacts(config c) {
		Config = c;
		string cwd = Config.getCWD();
		if (!fs::exists(cwd)) {
			//ERROR
		}
		else {
			cwd = cwd + "contact.db";
			if (!fs::exists(cwd)) {
				//sql lite create table;
			}
		}
		//load data base
	}
	void show();
	void add();
	void remove();
	bool verifyContact(string contactID);
	string getContact();
	string getSelf();
};

#endif // !CONTACT_H
