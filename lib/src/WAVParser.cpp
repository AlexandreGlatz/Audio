#include "WAVParser.h"
#include <fstream>
#include <iostream>

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
	info.header.bytePerSample *= 8;
	std::fstream s(path, std::ios::binary | std::ios::out);
	s.write(reinterpret_cast<char*>(&info.header), sizeof(WAV_FILE_HEADER));

	s.write(reinterpret_cast<char*>(&info.dataHeader), sizeof(WAV_DATA_HEADER));
	s.write(info.data, info.dataHeader.dataSize);
}

std::optional<WAVParser::WAV_INFO> WAVParser::Serialise(WAV_FILE_HEADER header, char* data, int dataSize)
{
	if (_CheckHeaderValidity(header) == false)
		return {};

	WAV_INFO info;
	header.fileTypeBlockID = RIFF_ID;
	header.fileSize = sizeof(WAV_FILE_HEADER) + sizeof(WAV_DATA_HEADER) + dataSize - 8;
	header.fileFormatID = WAVE_ID;

	header.blocFormatID = FMT_ID;
	header.blocSize = 16;
	header.bytePerSec = header.sampleRate;
	header.bytePerFrame = header.nbChannel * header.bytePerSample;

	WAV_DATA_HEADER dataHeader;
	dataHeader.dataBlockID = DATA_ID;
	dataHeader.dataSize = dataSize;

	info.header = header;
	info.dataHeader = dataHeader;
	info.data = data;

	return info;
}

void WAVParser::Cut(WAV_INFO* pInfo, float begin, float end)
{
	if (end <= begin)
		return;

	if (pInfo == nullptr)
		return;

	int duration = (end - begin) * pInfo->header.sampleRate * pInfo->header.bytePerFrame;

	if (duration >= pInfo->dataHeader.dataSize)
		return;

	int cutSize = pInfo->dataHeader.dataSize - duration;
	char* cutData = new char[cutSize];

	int start = begin * pInfo->header.sampleRate * pInfo->header.bytePerFrame;
	int over = end * pInfo->header.sampleRate * pInfo->header.bytePerFrame;
	int index = 0;
	
	memcpy(cutData, pInfo->data, start);
	memcpy(cutData + start, pInfo->data + over, pInfo->dataHeader.dataSize - over);

	pInfo->data = cutData;
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

	std::cout << "blocFormatID : " << infos.blocFormatID << std::endl;
	std::cout << "blocSize : " << infos.blocSize << std::endl;
	std::cout << "audioFormat : " << infos.audioFormat << std::endl;
	std::cout << "nbChannel : " << infos.nbChannel << std::endl;
	std::cout << "sampleRate : " << infos.sampleRate << std::endl;
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

bool WAVParser::_CheckHeaderValidity(WAV_FILE_HEADER header)
{
	return header.bytePerSample != INVALID_INFO && header.audioFormat != INVALID_INFO && header.nbChannel != INVALID_INFO && header.sampleRate != INVALID_INFO;
}
