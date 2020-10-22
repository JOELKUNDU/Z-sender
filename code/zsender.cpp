#define _CRT_SECURE_NO_WARNINGS
#include "zsender.h"

//Class: localStorage

bool localStorage::freshStart() {
	if (fs::exists(cwd)) {
		string configFile = cwdf;
		if (fs::exists(configFile)) {
			return false;
		}
		else
			return true;
	}
	else
		return true;
}
void localStorage::createDir() {
#ifdef _WIN32
	_wmkdir(wcwd.c_str());
	_wmkdir(wcwdd.c_str());
	_wmkdir(wcwdu.c_str());
#endif // _WIN32

#ifdef __linux__
	mkdir(cwd.c_str());
	mkdir(cwdd.c_str());
	mkdir(cwdu.c_str());
#endif
	fstream(cwdf, ios::out);
}
void localStorage::loadData() {
	fstream(cwdf, ios::in) >> name;
}
void localStorage::updateData() {
	fstream(cwdf, ios::out) << name ;
}

localStorage::localStorage() {
	if (freshStart()) {
		createDir();
		cout << "Enter your username: \t";
		cin >> name;
		updateData();
	}
	else loadData();
}
string localStorage::getName() {
	return name;
}
string localStorage::getUploadPath(string fname) {
	fs::path p(fname);
	string filename = p.filename().string();
	string uPath= cwdu + filename;
	if (fs::exists(uPath)) {
		cout << "ERROR: FILE ALREADY IN THE DIR. IT WILL BE OVERWRITTEN. REMOVE IT FROM THE ZSENDER/Upload DIR TO SAVE IT.\n";
		cout << "fILE PATH: " << uPath;
		cout << "Press ENTER to continue. (AFTER SAVING A COPY OF THE EXISTING FILE IF YOU WISH TO SAVE IT)";
		getchar();
	}
	return uPath;
}


//Class: menus

void menus::logo() {
	cout << "******************************************************************" << endl;
	cout << "\t Z - SENDER " << endl;
	cout << "\t Send Files securely with no eves dropping " << endl;
	cout << "\t By - JOELKUNDU (under GNU GPL V3 license) " << endl;
	cout << "******************************************************************" << endl;
	cout << endl;
}
void menus::mainMenu() {
	cout << endl;
	cout << "1.\t[send]\t\tSend File" << endl;
	cout << "2.\t[recv]\t\tRecieve File" << endl;
	cout << "3.\t[help]\t\tHelp" << endl;
	cout << "4.\t[exit]\t\tExit" << endl;
	cout << "5.\t[clrs]\t\tClear Screen" << endl;
	cout << endl;
	string cmd;
	while (true) {
		cout << "ZSENDER>> ";
		cin >> cmd;
		if (cmd == "send" || cmd == "SEND") {
			send();
		}
		else if (cmd == "recv" || cmd == "RECV") {
			recv();
		}
		else if (cmd == "help" || cmd == "HELP") {
			help();
		}
		else if (cmd == "exit" || cmd == "EXIT") {
			exit();
		}
		else if (cmd == "clrs" || cmd == "CLRS") {
			clrs();
		}
		else {
			cout << "ERROR: INVALID INPUT! TRY AGAIN\n";
		}
	}
}
void menus::send(){
	cout << "::: SENDING MENU :::";

	cout << endl;
	cout << "The File is encrpyted using AES-GCM and the transfer key is encrypted using a password" << endl;
	cout << "The password can be set by you or can be generated for a specified length." << endl;
	cout << endl;

	while (true) {
		cout << "Enter FilePath: ";
		getline(cin, sendFileName);
		if (fs::exists(sendFileName)) {
			setPasscode();
			string uPath = localLS.getUploadPath(sendFileName);
			//encrypt the file
			if (!encryptor(sendFileName, passcode).encrypt(uPath)) {
				cout << "FATAL ERROR: ENCRYPTION NOT SUCCESSFUL! EXITTING.";
				std::exit(1);
			}
			//server
			//generate the recv code
			//start broadcasting
			break;
		}
		else {
			cout << "ERROR: FILE DOES NOT EXIST! TRY AGAIN\n";
		}
	}

	cout << "Press ENTER to return to Main Menu.";
	getchar();
	clrs();
}
void menus::recv(){
	cout << "::: RECEIVING MENU :::";
	while (true) {
		cout << "Enter the Recieving Code: ";
		getline(cin, recvFileCode);
		cout << "Enter the Passcode: ";
		getline(cin, passcode);
		string outputPath;
		cout << "Enter the output path: ";
		getline(cin, outputPath);
		//if the recieve code is accurate then start downloading into the decrpted folder
		//decrypt the file and copy it to the output path
		//break;
		/*
		else{
			cout<<"ERROR: INVALID RECEIVING CODE! TRY AGAIN";
		}
		*/
	}

	cout << "Press ENTER to return to Main Menu.";
	getchar();
	clrs();
}
void menus::help(){
	cout << "::: HELP MENU :::";
	//to be populated after finishing the project
}
void menus::exit(){
	std::exit(0);
}
void menus::clrs(){
#ifdef _WIN32
	std::system("cls");
#else
	// Assume POSIX
	std::system("clear");
#endif
	logo();
	mainMenu();
}
void menus::setPasscode() {
	cout << "\nDo you want to set a password? [y/n]";
	char ch;
	cin >> ch;
	while (true) {
		if (ch=='y'||ch=='Y') {
			passcodeBegin:
			cout << "ENTER PASSWORD (Min Character - 8) : ";
			cin >> passcode;
			if (passcode.size() < 8) {
				cout << "ERROR: INVALID LENGTH! TRY AGAIN.";
				goto passcodeBegin;
			}
			else {
				cout << "\nPassword had been set.\n";
				break;
			}
		}
		else if (ch=='n'||ch=='N') {
			string password = "we64sfw44be4tba4tn4Gw4dFF";
			int len = 32;
			string out = "";
			uint64_t rseed = 0;
			for (unsigned int i = 0; i < password.size(); i++) {
				int a = (int)password.at(i);
				rseed += (uint64_t)a;
			}
			random_device a;
			minstd_rand d(rseed % UINT_MAX);
			string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
			for (int i = 0; i < len; i++) {
				int c = (d() + a()) % charset.size();
				out += charset.at(c);
			}
			passcode = out;
			cout << "The Password set is :" << passcode << "\n Copy and send this to the reciever along with the receiver code.";
		}
		else {
			cout << "ERROR: INVALID INPUT! TRY AGAIN.";
			continue;
		}
		break;
	}
}
menus::menus(localStorage LS) {
	localLS = LS;
	logo();
	mainMenu();
}


