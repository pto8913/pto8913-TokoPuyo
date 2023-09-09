#pragma once

#include <xaudio2.h>

// ------------------------------------------------------------------------
// AudioManagerHelper
// ------------------------------------------------------------------------
class AudioHelperLibrary
{
public:
	static void SetMasterVolume(float inVolume);
	static float GetMasterVolume();
};

// ------------------------------------------------------------------------
// Audio Instance
// ------------------------------------------------------------------------
class Audio
{
public:
	Audio(const wchar_t* fileName);

	// ------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------
	void Play();
	void Stop();
	void SetVolume(float inVolume);
	float GetVolume();
	void SetLoop(bool inState);
private:
	// ----------------------------------
	// Main : Initialization
	// ----------------------------------
	HRESULT OpenFile(const wchar_t* fileName);
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
private:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	float Volume = 1.f;
	bool bLoop = false;

	// ----------------------------------
	// State : CreateFile
	// ----------------------------------
	WAVEFORMATEXTENSIBLE wfx = {};

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	XAUDIO2_BUFFER m_buffer = {};
};