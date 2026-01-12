#include "WAVParser.h"
#include <fstream>
#include <iostream>
#include <optional>
WAVParser::WAVParser()
{
}

WAVParser::~WAVParser()
{

}

WAVParser::WAV_INFO WAVParser::Parse(std::string path)
{
	WAV_INFO info;
	WAV_FILE_HEADER header;
	std::fstream s(path, std::ios::binary | std::ios::in);
	s.read(reinterpret_cast<char*>(&header), sizeof(WAV_FILE_HEADER));
	header.bytePerSample /= 8;
	info.header = header;

	while (s.eof() == false)
	{
		WAV_DATA_HEADER dataHeader;
		s.read(reinterpret_cast<char*>(&dataHeader), sizeof(WAV_DATA_HEADER));
		if (dataHeader.dataBlockID == 0x61746164)
		{
			info.dataHeader = dataHeader;

			char* buffer = new char[dataHeader.dataSize];
			s.read(buffer, dataHeader.dataSize);
			info.data = buffer;
		}

	}
	return info;
}

void WAVParser::Write(std::string path, WAV_INFO info)
{

	std::fstream s(path, std::ios::binary | std::ios::out);
	s.write(reinterpret_cast<char*>(&info.header), sizeof(WAV_FILE_HEADER));

	s.write(reinterpret_cast<char*>(&info.dataHeader), sizeof(WAV_DATA_HEADER));
	s.write(info.data, info.dataHeader.dataSize);
}

void WAVParser::Debug(std::string path)
{
	WAV_FILE_HEADER infos;
	std::fstream s(path, std::ios::binary | std::ios::in);
	s.read(reinterpret_cast<char*>(&infos), sizeof(WAV_FILE_HEADER));
	infos.bytePerSample /= 8;

	std::cout << "fileTypeBlockID : "<< infos.fileTypeBlockID << std::endl;
	std::cout << "fileSize : " << infos.fileSize << std::endl;
	std::cout << "fileFormatID : " << infos.fileFormatID << std::endl;

	std::cout << "blocSize : " << infos.blocSize << std::endl;
	std::cout << "audioFormat : " << infos.audioFormat << std::endl;
	std::cout << "nbChannel : " << infos.nbChannel << std::endl;
	std::cout << "frequency : " << infos.frequency << std::endl;
	std::cout << "bytePerSec : " << infos.bytePerSec << std::endl;
	std::cout << "bytePerFrame : " << infos.bytePerFrame << std::endl;
	std::cout << "bytePerSample : " << infos.bytePerSample << std::endl;

	while (s.eof() == false)
	{
		WAV_DATA_HEADER data;
		s.read(reinterpret_cast<char*>(&data), sizeof(WAV_DATA_HEADER));

		if (data.dataBlockID == 0x61746164)
		{
			std::cout << "blocID : " << data.dataBlockID << std::endl;
			std::cout << "dataSize : " << data.dataSize << std::endl;
			char* buffer = new char[data.dataSize];
			s.read(buffer, data.dataSize);
		}

	}
}
