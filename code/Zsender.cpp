#include "Zsender.h"

//CleanUP
void cleanup() {
	cout << "\nCLEANING UP ... \n";
	//any cleanup code if required

	//exit;
	exit(0);
}

//Config Management

void config::create(string userid) {
	username = userid;
	this->RSApriv = Encryptor.RSApriv(RSA_KEY);
	this->RSApub = Encryptor.RSApub(RSA_KEY);
}
void config::readFile(string path) {
	fstream read(path, ios::in | ios::binary);
	read >> username;
	read >> RSApriv;
	read >> RSApub;
	read.close();
}
void config::writeFile(string path) {
	fstream write(path, ios::out | ios::app | ios::binary);
	write.seekg(0, ios::beg);
	write << username << " ";
	write << RSApriv << " ";
	write << RSApub << " ";
	write.close();
}

void loadConfig() {
	string cwdf;
#ifdef _WIN32
	cwdf = "c:\\Zsender\\config.file";
#endif // _WIN32
#ifdef __linux__
	cwdf = "/linux_path/config.file";
#endif // __linux__
	Config.readFile(cwdf);
}
bool checkConfig() {
	string cwd;

#ifdef _WIN32
	cwd = "c:\\Zsender";
#endif // _WIN32

#ifdef __linux__
	cwd = "/linux_path/";
#endif // __linux__

	//check cwd exists
	//if true check config file exists
	if (fs::exists(cwd)) {
		string configFile = cwd + "\\config.file";
		if (fs::exists(configFile)) {
			loadConfig();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
void makeConfig() {
	string cwd, cwdf;

#ifdef _WIN32
	cwd = "c:\\Zsender";
	wstring wcwd = wstring(cwd.begin(), cwd.end());
	if (!_wmkdir(wcwd.c_str())) {
		//Do Nothing as it won't fail
	}
	cwdf = cwd + "\\config.file";
#endif // _WIN32

#ifdef __linux__
	cwd = "/linux_path/";
	mkdir(cwd);
	cwdf = cwd + "/config.file";
#endif // __linux__

	cout << "Enter your username: \t";
	string username;
	cin >> username;
	Config.create(username);
	Config.writeFile(cwdf);
	cout << "MADE YOUR CONFIG FILE SUCCESSFULLY ...\n";
}

//User manager
void loadup() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	logo();
	mainmenu();
	while (true) {
		cout << "Z-sender>>";
		string cmd;
		cin >> cmd;
		if (cmd == "send") {
			//send();
			continue;
		}
		else if (cmd == "recv") {
			//recv();
			continue;
		}
		else if (cmd == "cont") {
			//cont();
			continue;
		}
		else if (cmd == "help") {
			help();
			continue;
		}
		else if (cmd == "exit") {
			break;
		}
		else {
			cout << "ERROR: Command not recognised" << endl;
			continue;
		}
	}
	cout << "Cleaning up and Exiting!!!" << endl;
	cleanup();
}

void cmdLineLoadUP(char** cmd, int count) {
	vector<string> commands;

	for (int a = 1; a < count; a++) {
		commands.push_back((char*)*(cmd + a));
	}
	string mainCommand = commands.at(1);
	string contactUser;
	if (count == 4) {
		contactUser = commands.at(3);
	}
	if (mainCommand == "-s") {
		//send
		string filePath = commands.at(2);
		//generate send file and launch a server

	}
	else if (mainCommand == "-r") {
		//receive
		string filePath = commands.at(2);
		//generate a receiver server
	}
	else if (mainCommand == "-c") {
		//contact ADD/DEL/SHOW
		string contactCommand = commands.at(2);
		//sql manager
	}
}