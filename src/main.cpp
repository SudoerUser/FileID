#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <getopt.h>
#include <sqlite3.h>
#include "libs/hexlib.hpp"
#include "libs/sqlite.hpp"

using namespace std;

void checker(string, string, string, string);
int signvalue(void *, int, char **, char **);
int check(char *, char *);
int banner(string);
string hexvalues;

static int verbose_flag;

int main(int argc, char **argv)
{
	int opt;
	int aflag = 0;
	int cflag = 0;
	int dflag = 0;
	int fflag = 0;
	int iflag = 0;
	int sflag = 0;
	int vflag = 0;
	int nflag = 0;

	char *avalue = NULL;
	char *cvalue = NULL;
	char *dvalue = NULL;
	char *fvalue = NULL;
	char *ivalue = NULL;
	char *svalue = NULL;
	char *vvalue = NULL;
	char *nvalue = NULL;
	char *zErrMsg = 0;

	while (1) {
		static struct option long_options[] = {
			/* These options set a flag. */
			{"verbose", no_argument, &verbose_flag, 1},
			{"brief", no_argument, &verbose_flag, 0},
			{"add", required_argument, 0, 'a'},
			{"create", required_argument, 0, 'c'},
			{"delete", required_argument, 0, 'd'},
			{"file", required_argument, 0, 'f'},
			{"database", required_argument, 0, 'i'},
			{"verify", required_argument, 0, 'v'},
			{"name", required_argument, 0, 'n'},
			{"show", no_argument, 0, 's'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		int c = getopt_long(argc, argv, "a:c:d:f:i:v:n:s",
				    long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c) {
			case 'a':
				aflag = 1;
				avalue = optarg;
				break;
			case 'c':
				cflag = 1;
				cvalue = optarg;
				break;
			case 'd':
				dflag = 1;
				dvalue = optarg;
				break;
			case 'i':
				iflag = 1;
				ivalue = optarg;
				break;
			case 'f':
				fflag = 1;
				fvalue = optarg;
				break;
			case 's':
				sflag = 1;
				break;
			case 'v':
				vflag = 1;
				vvalue = optarg;
				break;
			case 'n':
				nflag = 1;
				nvalue = optarg;
				break;

			default:
				banner(argv[0]);
			}
	}

	if (((iflag && fflag) == 1)
	    && ((aflag || cflag || dflag || vflag || sflag || nflag) == 0)) {
		check(fvalue, ivalue);
	} else if (((iflag && aflag && nflag && vflag) == 1)
		   && (fflag || cflag || dflag || sflag) == 0) {
		sign_todb(ivalue, nvalue, avalue, vvalue);
	} else if (((cflag) == 1)
		   &&((iflag || fflag || aflag || dflag || vflag || sflag|| nflag) == 0)) {
		create_table(cvalue);
	} else if (((iflag && sflag) == 1)
		   && ((fflag || aflag || cflag || dflag || vflag || nflag) == 0)) {
		shower(ivalue);
	} else if (((dflag && iflag) == 1)
		   && ((fflag || aflag || cflag || vflag || nflag || sflag) == 0)) {
		delete_row(ivalue, dvalue);
	} else {
		banner(argv[0]);
	}

	return 0;
}

int banner(string argzero)
{
	cout << "Usage: " << argzero << "  [OPTION]\
	\n Binary FILE Type Detector By Hexadecimal Signatures.\
	\n\t-a --add				Add sign to database \
	\n\t-c --create				Create a new Database \
	\n\t-d --delete				Delete sign \
  	\n\t-f --file				Load the file \
  	\n\t-i --database				Load database \
	\n\t-n --name				Name of sign\
	\n\t-s --show				Show signs in database\
	\n\t-v --verify				verify entry when append sign\
	\nExample:\n\
	fileid -c <database_path>\
			\n\tfileid --database <database_path> --add <sign> --name <sign_name> --verify <true_or_anyelse>\
			\n\tfileid --database <database_path> --file <file> \
			\n\tfileid --database <database_path> --delete <sign>\
			\n\tfileid --database <database_path> --show\
	" << endl;

	exit(1);
}

int check(char *fvalue, char *ivalue)
{
	sqlite3 *db;
	ifstream fin(fvalue, ios::binary);
	char *zErrMsg = 0;
	char *sql;
	const char *data = "Callback function called";

	int rc = sqlite3_open(ivalue, &db);
	char ch;

	// Open Files Error Handling 
	if (!fin) {
		cout << "Error : Cannot Open " << fvalue << endl;
		exit(1);
	}

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(db));
		return 0;
	}
	//Getting file bytes into hexvalues var
	for (int i = 0; !fin.eof(); i++) {
		fin.get(ch);
		hexvalues.push_back(ch);
	}
	hexvalues = string_to_hex(hexvalues);
	fin.close();

	sql = "SELECT SIGN , NAME , ep_verify from FILEID ";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, signvalue, (void *)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
}

void checker(string pattrn, string hexvalue, string section, string ep_verify)
{
	string pattern(pattrn);
	if (ep_verify == "true") {
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

int signvalue(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++) {
		checker(argv[i] ? argv[i] : "NULL",
			hexvalues,
			argv[i + 1] ? argv[i + 1] : "NULL",
			argv[i + 2] ? argv[i + 2] : "NULL");
	}
	return 0;
}
