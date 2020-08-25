#include "contact.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

contacts::contacts() {
#ifdef _WIN32
	cwdf = "c:\\Zsender\\conacts.db";
#endif // _WIN32
#ifdef __linux__
	cwdf = "/home/user/Zsender/contacts.db";
#endif // __linux__
	if (fs::exists(cwdf)) {
		sqlite3_open(cwdf.c_str(), &contactDB);
		//create db
		string sql = "CREATE TABLE CONTACTS("  \
			"ID				TEXT	PRIMARY KEY     NOT NULL," \
			"PUBLIC_KEY     TEXT    NOT NULL," \
			" );";
		char* zErrMsg = 0;
		int rc=	sqlite3_exec(contactDB, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			//ERROR
		}
	}
	else {
		sqlite3_open(cwdf.c_str(), &contactDB);
	}
}

void contacts::show() {
	//show all the data
	string sql = "SELECT * FROM CONTACTS";
	char* error;
	int rc = sqlite3_exec(contactDB, sql.c_str(), callback, 0, &error);
	if (rc != SQLITE_OK) {
		//error
	}
}
void contacts::add(string code) {
	//decrpypt the rsa encrypted string
	string decoded = RSA().Zdecrypt(code);
	//string stream to get the username and the public key
	string username, publicKey;
	stringstream(decoded) >> username >> publicKey;
	string sql = "INSERT INTO CONTACTS(ID,PUBLIC_KEY) VALUES ('" + username + "','" + publicKey + "');";
	//add to the database
	char* error;
	int rc = sqlite3_exec(contactDB, sql.c_str(), callback, 0, &error);
	if (rc != SQLITE_OK) {
		//error
	}
}
void contacts::remove(string id) {
	//verify contact

	//Delete contact
	string sql = "DELETE FROM CONTACTS WHERE ID='"+id+"'";
	char* error;
	int rc = sqlite3_exec(contactDB, sql.c_str(), callback, 0, &error);
	if (rc != SQLITE_OK) {
		//error;
	}
}
bool contacts::verifyContact(string contactID) {
	//check contact exists 
	string sql = "SELECT EXISTS(SELECT 1 FROM CONTACTS WHERE '" + contactID + "'";
	char* error;
	int rc = sqlite3_exec(contactDB, sql.c_str(), callback, 0, &error);
	if (rc != SQLITE_OK) {
		//ERROR
	}
	//manage output;
	return false;
}
string contacts::getContactKey(string id) {
	//return client access string
	string out = "";
	string sql = "SELECT PUBLIC_KEY FROM CONTACTS WHERE ID = '" + id + "'";
	char* error;
	int rc = sqlite3_exec(contactDB, sql.c_str(), callback, 0, &error);
	if (rc != SQLITE_OK) {
		//ERROR
	}
	//manage output;
	return out;
}
string contacts::getSelf() {
	//ask server to send self invite link
	string out = "";

	return out;
}