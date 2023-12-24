#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>

#include "error.h"
#include "scanner.h"

void run(const std::string& source){
    //std::cout << source << "\n";
    Scanner scanner(source);
    std::vector<Token> tokens{ scanner.scanTokens() };

    for (const auto& token : tokens) {
        std::cout << token.toString() << "\n";
    }
}

void runFile(const std::filesystem::path& filePath) {
    // Check if the file exists
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File not found: " << filePath.string() << std::endl;
    }

    // Open the file for reading
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);

    // Check if the file is opened successfully
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open file: " << filePath.string() << std::endl;

    }

    // Read the contents of the file into a string
    std::string fileContents((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    // Close the file
    fileStream.close();

    run(fileContents);

    if (Error::hadError) std::exit(65);
}

void runPrompt() {
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin,line);
        if (line.empty()) break;
        run(line);
        Error::hadError = false;
    }
}


int main(int argv, char** argc) {

	if (argv > 2) {
		std::cout << "Usage: jlox [script]" << std::format("haha") << std::endl;
		return 0;
	}
	else if (argv == 2) {
		runFile(argc[1]);
	}
	else {
		runPrompt();
	}
}