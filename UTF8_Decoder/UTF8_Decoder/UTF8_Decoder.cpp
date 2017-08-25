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

	fin.open(inputFileName, std::ios::binary);
	if (!fin) {
		std::cout << "The requested file could not be opened, exiting program...";
		return 0;
	}

	readBytes(fin, bitBuffer);
	

	return 0;
}

void readBytes(std::ifstream &fin, std::vector<bool> &bitBuffer) {
	char byte;
	
	while (fin.read(&byte, 1)) {
		for (int i = 0; i < CHAR_BIT; ++i) {
			bitBuffer.push_back(((byte >> i) & 1) != 0);
		}
	}
}

std::string trimFileExtension(std::string ogFileName) {

}