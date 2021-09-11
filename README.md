
# FileID


**FileID** is a simple tool to detect binary file types by hexadecimal signatures. FileID uses regex for searching in binary file.

**How to work with FileID?**

This program is very easy to use.
Use ```make``` command to compile the source code. and in result you see this:
```
$ ./fileid
Usage: ./fileid  [OPTION]	
 Binary FILE Type Detector By Hexadecimal Signatures.	
	-a --add				Add sign to database 	
	-c --create				Create a new Database 	
	-d --delete				Delete sign   	
	-f --file				Load the file   	
	-i --database				Load database 	
	-n --name				Name of sign	
	-s --show				Show signs in database	
	-v --verify				Verify entry when append sign	
Example:
	fileid -c <database_path>			
	fileid --database <database_path> --add <sign> --name <sign_name> --verify <true_or_anyelse>			
	fileid --database <database_path> --file <file> 			
	fileid --database <database_path> --delete <sign>			
	fileid --database <database_path> --show	
```
