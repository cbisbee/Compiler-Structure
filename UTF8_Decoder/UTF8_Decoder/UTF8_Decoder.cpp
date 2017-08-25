#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>

/*
Charles Bisbee
8/25/17
CSCI 450: Compiler Structure

This program is designed to read in UTF-8 encoded text file with unicode characters
and for each character output its code point (in hex) into a seperate file.
*/

void readBytes(std::ifstream &fin, std::vector<bool>&);
void getCodePointsFromBits(std::vector<bool>&, std::ofstream&);
std::string trimFileExtension(std::string);

int main(int argc, char* argv) {
	std::string inputFileName;
	std::string outputFileName;
	std::ifstream fin;
	std::ofstream fout;
	std::vector<bool> bitBuffer;

	std::cout << "Enter a UTF-8 encoded file to convert: ";
	std::cin >> inputFileName;
	outputFileName = trimFileExtension(inputFileName) + "_CodePoints.txt";

	fin.open(inputFileName, std::ifstream::binary);
	if (!fin) {
		std::cout << "The requested file could not be opened, exiting program...";
		return 0;
	}

	readBytes(fin, bitBuffer);
	getCodePointsFromBits(bitBuffer, fout);
	

	return 0;
}

void readBytes(std::ifstream &fin, std::vector<bool> &bitBuffer) {
	char byte;
	
	while (fin.read(&byte, 1)) {
		//Bytes are encoded in reverse order that wikipedia states,
		//start from the back of the char
		for (int i = CHAR_BIT - 1; i >= 0; --i) {
			bitBuffer.push_back(((byte >> i) & 1));
		}
	}
}

void getCodePointsFromBits(std::vector<bool> &bitBuffer, std::ofstream &fout) {
	bool newCodePoint = true;
	int codePointSize = 0;
	int bytesAllowed = 0;
	int currentByteNumber = 0;
	int bitPosition = 0;
	for (int i = 0; i < bitBuffer.size(); ++i) {
		if (newCodePoint) {
			//Conventional ASCII Encoded Character
			if (!bitBuffer[i]) {
				codePointSize = 7;
				bytesAllowed = 1;
				currentByteNumber = 1;
				newCodePoint = false;
				bitPosition = 0;
			}
		}

		//Start reading in actual code point
		

	}
}

std::string trimFileExtension(std::string ogFileName) {
	return ogFileName.substr(0, ogFileName.length() - 3);
}