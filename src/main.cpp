#include <iostream>
#include <fstream>
#include <string>
#include <json/value.h>
#include <regex>
#include "libs/hexlib.hpp"

#define pass (void)0

using namespace std;

int checker(string, string);

int main(int argc, char **argv)
{

	char ch;
	string hexvalues;
	//check argc
	if (argc != 3) {
		cout << " Usage\n\t" 
			<< argv[0] << " <file> " << " <hex_value> ";
		exit(1);
	}

	ifstream fin(argv[1], ios::binary);

	if (!fin) {
		cout << "\nFailed..." << endl;
		exit(1);
	}

	for (int i = 0; !fin.eof(); i++) {
		fin.get(ch);
		hexvalues.push_back(ch);
	}

	fin.close();
	hexvalues = string_to_hex(hexvalues);

	if (checker(argv[2], hexvalues) != 0)
		cout << "found";
}

int checker(string pattrn,string hexvalue) {
    string pattern(pattrn);
    regex rx(pattern);
    
    string s(hexvalue);
    ptrdiff_t number_of_matches = distance(
        sregex_iterator(s.begin(), s.end(), rx),
        sregex_iterator());
    
    return number_of_matches;
}
