#include "WAVParser.h"

int main(int argc, char* argv[])
{
	WAVParser::WAV_INFO info = WAVParser::Parse(argv[1]);
	WAVParser::Write(std::string(argv[2]), info);
}

