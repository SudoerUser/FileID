#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int read_to_show(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int create_table(char *dbname)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open(dbname, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(db));
		return 0;
	}

	/* Create SQL statement */

	sql = "CREATE TABLE FILEID("
	    "NAME           TEXT    NOT NULL,"
	    "SIGN           TEXT    NOT NULL,"
	    "ep_verify      TEXT    NOT NULL);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Database created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}

int sign_todb(char *dbname, char *name, char *sign, char *verify)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	string sig(sign);
	string verf(verify);
	string nm(name);
	/* Open database */
	rc = sqlite3_open(dbname, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(db));
		return 0;
	}

	/* Create SQL statement */
	string sql =
	    "INSERT INTO FILEID (NAME,SIGN,ep_verify) VALUES ('" + nm + "','" +
	    sig + "','" + verf + "')";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "The sign added successfully\n");
	}
	sqlite3_close(db);
	return 0;
}

int shower(char *dbname)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char *data = "Callback function called";

	/* Open database */
	rc = sqlite3_open(dbname, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(db));
		return 0;
	}

	/* Create SQL statement */
	sql = "SELECT SIGN from FILEID ";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, read_to_show, (void *)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}

static int delete_callback(void *data, int argc, char **argv, char **azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char *)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int delete_row(char *dbname, string sign)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *data = "Callback function called";

	/* Open database */
	rc = sqlite3_open(dbname, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(db));
		return 0;
	}

	/* Create merged SQL statement */
	string sql = "delete from FILEID where SIGN='" + sign + "';";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), delete_callback, (void *)data,
			  &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "The sign deleted successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
