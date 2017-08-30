#include <iostream>
#include <fstream>
#include <string>

/*
Charles Bisbee
8/25/17
CSCI 450: Compiler Structure

This program is designed to read in UTF-8 encoded text file with unicode characters
and for each character output its code point (in hex) into a seperate file.
*/

void readBytes(std::ifstream&, std::ofstream&);

int main(int argc, char* argv) {
	std::string inputFileName, outputFileName;
	std::ifstream fin;
	std::ofstream fout;

	std::cout << "Enter a UTF-8 encoded file to convert: ";
	std::cin >> inputFileName;
	outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + "_CodePoints.txt";

	fin.open(inputFileName, std::ifstream::binary);
	if (!fin) {
		std::cout << "The requested file could not be opened, exiting program...";
		return 0;
	}
	fout.open(outputFileName);

	readBytes(fin, fout);

	return 0;
}

void readBytes(std::ifstream &fin, std::ofstream &fout) {
	char byte;
	int codePoint = 0, currentBitNum = 0, bytesAllowed = 0, currentByte = 0, allowedBits = 0, i;
	bool newCodePoint = true;
	
	while (fin.read(&byte, 1)) {
		if (currentByte <= bytesAllowed) {
			i = CHAR_BIT - 3; //Skip the '10' at the start of the byte if you are starting a new byte but are not in a new encoding
		}
		else {
			//output the code point in hex
			fout << std::hex << "0x" << codePoint << std::endl;
			codePoint = 0;
			newCodePoint = true;
			bytesAllowed = 0;
			currentByte = 1;
			i = CHAR_BIT - 1;
		}
		//Bytes are encoded in reverse order that wikipedia states, start from the back of the char when adding to the bit string
		while (i >= 0) {
			if (newCodePoint) {
				int precedingOnes = 0;
				while ((byte >> i) & 1) {
					--i;
					++precedingOnes;
				}
				newCodePoint = false;
				currentBitNum = 1;
				currentByte = 1;
				switch (precedingOnes)
				{
				case 0:
					allowedBits = 7;
					bytesAllowed = 1;
					break;
				case 2:
					allowedBits = 11;
					bytesAllowed = 2;
					break;
				case 3:
					allowedBits = 16;
					bytesAllowed = 3;
					break;
				case 4:
					allowedBits = 21;
					bytesAllowed = 4;
					break;
				default:
					break;
				}
			}
			else if (currentBitNum <= allowedBits) {
				if ((byte >> i) & 1) codePoint += pow(2, (allowedBits - currentBitNum));
				++currentBitNum;
			}
			--i;
		}
		++currentByte;
	}
	fout << std::hex << "0x" << codePoint << std::endl;
}