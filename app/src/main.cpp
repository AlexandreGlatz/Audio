#include "WAVParser.h"
#include <vector>
#include <optional>

int main(int argc, char* argv[])
{
	//WAVParser::WAV_INFO info = WAVParser::Parse(argv[1]);
	//WAVParser::Write(std::string(argv[2]), info);

    int time = 100;
    // Oscillateur sinusoidal sur 1 canal 
    float* audio_buffer = new float[44100 * time]; // Fréquence d'échantillonnage * 5 (= 5 secondes de son) 
    const float freq = 440.0f; // 440 hz -> tonalité du téléphone 
    for (size_t i = 0; i < 44100 * time; ++i)
    {
        audio_buffer[i] = 0.3 * sinf(2.0f * 3.14159265f * freq * (float)i / (float)44100); // 0.3 c'est l'atténuation d'amplitude 
    }

    WAVParser::WAV_FILE_HEADER header;
    header.bytePerSample = 32 / 8;
    header.nbChannel = 1;
    header.sampleRate = 44100;
    header.audioFormat = 3;

    std::optional<WAVParser::WAV_INFO> serialised = WAVParser::Serialise(header, reinterpret_cast<char*>(audio_buffer), 44100 * time);
    WAVParser::WAV_INFO test = serialised.value();

    WAVParser::Cut(&test, 1, 2);
    WAVParser::Write("../res/tes.wav", test);

}

