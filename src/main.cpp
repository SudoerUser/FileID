#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <getopt.h>
#include "libs/hexlib.hpp"
#include "libs/simpleIni.h"

using namespace std;

void checker(string, string, string , string);
int banner(string);

int main(int argc, char **argv)
{
	char ch;
	char *fvalue = NULL;
	char *ivalue = NULL;
	int opt,iflag,fflag;
	string hexvalues;
	CSimpleIniA ini;
	CSimpleIniA::TNamesDepend sections;
	
	while((opt = getopt(argc, argv, "f:i:")) != -1)
	{
		switch(opt)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;
				break;
			case 'f':
				fflag = 1;
				fvalue = optarg;
				break;
			default:
				banner(argv[0]);

		}
	}

	if(iflag != 1 || fflag != 1) banner(argv[0]);

	ifstream fin(fvalue, ios::binary);
	SI_Error rc = ini.LoadFile(ivalue);
	
	// Open Files Error Handling 
	if (!fin) {
		cout << "Error : Cannot Open " << fvalue << endl;
		exit(1);
	}

	if (rc < 0) {
		cout << "Error : Cannot Open " << ivalue << endl;
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
		string pattrn = string(pattrn.cbegin(), pattrn.cbegin() + 4288);
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

int banner(string argzero)
{
	cout << " Usage:" << argzero << "[OPTION]....[OPTION].....\
	\n Binary FILE Type Detector By Hexadecimal Signatures.\
  	\n\t-f,                   Load The Target File \
  	\n\t-i                    Load The INI File" << endl;
	exit(1);
}

