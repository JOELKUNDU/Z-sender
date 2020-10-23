// This code is placed in the public domain by JOEL KUNDU
// under GNU GPL  v3.0. 
// CONTACT: jdevcode2@gmail.com

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
#ifdef _WIN32
	string uPath = cwdu+'\\'+ filename;
#endif // _WIN32
#ifdef __linux__
	string upath = cwdu + '/' + filename;
#endif // __linux__


	if (fs::exists(uPath)) {
		cin.ignore();
		cout << "\nERROR: FILE ALREADY IN THE DIR. IT WILL BE OVERWRITTEN. REMOVE IT FROM THE ZSENDER/Upload DIR TO SAVE IT.\n";
		cout << "\nfILE PATH: " << uPath;
		cout << "\nPress ENTER twice to continue. (AFTER SAVING A COPY OF THE EXISTING FILE IF YOU WISH TO SAVE IT)\n\n";
		getchar();
		cin.ignore();
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
	cin.ignore();
}
void menus::send(){
	cout << "\n\n::: SENDING MENU :::\n\n";

	while (true) {
		cin.ignore();
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
			sendServer S(sendFileName);
			//generate the recv code
			cout << "\nRECIEVE CODE: \n\n" << S.getCode(passcode) <<"\n"<< endl;
			cout << "\nShare the code and the passcode with the reciever to transfer the file.\n" << endl;
			//start broadcasting
			cout << "\nSTARTING SERVER\n";
			S.startServer();
			break;
		}
		else {
			cout << "ERROR: FILE DOES NOT EXIST! TRY AGAIN\n";
			cout << "\nPress ENTER twice to continue\n";
			getchar();
			continue;
		}
	}

	cout << "Press ENTER to return to Main Menu.";
	getchar();
	clrs();
}
void menus::recv(){
	cin.ignore();
	cout << "\n::: RECEIVING MENU :::" << endl;
	cout << "\nEnter the Recieving Code: ";
	getline(cin, recvFileCode);
	cout << "\nEnter the Passcode: ";
	getline(cin, passcode);
	string outputPath;
	cout << "\nEnter the output dir path: ";
	getline(cin, outputPath);
	cout << "\nSTARTING SERVER\n";
	recvServer R(recvFileCode, passcode);
	R.startServer();
	if (!decryptor(R.getPath(), passcode).decrypt(R.retOutputPath(outputPath))) {
		cout << "FATAL ERROR: DECRYPTION NOT SUCCESSFUL! EXITTING.";
		std::exit(1);
	}
	cout << "Press ENTER to return to Main Menu.";
	getchar();
	clrs();
}
void menus::help(){
	cout << "\n\n::: HELP MENU - ZSENDER :::"<<endl;
	cout << endl;
	cout << "Z-SENDER - ENCRYPTED TERMINAL FILE TRANSFER UTILITY\n";
	cout << "Placed in the public domain by - JOEL KUNDU\n";
	cout << "CONTACT: jdevcode2@gmail.com";
	cout << endl;
	cout << "Z-Sender is a terminal based cross platform file transfer utility to transfer files\n";
	cout << "in a secure way via AES-GCM (64k Tables) encryption over the internet.\n";
	cout << "Incase you find any BUG please use the contact information given above. The code for\n";
	cout << "this program is provided as OPEN-SOURCE under GNU GPL-V3 and you should have recieved\n";
	cout << "a copy of the licence when you clones the repositary. If not please contact me.\n";
	cout << endl;
	cout << "# USAGE #" << endl;
	cout << endl;
	cout << "> TO SEND A FILE" << endl;
	cout << "\t1. Type in 'send' when prompted on the Main Menu." << endl;
	cout << "\t2. Enter the complete path to the file to be sent." << endl;
	cout << "\t3. Create a Password (more than 8 characters long)." << endl;
	cout << "\t   If you don't create one then a password will be generated." << endl;
	cout << "\t4. Send the Generated RECIEVE CODE and the PASSWORD to the reciever." << endl;
	cout << "\t5. Wait for the reciever to download the file.(KEEP THE TERMINAL OPEN)" << endl;
	cout << endl;
	cout << "> TO RECIEVE A FILE" << endl;
	cout << "\t1. Type in 'recv' when prompted on the Main Menu." << endl;
	cout << "\t2. Enter the code and the password given by the user." << endl;
	cout << "\t3. Enter the OUTPUT PATH (where the downloaded file should be saved." << endl;
	cout << "\t4. Wait for the system to connect, download and decrpyt the file.(KEEP THE TERMINAL OPEN)" << endl;
	cout << endl;
	cout << "NOTE: \nThe downloaded encrypted file can also be decypted using ZENC.\n\n";
	cout << "IMPORTANT DIRs" << endl;
#ifdef _WIN32
	cout <<endl<< "c:\\Zsender";
	cout <<endl<< "c:\\Zsender\\config.file";
	cout <<endl<< "c:\\Zsender\\Downloads";
	cout <<endl<< "c:\\Zsender\\Uploads";
#endif // _WIN32
#ifdef __linux__
	cout << endl <<"/home/user/Zsender/";
	cout << endl <<"/home/user/Zsender/config.file";
	cout << endl <<"/home/user/Zsender/Downloads";
	cout << endl <<"/home/user/Zsender/Uploads";
#endif // __linux__
	cout << endl;

	cout << "\n:::HELP MENU - ZSENDER:::\n\n";





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
	cout << "\nDo you want to set a password? [y/n]\t";
	char ch;
	cin >> ch;
	while (true) {
		if (ch=='y'||ch=='Y') {
			passcodeBegin:
			cout << "\nENTER PASSWORD (Min Character - 8) : ";
			cin >> passcode;
			if (passcode.size() < 8) {
				cout << "\nERROR: INVALID LENGTH! TRY AGAIN.";
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
			cout << "\nThe Password set is :" << passcode << "\n\n:::IMPORTANT:::\n\nCopy and send this to the reciever along with the receiver code.";
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
		string finalOut;

		CryptoPP::StringSource encode(encCode, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(finalOut)));

		return finalOut;
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
	string decCode ,bufCode;
	try {
		//hkdf
		CryptoPP::StringSource decode(code, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(bufCode)));
		code.clear();
		code = bufCode;
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
size_t sendServer::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){
	((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendServer::getIP() {
	try {
		cout << "\nLOCAL IP: ";
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, "https://myexternalip.com/raw");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		cout << "\n";
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
	reader.close();
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

	::bind(listening, (sockaddr*)&hint, sizeof(hint));

	if (listening == INVALID_SOCKET) {
		cout << "FATAL ERROR: UNABLE TO BIND SOCKET! EXITING." << endl;
		exit(1);
	}

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(sockaddr_in);
	
	cout << "\nWAITING FOR CONNECTION\n";

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		cout << "FATAL ERROR: UNABLE TO CREATE CLIENT SOCKET! EXITING." << endl;
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);


	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)	{
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	bool fileTransferFail = false;
	//FILE TRANSFER BEGINS

	char buf[4096];
	ZeroMemory(buf, 4096);

	cout << "\nAUTHENTICATING CONNECTION\n";

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
				return;
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
				return;
			}
			else {
				cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";
			}
		}
	}

	cout << "\nSENDING FILE\n";

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
	else {
		cout << "\nFAILED TO SEND FILE.\nTRY AGAIN?[y/n]\t";
		char ch; 
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			startServer();
		}
		else if (ch == 'n' || ch == 'N') {
			cout << "\nENDING TRANSMISSION\n";
			closesocket(clientSocket);
			WSACleanup();
			return;
		}
		else {
			cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";
		}
	}

	//CLEAN UP
	closesocket(clientSocket);
	WSACleanup();
	return;
#endif // _WIN32

#ifdef __linux__
	

	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)	{
		cout << "FATAL ERROR: UNABLE TO CREATE SOCKET! EXITING." << endl;
		exit(1);
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	if (listening == -1) {
		cout << "FATAL ERROR: UNABLE TO CREATE SOCKET! EXITING." << endl;
		exit(1);
	}

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	cout << "\nWAITNG FOR CONNECTION\n";

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	memset(host, 0, NI_MAXHOST); 
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
		cout << host << " connected on port " << service << endl;
	}
	else{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	close(listening);

	bool fileTransferFail = false;
	//FILE TRANSFER BEGINS

	char buf[4096];
	ZeroMemory(buf, 4096);

	cout << "\nAUTHENTICATING CONNECTION\n";

	//AUTH LOOP
	while (true) {
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == -1)
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
				close(clientSocket);
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
				close(clientSocket);
				return;
			}
			else {
				cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";
			}
		}
	}

	cout << "\nSENDING FILE\n";

	vector<string>::iterator it;
	int sendIndex = 0;

	for (it = fileContent.begin(); it < fileContent.end(); it++) {
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == -1)
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
	else {
		cout << "\nFAILED TO SEND FILE.\nTRY AGAIN?[y/n]\t";
		char ch;
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			startServer();
		}
		else if (ch == 'n' || ch == 'N') {
			cout << "\nENDING TRANSMISSION\n";
			close(clientSocket);
			return;
		}
		else {
			cout << "ERROR: INVALID INPUT! TRY AGAIN.\n";
		}
	}
	close(clientSocket);
	return;

