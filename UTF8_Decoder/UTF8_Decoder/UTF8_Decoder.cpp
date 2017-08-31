#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <codecvt>

/*
Charles Bisbee
8/25/17
CSCI 450: Compiler Structure
This program is designed to read in UTF-8 encoded text file with unicode characters
and for each character output its code point (in hex) into a seperate file.
*/

void readRawBytes(std::ifstream&, std::ofstream&);
uint32_t decode4ByteUTF8(std::vector<uint8_t>);
uint32_t decode3ByteUTF8(std::vector<uint8_t>);
uint32_t decode2ByteUTF8(std::vector<uint8_t>);
uint32_t decode1ByteUTF8(uint8_t);

int main(int argc, char* argv[]) {
	std::string inputFileName, outputFileName;
	std::ifstream fin;
	std::ofstream fout;

	if (argc > 1) inputFileName = argv[1];
	else {
		std::cout << "Enter a UTF-8 encoded file to convert: ";
		std::cin >> inputFileName;
	}

	outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + "_CodePoints.txt";
	fin.open(inputFileName, std::ifstream::binary);
	if (!fin) {
		std::cout << "The requested file could not be opened, exiting program...";
		return 0;
	}

	fout.open(outputFileName);
	readRawBytes(fin, fout);

	return 0;
}

void readRawBytes(std::ifstream &fin, std::ofstream &fout) {
	char byte;
	std::vector<uint8_t> encodedData;
	while (fin.read(&byte, 1)) {
		int i = 0;
		encodedData.push_back((uint8_t)byte);
		while ((byte << i) & 1) ++i;
		switch (i)
		{
		case 0:
			fout << std::hex << "0x" << decode1ByteUTF8((uint8_t)byte) << std::endl;
			break;
		case 2:
			fin.read(&byte, 1);
			encodedData.push_back((uint8_t)byte);
			fout << std::hex << "0x" << decode2ByteUTF8(encodedData);
			break;
		case 3:
			for (int i = 0; i < 2; ++i) { fin.read(&byte, 1); encodedData.push_back((uint8_t)byte); }
			fout << std::hex << "0x" << decode3ByteUTF8(encodedData) << std::endl;
			break;
		case 4:
			for (int i = 0; i < 4; ++i) { fin.read(&byte, 1); encodedData.push_back((uint8_t)byte); }
			fout << std::hex << "0x" << decode4ByteUTF8(encodedData) << std::endl;
			break;
		default:
			break;
		}
		encodedData.clear();
	}
}

uint32_t decode4ByteUTF8(std::vector<uint8_t> encodedData) {
	uint32_t codePoint =
		((((uint32_t)encodedData[0]) & 0x7) << 18) |
		((((uint32_t)encodedData[1]) & 0x3F) << 12) |
		((((uint32_t)encodedData[2]) & 0x3F) << 6) |
		((((uint32_t)encodedData[3]) & 0x3F) << 0);

	return codePoint;
}

uint32_t decode3ByteUTF8(std::vector<uint8_t> encodedData) {
	uint32_t codePoint =
		((((uint32_t)encodedData[0]) & 0xF) << 12) |
		((((uint32_t)encodedData[1]) & 0x3F) << 6) |
		((((uint32_t)encodedData[2]) & 0x3F) << 0);

	return codePoint;
}

uint32_t decode2ByteUTF8(std::vector<uint8_t> encodedData) {
	uint16_t codePoint =
		((((uint16_t)encodedData[0]) & 0x1F) << 6) |
		((((uint16_t)encodedData[1]) & 0x3F) << 0);
	return codePoint;
}

uint32_t decode1ByteUTF8(uint8_t encodedData) {
	return (((encodedData)& 0x7F) << 0);
}