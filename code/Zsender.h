// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0.
// This code is still in production stage and is incomplete.

#ifndef ZSENDER_H
#define ZSENDER_H

//std c++ headers
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>//to load the file
#include <thread>
#include <filesystem>
#include <fstream>
#include <vector>

//Zsender Header Files
#include "menus.h"
#include "encryption.h"


using namespace std;
namespace fs = std::filesystem;

#define		endl			'\n'
#define		RSA_KEY			2048
#define		linux_path		home/user/Zsender

//HANDLE CONFIGURATION AND SETTINGS

class config {
	string username;
	string RSApriv;
	string RSApub;
	string cwd;
public:
	config() {
#ifdef _WIN32
		cwd = "c:\\Zsender";
#endif // _WIN32

#ifdef __linux__
		cwd = "/linux_path/";
#endif // __linux__
	}
	void create(string userid);
	void readFile(string path);
	void writeFile(string path);
	string getCWD() {
		return cwd;
	}
	string getPriv() {
		return RSApriv;
	}
	string getPub() {
		return RSApub;
	}
	~config() {

	}
};

void loadConfig();
bool checkConfig();
void makeConfig();

//HANDLE CLEANUP OF PROCESSES

void cleanup();

//HANDLE COMMANDS AND UI

void loadup();
void cmdLineLoadUP(char** cmd, int count);



#endif // !ZSENDER_H

