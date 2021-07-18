#include <iostream>
#include <fstream>
#include <filesystem>

void printHelp();

std::string encryptArg = "encrypt";
std::string decryptArg = "decrypt";

int main(int argc, char* argv[]) {

	if (argc < 4) {
		printHelp();
		return 1;
	}

	char* inputfile = argv[1];
	char* outputfile = argv[2];
	char* option = argv[3];
	
	if (option == decryptArg) {
		std::ifstream scorefile;
		std::ofstream decryptscorefile;

		if (!std::filesystem::exists(inputfile)) {
			std::cout << "Input file does not exists!";
			return 1;
		}

		scorefile.open(inputfile, std::ios::in | std::ios::binary);
		decryptscorefile.open(outputfile, std::ios::out | std::ios::binary | std::ios::trunc);

		scorefile.seekg(0);

		char* buffer = new char[1];
		scorefile.read(buffer, 1);
		decryptscorefile.write(buffer, 1);
		delete[] buffer;

		int key = 0;

		while (true) {
			if (scorefile.eof()) { break; }
			char* c = new char[1];
			scorefile.read(c, 1);

			char* encrypted = c;

			key = ((key << 3) & 0xFF) | ((key >> 5) & 7);

			char clear = *encrypted ^ key;

			key += clear;

			decryptscorefile.write(&clear, 1);

			delete[] c;
		}

		scorefile.close();
		decryptscorefile.close();
	}
	else if (option == encryptArg) {
		std::ifstream scorefile;
		std::ofstream encryptscorefile;

		if (!std::filesystem::exists(inputfile)) {
			std::cout << "Input file does not exists!";
			return 1;
		}

		scorefile.open(inputfile, std::ios::in | std::ios::binary);
		encryptscorefile.open(outputfile, std::ios::out | std::ios::binary | std::ios::trunc);

		scorefile.seekg(0);

		char* buffer = new char[1];
		scorefile.read(buffer, 1);
		encryptscorefile.write(buffer, 1);
		delete[] buffer;

		int key = 0;

		while (true) {
			if (scorefile.eof()) { break; }
			char* c = new char[1];
			scorefile.read(c, 1);
			
			char* clear = c;

			key = ((key << 3) & 0xFF) | ((key >> 5) & 7);

			char encrypted = *clear ^ key;

			key += *clear;

			encryptscorefile.write(&encrypted, 1);

			delete[] c;
		}

		scorefile.close();
		encryptscorefile.close();
	} else {
		std::cout << option << " is not a valid option" << std::endl;
		printHelp();
	}

	return 0;
}

void printHelp()
{
	std::cout << "Usage: EoScoreDecryptor.exe [input file] [output file] OPTIONS\n\nOPTIONS are:\n" << decryptArg << " | takes encrypted score and decrypts it\n" << encryptArg << " | takes decrypted score and encrypt it" << std::endl;
}