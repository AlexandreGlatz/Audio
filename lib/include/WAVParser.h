#ifndef _WAV_H__
#define _WAV_H__
#include "Define.h"
#include <string>

class WAVParser
{
public:
	struct WAV_FILE_HEADER
	{
		uint32 fileTypeBlockID;
		uint32 fileSize; //-8
		uint32 fileFormatID;

		uint32 blocFormatID;
		uint32 blocSize;
		uint16 audioFormat;
		uint16 nbChannel;
		uint32 frequency;
		uint32 bytePerSec;
		uint16 bytePerFrame;
		uint16 bytePerSample; //to divide by 8;
	};

	struct WAV_DATA_HEADER
	{
		uint32 dataBlockID;
		uint32 dataSize;
	};

	struct WAV_INFO
	{
		WAV_FILE_HEADER header;
		WAV_DATA_HEADER dataHeader;
		char* data;
	};

	WAVParser();
	~WAVParser();

	static WAV_INFO Parse(std::string path);
	static void Debug(std::string path);
	static void Write(std::string path, WAV_INFO info);

private:
};

#endif