#endif // __linux__
}

//Class: recvSender
recvServer::recvServer(string encCode, string passcode) {
	constbegin:
	code = decryptor(passcode).decryptCode(encCode);

	if (!authCode()) {
		cout << "\nERROR: IVALID CODE! PLEASE ENTER THE CORRECT CODE. (type exit to exit)\n";
		cout << "Recieve Code: ";
		getline(cin, encCode);
		if (encCode == "exit" || encCode == "EXIT") {
			std::exit(1);
		}
		goto constbegin;
	}

#ifdef _WIN32
	fpath = "c:\\Zsender\\Downloads\\" + filename;
#endif // _WIN32
#ifdef __linux__
	fpath = "/home/user/Zsender/Downloads/" + filename;
#endif // __linux__
}
bool recvServer::authCode() {
	stringstream cstream(code);

	cstream >> ip;
	string p;
	cstream >> p;
	port = stoi(p);
	cstream >> filename;
	cstream >> challenge;
	if (challenge.size() != 128 || port != PORT) {
		return false;
	}
	else {
		return true;
	}
}
void recvServer::processFile() {
	fstream writer(fpath, ios::out | ios::binary);
	writer.seekp(0, ios::beg);
	vector<string>::iterator it;
	for (it = fileContent.begin(); it < fileContent.end(); it++) {
		string data = *it;
		writer << data;
	}
	writer.close();
}
void recvServer::startServer(){
	string ipAddress = ip;
#ifdef _WIN32
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0)
	{
		cerr << "FATAL ERROR: CONNOT START WINSOCK! ERROR NO. " << wsResult << endl;
		exit(1);
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "FATAL ERROR: CANNOT CREATE SOCKET! ERROR NO. " << WSAGetLastError() << endl;
		WSACleanup();
		exit(1);
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons((u_short)port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	cout << "\nATTEMPTING TO CONNECT TO SENDER\n";

	while (true) {
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "ERROR: CANNOT CONNECT TO SENDER! TRY AGAIN? [y/n] " << endl;
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				continue;
			}
			else {
				cout << "\nEXITING.\n";
				closesocket(sock);
				WSACleanup();
				exit(1);
			}
		}
		break;
	}
	char buf[4096];

	cout << "\nATHENTICATING CONNECTION\n";

	//AUTH ATTEMPT
	int sendResult0 = send(sock, challenge.c_str(), challenge.size() + 1, 0);
	if (sendResult0 != SOCKET_ERROR) {
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0) {
			fileSize = stoi(string(buf, 0, bytesReceived));
		}
	}
	else {
		cout << "FATAL ERROR: AUTHENTICATION FAILED! PLEASE RESTART THE PROGRAM AND TRY AGAIN.";
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	cout << "\nDOWNLOADING FILE\n";

	for (uint64_t i = 0; i < fileSize; i++) {
		string confirm = "RECV";
		int sendResult = send(sock, confirm.c_str(), confirm.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			// Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0) {
				fileContent.push_back(string(buf, 0, bytesReceived));
			}
			cout << "\rPROGRESS: " << i / fileSize << "%";
		}
		else {
			cout << "FATAL ERROR: CONNECTION ERROR! EXITING.";
			exit(1);
		}
	}

	cout << "\nFILE RECIEVED!\n";

	//process file
	processFile();
	cout << "Encrypted file saved at " << fpath << endl;
	closesocket(sock);
	WSACleanup();