//Class: encryptor
encryptor::encryptor(string passcode) {
	this->passcode = passcode;
}
encryptor::encryptor(string fname,string passcode) {
	this->filename = fname;
	this->passcode = passcode;
}
bool encryptor::encrypt(string encpath) {
	try {
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Encryption enc;
		//hkdf
		string password = passcode;
		string iv = "";
		minstd_rand d(passcode.size());
		string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
		for (unsigned int i = 0; i < 16; i++) {
			int c = d() % charset.size();
			iv += charset.at(c);
		}
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		enc.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
		//encrypt the file
		CryptoPP::FileSource f(filename.c_str(), true,
			new CryptoPP::AuthenticatedEncryptionFilter(enc,
				new CryptoPP::FileSink(encpath.c_str())));
		//return value
		return true;
	}
	catch (CryptoPP::Exception& e) {
		cerr <<"ERROR: "<< e.what() << endl;
		return false;
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		return false;
	}
}
string encryptor::encryptCode(string code) {
	string encCode;
	try {
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Encryption enc;
		//hkdf
		string password = passcode;
		string iv = "";
		minstd_rand d(passcode.size());
		string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
		for (unsigned int i = 0; i < 16; i++) {
			int c = d() % charset.size();
			iv += charset.at(c);
		}
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		enc.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
		//encrypt the file
		CryptoPP::StringSource f(code, true,
			new CryptoPP::AuthenticatedEncryptionFilter(enc,
				new CryptoPP::StringSink(encCode)));
		//return value
		return encCode;
	}
	catch (CryptoPP::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return "";
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		return "";
	}
}

//Class: decryptor
decryptor::decryptor(string passcode) {
	this->passcode = passcode;
}
decryptor::decryptor(string fname, string passcode) {
	this->filename = fname;
	this->passcode = passcode;
}
bool decryptor::decrypt(string decpath) {
	try {
		//hkdf
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Decryption dec;
		//hkdf
		string password = passcode;
		string iv = "";
		minstd_rand d(passcode.size());
		string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
		for (unsigned int i = 0; i < 16; i++) {
			int c = d() % charset.size();
			iv += charset.at(c);
		}
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		dec.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
		//decrypt the file
		CryptoPP::FileSource f(filename.c_str(), true,
			new CryptoPP::AuthenticatedDecryptionFilter(dec,
				new CryptoPP::FileSink(decpath.c_str())));
		//return value
		return true;
	}
	catch (CryptoPP::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return false;
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		return false;
	}
}
string decryptor::decryptCode(string code) {
	string decCode;
	try {
		//hkdf
		CryptoPP::GCM<CryptoPP::AES, CryptoPP::GCM_64K_Tables>::Decryption dec;
		//hkdf
		string password = passcode;
		string iv = "";
		minstd_rand d(passcode.size());
		string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
		for (unsigned int i = 0; i < 16; i++) {
			int c = d() % charset.size();
			iv += charset.at(c);
		}
		CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const unsigned char*)password.data(), password.size(),
			(const unsigned char*)iv.data(), iv.size(), NULL, 0);
		dec.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, key + CryptoPP::AES::DEFAULT_KEYLENGTH);
		//decrypt the file
		CryptoPP::StringSource f(code, true,
			new CryptoPP::AuthenticatedDecryptionFilter(dec,
				new CryptoPP::StringSink(decCode)));
		//return value
		return decCode;
	}
	catch (CryptoPP::Exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return "";
	}
	catch (...) {
		cout << "ERROR: " << strerror(errno) << endl;
		return "";
	}
}

