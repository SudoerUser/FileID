install:

	@mkdir ./bin
	g++ ./src/main.cpp -o ./bin/FileID
	@cp ./src/userdb.ini ./bin/userdb.ini
	@echo Compiled
remove:
	@rm -rf ./bin
