#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "libs/hexlib.hpp"
#include "libs/simpleIni.h"

#define pass (void)0

using namespace std;

void checker(string, string, string , string);

int main(int argc, char **argv)
{
	//check argc
	if (argc != 3) {
		cout << " Usage\n\t"
		    << argv[0] << " <file> " << " <ini_file> ";
		exit(1);
	}

	char ch;
	string hexvalues;
	CSimpleIniA ini;
	CSimpleIniA::TNamesDepend sections;
	SI_Error rc = ini.LoadFile(argv[2]);
	ifstream fin(argv[1], ios::binary);

	// Open Files Error Handling 
	if (!fin) {
		cout << "Error : Cannot Open " << argv[1] << endl;
		exit(1);
	}

	if (rc < 0) {
		cout << "Error : Cannot Open " << argv[2] << endl;
		exit(1);
	}
	// Getting file bytes into hexvalues var
	for (int i = 0; !fin.eof(); i++) {
		fin.get(ch);
		hexvalues.push_back(ch);
	}

	fin.close();
	hexvalues = string_to_hex(hexvalues);

	//Getting ini file Sections & Sign Value
	ini.GetAllSections(sections);
	for (auto it = sections.begin(); it != sections.end(); ++it) {
		checker(
			ini.GetValue(it->pItem, "sign", ""),
			hexvalues,
			it->pItem,
			ini.GetValue(it->pItem, "ep_verify", ""));

	}

	return 0;
}

void checker(string pattrn, string hexvalue, string section,string ep_verify)
{
	string pattern(pattrn);
	if(ep_verify == "true"){
		string pattrn = string(pattrn.cbegin(), pattrn.cbegin() + 1920);
		string pattern(pattrn);
	}
	regex rx(pattern);

	string s(hexvalue);
	ptrdiff_t number_of_matches =
	    distance(sregex_iterator(s.begin(), s.end(), rx),
		     sregex_iterator());

	if (number_of_matches > 0) {
		cout << section << " : " << pattrn << " Found!!!" << endl;
	}

}