//Class: sendServer
void sendServer::getIP() {
	try {
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, "https://myexternalip.com/raw");
		curl_easy_setopt(curl, CURLOPT_READDATA, &ip);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	catch (...) {
		cout << "ERROR: CAN'T CONNET TO THE INTERNET! CONNECT AND RESTART APPLICATION.\n";
		cout << "Press ENTER to exit.";
		getchar();
		std::exit(1);
	}
}
sendServer::sendServer(string fname) {
	getIP();
	fpath = fname;	
	fs::path p(fname);
	filename = p.filename().string();
	fileContent = processFile();
	challenge = genChallenge();
	string port = to_string(PORT);
	code = ip + " " + port + " " + filename + " " + challenge;
}
string sendServer::getCode(string pass) {
	return encryptor(pass).encryptCode(code);
}
vector<string>sendServer::processFile() {
	vector<string> buffer;	
	fstream reader(fpath, ios::in | ios::binary);
	reader.seekg(0, ios::beg);
	bool end = false;
	while (!reader.eof()) {
		string buf;
		for (int a = 0; a < 4095; a++) {
			if (reader.eof()) {
				end = true;
				break;
			}
			unsigned char b;
			reader >> b;
			buf += b;
		}
		buffer.push_back(buf);
		if (end) {
			break;
		}
	}
	fileSize = buffer.size();
	return buffer;
}
string sendServer::genChallenge() {
	string challenge = "";
	random_device alpha;
	uniform_int_distribution dist(0, INT_MAX);
	minstd_rand d(dist(alpha));
	string charset = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890*=";
	for (int i = 0; i < cLen; i++) {
		int c = d() % charset.size();
		challenge += charset.at(c);
	}
	return challenge;
}
void sendServer::startServer() {
#ifdef _WIN32
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	if (WSAStartup(ver, &wsData)) {
		cout << "FATAL ERROR: UNABLE TO START SERVER! EXITING."<<endl;
		exit(1);
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cout << "FATAL ERROR: UNABLE TO CREATE SOCKET! EXITING." << endl;
		exit(1);
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	if (listening == INVALID_SOCKET) {
		cout << "FATAL ERROR: UNABLE TO BIND SOCKET! EXITING." << endl;
		exit(1);
	}

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		cout << "FATAL ERROR: UNABLE TO CREATE CLIENT SOCKET! EXITING." << endl;
		exit(1);
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	bool fileTransferFail = false;
	//FILE TRANSFER BEGINS

	char buf[4096];
	ZeroMemory(buf, 4096);

	//AUTH LOOP
	while (true) {
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "\nERROR: ERROR IN RECEIVING INFO" << endl;
			cout << "TRY AGAIN? [y/n] ";
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				continue;
			}
			else if (ch == 'n' || ch == 'N') {
				cout << "\nENDING TRANSMISSION\n";
				closesocket(clientSocket);
				WSACleanup();
			}
			else {
				cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";				
			}
		}
		string recvChallenge = string(buf, 0, bytesReceived);
		if (recvChallenge == challenge) {
			cout << "\nCONNECTION AUTHENTICATED\n";
			string fSize = to_string(fileSize);
			send(clientSocket, fSize.c_str(), fSize.size() + 1, 0);
			break;
		}
		else {
			cerr << "\nERROR: ERROR IN AUTHENTICATING CONNECTION" << endl;
			cout << "TRY AGAIN? [y/n] ";
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				continue;
			}
			else if (ch == 'n' || ch == 'N') {
				cout << "\nENDING TRANSMISSION\n";
				closesocket(clientSocket);
				WSACleanup();
			}
			else {
				cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";
			}
		}
	}

	vector<string>::iterator it;
	int sendIndex=0;

	for (it = fileContent.begin(); it < fileContent.end(); it++) {
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "ERROR: ERROR IN RECEIVING INFO" << endl;
			fileTransferFail = true;
			break;
		}
		else if (bytesReceived == 0)
		{
			cout << "ERROR: CLIENT DISCONNECTED" << endl;
			fileTransferFail = true;
			break;
		}
		else if (string(buf, 0, bytesReceived) == "RECV") {
			string toSend = *it;
			send(clientSocket, toSend.c_str(), toSend.size() + 1, 0);			
		}
		sendIndex++;
		cout << "\rPROGRESS: " << sendIndex / fileSize << "% ";
	}

	if (!fileTransferFail) {
		cout << "\nFILE SENT SUCCESSFULLY\n";
	}

	//CLEAN UP
	closesocket(clientSocket);
	WSACleanup();
	return;
#endif // _WIN32
#ifdef __linux__

#endif // __linux__
}

//Class: recvSender
recvServer::recvServer(string encCode, string passcode) {
	code = decryptor(passcode).decryptCode(encCode);

	stringstream cstream(code);
	cstream >> ip;
	string p;
	cstream >> p;
	port = stoi(p);
	cstream >> filename;
	cstream >> challenge;

#ifdef _WIN32
	fpath = 
#endif // _WIN32

}