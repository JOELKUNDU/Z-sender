#pragma once
#ifndef ZSENDER_H
#define ZSENDER_H


#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <random>
#include <vector>
#include <sstream>

#include "curl/curl.h"

#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/sha.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
#endif


using namespace std;
namespace fs = std::filesystem;

#define		PORT	6842

//class to store the session data;
/*
	- store the username
	- handle fresh starts
	- load up IP
*/
class localStorage {
private:
#ifdef _WIN32
	//wstirngs
	wstring wcwd = L"c:\\Zsender";
	wstring wcwdf = L"c:\\Zsender\\config.file";
	wstring wcwdd = L"c:\\Zsender\\Downloads";
	wstring wcwdu = L"c:\\Zsender\\Uploads";
	//strings
	string cwd  = "c:\\Zsender";
	string cwdf = "c:\\Zsender\\config.file";
	string cwdd = "c:\\Zsender\\Downloads";
	string cwdu = "c:\\Zsender\\Uploads";
#endif // _WIN32
#ifdef __linux__
	string cwd = "/home/user/Zsender/";
	string cwdf = "/home/user/Zsender/config.file";
	string cwdd = "/home/user/Zsender/Downloads";
	string cwdu = "/home/user/Zsender/Uploads";
	string cwdk = "/home/user/Zsender/Keys";
#endif // __linux__
	string name="Zsender";
	bool stockKeys = true;
	bool freshStart();			//checks if the program is running for the first time
	void createDir();			//creates the required dirs
	void loadData();			//loads data from the config file
public:
	void updateData();			//updates any changes to the config into the config file
	localStorage();				//constructor
								//checks if its firstStart()
								//if yes then it creates the dirs and the config file
								//also shows the notice of the key policy
								//if not then it simply loads the data;
	string getName();			//return user name
	string getUploadPath(string fname); // returns a valid upload-path 
};
//class to show the menus and handle the input;
/*
	- handle I/O
	- handle logic of the program
*/
class menus {
private:
	string passcode;
	string sendFileName;		//file to be sent
	string recvFileCode;		//recv code to download the file
	void logo();				//prints the banner
	void mainMenu();			//prints the main menu and has the associated logic
	void send();				//print the send menu and has the associated logic
	void recv();				//prints the receive menue and has the associated logic
	void help();				//print the help menu
	void exit();				//exits the program
	void clrs();				//clears the screen in the mainMenu and returns to mainMenu from any other function
	void setPasscode();			//set the passcode to encrypt the code.
	localStorage localLS;		//copy of the session's localStorage Class
public:
	menus(localStorage LS);		//constuctor
								//creates a copy of the localStorage
								//displays the banner and the mainmenu
};
//class to handle the encryption
/*
	- provide the encryption/decryption functions and the ability to 
*/
class encryptor {
private:
	string filename;			//filepath to the to be encrypted file
	string passcode;			//password to encrypt the file
public:
	encryptor(string passcode);					//constructor for encrypting codes; only assigns the passcode
	encryptor(string fname,string passcode);	//constructor for encryptin files; assigns both filename and passcode
	bool encrypt(string encpath);				//encrypts the files (gen key via hkdf; FileSink; return TRUE if successful else Displays the error and returns FALSE
	string encryptCode(string code);			//encrypts the code and returns the enc code if successful else NULL in case of faliure
};
class decryptor {
private:
	string filename;			//filepath to the to be decrypted file			
	string passcode;			//passcode to decrpyt the file
public:
	decryptor(string passcode);					//constructor for decrpyting the code; assigns only the passcode
	decryptor(string fname, string passcode);	//constructor for decrypting the file; assigns filename and passcode
	bool decrypt(string decpath);				//decrypts the file with passcode true is successful else false
	string decryptCode(string code);			//decrypts the code and returns it if successful else returns NULL 
};
//classes to handle the sending and recieveing of the data;
/*
sendServer
	-	Acts as the server for client to connect to and download the file
	-	handles the processing of the file into a vector of strings to be sent
	-	handles the initial comunication information
	-	generates the code for transmission and sends it out in encrypted format
	-	CODE Format [ip address] [port] [filename] [random challenge 128 char long]
*/
class sendServer {
	string ip;					//local IP
	int port = PORT;			//listening port
	string filename;			//file being sent
	string fpath;				//path of the file being sent
	string challenge;			//challenge to authenticate
	bool auth = false;			//authenticated?
	uint64_t fileSize;			//Number of elements in vector
	void getIP();				//gets the local IP	
	string code;
	vector<string> fileContent;
	int cLen = 128;
	string genChallenge();		//generates the challenge
	vector<string>processFile();//Processes the file into a vector of strings 4095 chars long.
public:
	sendServer(string fname);   //constructor
								//loads the local IP and shows the error in case of offline(function)
								//loads the filename 
								//processes the file(function)
								//generates the challenge and prepares the code(function)
	string getCode(string pass);//returns the encrypted code
	void startServer();			//start the server for listening
								//On connection it authenticates the challenge
								//if successfull then it will echo the filesize back
								//Then it will wait for the go string and then start broadcasting the 
								//string vector and shows the progress
								//on successful transfer it closes the socket							
};
/*
recvServer
	-	checks the authenticity of the code (decryptable into a meaningful text format)
	-	connects to the server according to the code
	-	sends the challenge and receives the size of the file in turn
	-	process the file and places it into the download folder and return the path
*/
class recvServer {
private:
	string ip;						//Server IP
	int port;						//Server Port
	string filename;				//filename 
	string challenge;				//challenge string
	string code;					//decrypted code
	vector<string> fileContent;		//vector holding the file data
	uint64_t fileSize;				//to be size of the fileContent data
	string fpath;					//downloaded path
	bool authCode();							//verifies the code on the basis of the port and challenge length
	void processFile();							//converts the vector into a file saved at the download path
public:
	recvServer(string encCode,string passcode);	//constructor
												//decrypting the encoded string
												//assigning the values
												//authenicating the code
												//assign download path
	void startServer();							//Starts the recieving server 
	string getPath();							//return the the download path.
};
#endif 
