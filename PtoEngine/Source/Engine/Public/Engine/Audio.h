#pragma once

#include <xaudio2.h>

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	VoiceCallback();
	~VoiceCallback();

	//Called when the voice has just finished playing a contiguous audio stream.
	virtual void __stdcall OnStreamEnd() override;

	//Unused methods are stubs
	virtual void __stdcall OnVoiceProcessingPassEnd() override;
	virtual void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) override;
	virtual void __stdcall OnBufferEnd(void* pBufferContext) override;
	virtual void __stdcall OnBufferStart(void* pBufferContext) override;
	virtual void __stdcall OnLoopEnd(void* pBufferContext) override;
	virtual void __stdcall OnVoiceError(void* pBufferContext, HRESULT Error) override;
};

// ------------------------------------------------------------------------
// AudioManager
// ------------------------------------------------------------------------
class AudioManager
{
public:
	AudioManager();
	virtual ~AudioManager();

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	static AudioManager& Get();

	void SetMasterVolume(float inVolume);
	float GetMasterVolume() const;

	IXAudio2* GetAudioDevice();
private:
	IXAudio2* pXAudio2 = nullptr;
	IXAudio2MasteringVoice* pMasterVoice = nullptr;
};


// ------------------------------------------------------------------------
// Audio Instance
// ------------------------------------------------------------------------
class Audio
{
public:
	Audio(const wchar_t* fileName);
	virtual ~Audio();

	// ------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------
	void Play();
	void Stop();
	bool IsPlaying() const;
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
	bool bIsPlay = false;

	// ----------------------------------
	// State : CreateFile
	// ----------------------------------
	WAVEFORMATEXTENSIBLE wfx = {};

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	XAUDIO2_BUFFER m_buffer = {};
	VoiceCallback callback;
};