#endif // _WIN32
#ifdef __linux__
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		cerr << "FATAL ERROR: CANNOT CREATE SOCKET! ERROR NO. " << WSAGetLastError() << endl;
		close(sock);
		exit(1);
	}
	int port = PORT;

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	cout << "\nTRYING TO CONNECT TO THE SENDER\n";

	while (true) {
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "ERROR: CANNOT CONNECT TO SENDER! TRY AGAIN? [y/n]" << WSAGetLastError() << endl;
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				continue;
			}
			else {
				cout << "\nEXITING.\n";
				close(sock);
				exit(1);
			}
		}
		break;
	}
	char buf[4096];

	cout << "\nAUTHENTICATING CONNECTION\n";

	//AUTH ATTEMPT
	int sendResult0 = send(sock, challenge.c_str(), challenge.size() + 1, 0);
	if (sendResult0 != -1) {
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0) {
			fileSize = stoi(string(buf, 0, bytesReceived));
		}
	}
	else {
		cout << "FATAL ERROR: AUTHENTICATION FAILED! PLEASE RESTART THE PROGRAM AND TRY AGAIN.";
		close(sock);
		exit(1);
	}

	cout << "\nDOWNLOADING THE FILE\n";

	for (uint64_t i = 0; i < fileSize; i++) {
		string confirm = "RECV";
		int sendResult = send(sock, confirm.c_str(), confirm.size() + 1, 0);
		if (sendResult != -1)
		{
			// Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0) {
				fileContent.push_back(string(buf, 0, bytesReceived));
			}
			cout << "\rPROGRESS: " << i / fileSize << "%";
		}
		else {
			cout << "FATAL ERROR: CONNECTION ERROR! EXITING.";
			close(sock)
			exit(1);
		}
	}

	cout << "\nFILE RECIEVED!\n";
	processFile();
	cout << "Encrypted file saved at " << fpath << endl;

	close(sock);

#endif // __linux__

}
string recvServer::getPath() {
	return fpath;
}
string recvServer::retOutputPath(string path) {
	int size = path.size();
#ifdef _WIN32
	if (path.at(size - 1) == '\\') {
		return path + filename;
	}
	else {
		return path + '\\' + filename;
	}
#endif // _WIN32

#ifdef __linux__ 
	if (path.at(size - 1) == '/') {
		return path + filename;
	}
	else {
		return path + '/' + filename;
	}
#endif // 


}