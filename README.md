
# FileID


**FileID** is a simple tool to detect binary file types by hexadecimal signatures. FileID uses regex for searching in binary file.

**How to work with FileID?**

This program is very easy to use.
Use ```make``` command to compile the source code. and in result you see this:
```bash
[amir@Sudoer bin (master)]$ ./FileID
Usage:./FileID[OPTION]....[OPTION].....
Binary FILE Type Detector By Hexadecimal Signatures.
	-f, Load The Target File
	-i Load The INI File
[amir@Sudoer bin (master)]$ ./FileID -f FileID -i userdb.ini
*ELF : 7f454c46 Found!!!
[amir@Sudoer bin (master)]$
```
