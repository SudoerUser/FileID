install:
	@g++ ./src/main.cpp -o ./fileid -lsqlite3
	@echo Compiled
remove:
	@rm -rf ./fileid



