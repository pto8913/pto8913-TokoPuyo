
#include "Audio.h"

#include "Utils/DirectXUtils.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

// ------------------------------------------------------------------------
// AudioManager
// ------------------------------------------------------------------------
class AudioManager
{
public:
    AudioManager();
    virtual ~AudioManager();

private:
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
public:

    static AudioManager& Get();

    IXAudio2* GetAudioDevice();
    IXAudio2MasteringVoice* GetMasterVoice();
private:
    IXAudio2* pXAudio2 = nullptr;
    IXAudio2MasteringVoice* pMasterVoice = nullptr;
};

AudioManager::AudioManager()
{
    HRESULT result = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Initialize Audio Failed\n");
#endif
        return;
    }

    result = pXAudio2->CreateMasteringVoice(&pMasterVoice);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Initialize Mastering Voice Failed\n");
#endif
        return;
    }
}
AudioManager::~AudioManager()
{
    Util::SafeRelease(pXAudio2);
    pMasterVoice = nullptr;
}
AudioManager& AudioManager::Get()
{
    static AudioManager instance;
    return instance;
}

IXAudio2* AudioManager::GetAudioDevice()
{
    return pXAudio2;
}
IXAudio2MasteringVoice* AudioManager::GetMasterVoice()
{
    return pMasterVoice;
}

// ------------------------------------------------------------------------
// AudioManagerHelper
// ------------------------------------------------------------------------
void AudioHelperLibrary::SetMasterVolume(float inVolume)
{
    AudioManager::Get().GetMasterVoice()->SetVolume(inVolume);
}
float AudioHelperLibrary::GetMasterVolume()
{
    float Out;
    AudioManager::Get().GetMasterVoice()->GetVolume(&Out);
    return Out;
}

// ------------------------------------------------------------------------
// Audio Instance
// ------------------------------------------------------------------------
Audio::Audio(const wchar_t* fileName)
{
    OpenFile(fileName);

    SetVolume(Volume);
}

// ------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------
void Audio::Play()
{
    if (pSourceVoice != nullptr)
    {
        pSourceVoice->Stop(0);
        pSourceVoice->FlushSourceBuffers();
        pSourceVoice->SubmitSourceBuffer(&m_buffer);
        pSourceVoice->Start(0);
        //WaitForSingleObjectEx(callback.hBufferEndEvent, INFINITE, TRUE);
    }
    else
    {
#if _DEBUG
        OutputDebugStringA("Source Voice is not valid\n");
#endif
    }
}
void Audio::Stop()
{
    if (pSourceVoice != nullptr)
    {
        pSourceVoice->Stop(0);
    }
}
void Audio::SetVolume(float inVolume)
{
    Volume = inVolume;
    if (pSourceVoice)
    {
        pSourceVoice->SetVolume(Volume);
    }
}
float Audio::GetVolume()
{
    return Volume;
}
void Audio::SetLoop(bool inState)
{
    bLoop = inState;
    if (bLoop)
    {
        m_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
}

// ----------------------------------
// Main : Initialization
// ----------------------------------
HRESULT Audio::OpenFile(const wchar_t* fileName)
{
    HANDLE hFile = CreateFile(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (INVALID_HANDLE_VALUE == hFile)
    {
#if _DEBUG
        OutputDebugStringA("file HANDLE is Invalid\n");
#endif
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
#if _DEBUG
        OutputDebugStringA("file pointer is Invalid\n");
#endif
        return HRESULT_FROM_WIN32(GetLastError());
    }

    DWORD chunkSize;
    DWORD chunkPosition;
    HRESULT result = FindChunk(hFile, fourccRIFF, chunkSize, chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Find Chunk Failed 0\n");
#endif
        return S_FALSE;
    }
    DWORD fileType;
    result = ReadChunkData(hFile, &fileType, sizeof(DWORD), chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Read Chunk Failed 0\n");
#endif
        return S_FALSE;
    }
    if (fileType != fourccWAVE)
    {
        return S_FALSE;
    }

    result = FindChunk(hFile, fourccFMT, chunkSize, chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Find Chunk Failed 1\n");
#endif
        return S_FALSE;
    }
    result = ReadChunkData(hFile, &wfx, chunkSize, chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Read Chunk Failed 1\n");
#endif
        return S_FALSE;
    }
    //fill out the audio data buffer with the contents of the fourccDATA chunk
    result = FindChunk(hFile, fourccDATA, chunkSize, chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Find Chunk Failed 2\n");
#endif
        return S_FALSE;
    }
    BYTE* pDataBuffer = new BYTE[chunkSize];
    result = ReadChunkData(hFile, pDataBuffer, chunkSize, chunkPosition);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Read Chunk Failed 2\n");
#endif
        return S_FALSE;
    }
    m_buffer.AudioBytes = chunkSize;  //size of the audio m_buffer in bytes
    m_buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    m_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    result = AudioManager::Get().GetAudioDevice()->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);
    if (FAILED(result))
    {
#if _DEBUG
        OutputDebugStringA("Create Source Voice Failed\n");
#endif
        return result;
    }

    return S_OK;
}

HRESULT Audio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }
    return S_OK;
}

HRESULT Audio::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}