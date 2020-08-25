#include "menus.h"

void logo() {
	cout << "******************************************************************" << endl;
	cout << "\t Z - SENDER " << endl;
	cout << "\t Send Files securely with no eves dropping " << endl;
	cout << "\t By - JOELKUNDU (under GNU GPL V3 license) " << endl;
	cout << "******************************************************************" << endl;
	cout << endl;
}
void mainmenu() {
	cout << endl;
	cout << "1.\t[send]\t\tSend File" << endl;
	cout << "2.\t[recv]\t\tRecieve File" << endl;
	cout << "3.\t[cont]\t\tManage Contacts" << endl;
	cout << "4.\t[help]\t\tHelp" << endl;
	cout << "5.\t[exit]\t\tExit" << endl;
	cout << endl;
}
void help() {
	cout << endl;
	cout << "********************************" << endl;
	cout << "*### HELP BOOK FOR Z-SENDER ###*" << endl;
	cout << "********************************" << endl;
	cout << "Z-Sender is a comand file transfer utility to safely and securely share files over the internet." << endl;
	cout << "The files are secured using AES-GCM (256bit key) and the keys and transfer details are protected" << endl;
	cout << "using RSA (4096bit key)." << endl;
	cout << "$ USAGE $" << endl;
	cout << "Every menu is in the form serial no. [command] command description." << endl;
	cout << "Enter [command] (without the square bracets) to use that command." << endl;
	cout << "" << endl;
	cout << "> SENDING A FILE" << endl;
	//add details on how to send a file
	cout << "> RECEIVING A FILE" << endl;
	//add details on recieving a file
	cout << "> MANAGING CONTACTS" << endl;
	//add details on managing the contacts
	cout << "> OPENING HELPBOOK" << endl;
	cout << "Type \"help\" to open the helpbook" << endl;
	cout << "********************************" << endl;
}
void send() {
	cout << endl;
	cout << "1.\t[snew]\t\tSend to a new contact or anonymously" << endl;
	cout << "3.\t[sold]\t\tSend to an old contact" << endl;
	cout << "4.\t[back]\t\tGo back to main menu" << endl;
	cout << endl;
	while (true) {
		cout << "Z-sender//SENDING>>";
		string cmd1;
		string filename;
		cin >> cmd1;
		if (cmd1 == "back") {
			return;
		}
		else if (cmd1 == "help")help();
		else if (cmd1 == "snew" || cmd1 == "sold") {
			cout << "Enter path to the file to be sent: ";
			getline(cin, filename);
			try {
				fstream(filename, ios::in);
			}
			catch (...) {
				cout << "ERROR: unable to open the file";
			}
			sender s(cmd1, filename);
		}
		else {
			cout << "ERROR: Command not found" << endl;
			continue;
		}
	}

}
void recv() {
	cout << endl;
	cout << "1.\t[recn]\t\tRecieve From an unkown contact" << endl;
	cout << "2.\t[reco]\t\tRecieve From a known contact" << endl;
	cout << "4.\t[back]\t\tGo back to main menu" << endl;
	cout << endl;
	cout << "Z-sender//RECIEVING>>";
	string cmd1;
	cin >> cmd1;
	if (cmd1 == "back") {
		return;
	}
	else if (cmd1 == "help")help();
}
void cont() {
	cout << endl;
	cout << "1.\t[show]\t\tShow the contact list" << endl;
	cout << "2.\t[cadd]\t\tAdd a new contact" << endl;
	cout << "3.\t[rmov]\t\tRemove a contact" << endl;
	cout << "4.\t[back]\t\tGo back to main menu." << endl;
	cout << endl;
	while (true) {
		cout << "Z-sender//CONTACTS>>";
		string cmd1;
		cin >> cmd1;
		if (cmd1 == "back") {
			return;
		}
		else if (cmd1 == "help")help();
	}
}