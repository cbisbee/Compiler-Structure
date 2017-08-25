#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

/*
Charles Bisbee
8/25/17
CSCI 450: Compiler Structure

This program is designed to read in UTF-8 encoded text file with unicode characters
and for each character output its code point (in hex) into a seperate file.
*/

void readBytes(std::ifstream &fin);
std::string trimFileExtension(std::string);

int main(int argc, char* argv) {
	std::string inputFileName;
	std::string outputFileName;
	std::ifstream fin;
	std::ofstream fout;

	std::cout << "Enter a UTF-8 encoded file to convert: ";
	std::cin >> inputFileName;
	outputFileName = trimFileExtension(inputFileName) + "_CodePoints.txt";

	fin.open(inputFileName, std::ifstream::binary);
	if (!fin) {
		std::cout << "The requested file could not be opened, exiting program...";
		return 0;
	}

	readBytes(fin);

	std::cin.get();
	std::cin.get();

	return 0;
}

void readBytes(std::ifstream &fin) {
	char byte;
	int codePoint = 0;
	int currentBitNum = 0;
	int allowedBits = 0;
	bool newCodePoint = true;
	
	while (fin.read(&byte, 1)) {
		//Bytes are encoded in reverse order that wikipedia states,
		//start from the back of the char when adding to the bit string
		int i = CHAR_BIT - 1;
		while (i >= 0) {
			if (newCodePoint) {
				int precedingOnes = 0;
				while ((byte >> i) & 1) {
					--i;
					++precedingOnes;
				}
				newCodePoint = false;
				currentBitNum = 0;
				switch (precedingOnes)
				{
				case 0:
					allowedBits = 7;
					break;
				case 2:
					allowedBits = 11;
					break;
				case 3:
					allowedBits = 16;
					break;
				case 4:
					allowedBits = 21;
					break;
				default:
					break;
				}
			}
			else if (currentBitNum < allowedBits) {
				if ((byte >> i) & 1) codePoint += pow(2, currentBitNum);
				++currentBitNum;
			}
			else {
				//output the code point in hex
				std::cout << std::hex << "0x" << codePoint << std::endl;
				codePoint = 0;
				newCodePoint = true;
			}
			--i;
		}
	}
}

std::string trimFileExtension(std::string ogFileName) {
	return ogFileName.substr(0, ogFileName.length() - 4);
}