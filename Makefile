install:

	@mkdir ./bin
	g++ ./src/main.cpp -o ./bin/FileID
	@echo Compiled
remove:
	@rm -rf ./bin
