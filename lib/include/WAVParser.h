#ifndef _WAV_H__
#define _WAV_H__

#include "Define.h"
#include <string>
#include <optional>
#define INVALID_INFO 0

#define RIFF_ID 0x46464952
#define WAVE_ID 0x45564157
#define FMT_ID  0x20746D66
#define DATA_ID 0x61746164

class WAVParser
{
public:
	struct WAV_FILE_HEADER
	{
		uint32 fileTypeBlockID	= INVALID_INFO;
		uint32 fileSize			= INVALID_INFO; //-8
		uint32 fileFormatID		= INVALID_INFO;

		uint32 blocFormatID		= INVALID_INFO;
		uint32 blocSize			= INVALID_INFO;
		uint16 audioFormat		= INVALID_INFO;
		uint16 nbChannel		= INVALID_INFO;
		uint32 sampleRate		= INVALID_INFO;
		uint32 bytePerSec		= INVALID_INFO;
		uint16 bytePerFrame		= INVALID_INFO;
		uint16 bytePerSample	= INVALID_INFO; //to divide by 8;

		WAV_FILE_HEADER()
		{ }
	};

	struct WAV_DATA_HEADER
	{
		uint32 dataBlockID;
		uint32 dataSize;
		WAV_DATA_HEADER()
		{ }
	};

	struct WAV_INFO
	{
		WAV_FILE_HEADER header;
		WAV_DATA_HEADER dataHeader;
		char* data;

		WAV_INFO()
		{ }
	};

	WAVParser();
	~WAVParser();

	static WAV_INFO Parse(std::string path);
	static void Debug(std::string path);
	static void Write(std::string path, WAV_INFO info);
	static std::optional<WAV_INFO> Serialise(WAV_FILE_HEADER header, char* data, int dataSize);

private:
	static bool _CheckHeaderValidity(WAV_FILE_HEADER header);
};

#endif