#include "WAVParser.h"
#include <vector>
#include <optional>
#include <Windows.h>
#include <xaudio2.h>
#include <iostream>
#include <cmath>
#include "Effect.hpp"

int main(int argc, char* argv[])
{
	//WAVParser::Write(std::string(argv[2]), info);

    //int time = 100;
    //// Oscillateur sinusoidal sur 1 canal 
    //float* audio_buffer = new float[44100 * time]; // Fréquence d'échantillonnage * 5 (= 5 secondes de son) 
    //const float freq = 440.0f; // 440 hz -> tonalité du téléphone 
    //for (size_t i = 0; i < 44100 * time; ++i)
    //{
    //    audio_buffer[i] = 0.3 * sinf(2.0f * 3.14159265f * freq * (float)i / (float)44100); // 0.3 c'est l'atténuation d'amplitude 
    //}

    //WAVParser::WAV_FILE_HEADER header;
    //header.bytePerSample = 32 / 8;
    //header.nbChannel = 1;
    //header.sampleRate = 44100;
    //header.audioFormat = 3;

    //std::optional<WAVParser::WAV_INFO> serialised = WAVParser::Serialise(header, reinterpret_cast<char*>(audio_buffer), 44100 * time);
    //WAVParser::WAV_INFO test = serialised.value();

    //WAVParser::Cut(&test, 1, 2);
    //WAVParser::Write("../res/tes.wav", test);

    // Initialisation COM
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); 
    if (FAILED(hr)) return hr;

    // On créé l'instance de XAudio2 
    IXAudio2* xaudio2 = nullptr;
    hr = XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) return hr;

    // On créé la voix master : elle start Xaudio2 automatiquement (on n'appelle pas xaudio2->Start())
    IXAudio2MasteringVoice* master_voice = nullptr;
    hr = xaudio2->CreateMasteringVoice(&master_voice);
    if (FAILED(hr)) return hr;

    WAVParser::WAV_INFO info = WAVParser::Parse("../res/b423b42.wav");

    // On créé un WAVEFORMATEX (similaire à la structure d'un fichier WAVE, qui inclut en puissance le Broadcast extension format)
    // Attention WAVEFORMAT (sans EX) est XAUDIO1 (non rétrocompatible)
    WAVEFORMATEX wfx = {};
    wfx.wFormatTag = info.header.audioFormat;
    wfx.nChannels = info.header.nbChannel;
    wfx.nSamplesPerSec = info.header.sampleRate;
    wfx.wBitsPerSample = info.header.bytePerSample * 8;
    wfx.nBlockAlign = info.header.nbChannel * (info.header.bytePerSample);
    wfx.nAvgBytesPerSec = info.header.bytePerSec;

    // Source voice = un lecteur de buffer audio / playback
    IXAudio2SourceVoice* source_voice = nullptr;
    hr = xaudio2->CreateSourceVoice(&source_voice, &wfx);
    if (FAILED(hr)) return hr;

    // La structure buffer XAudio2 : on lui donne notre buffer + quelques infos
    XAUDIO2_BUFFER buf = {};
    buf.pAudioData = reinterpret_cast<BYTE*>(info.data);
    buf.AudioBytes = info.dataHeader.dataSize * info.header.bytePerSample;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    // On donne le buffer à la source 
    source_voice->SubmitSourceBuffer(&buf);

    // Et c'est parti ! 
    source_voice->Start();

    IXAudio2* pXAudio2 = nullptr;
    IXAudio2MasteringVoice* pMasterVoice = nullptr;

    // Initialisation
    XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    pXAudio2->CreateMasteringVoice(&pMasterVoice);

    // Créer l’effet
    MyGainEffect* myEffect = new MyGainEffect();

    // Chaîne d’effets
    XAUDIO2_EFFECT_DESCRIPTOR effectDesc = { myEffect, TRUE, 1 };
    XAUDIO2_EFFECT_CHAIN effectChain = { 1, &effectDesc };

    // Appliquer à la voix mastering
    pMasterVoice->SetEffectChain(&effectChain);

    // Exemple : changer le gain
    myEffect->m_gain = 2.0f; // double le volume
    while (true) {
        XAUDIO2_VOICE_STATE state;
        source_voice->GetState(&state);
        if (state.BuffersQueued == 0) break;
        Sleep(100);
    }

    source_voice->DestroyVoice();
    master_voice->DestroyVoice();
    xaudio2->Release();
    CoUninitialize();

}

