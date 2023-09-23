#pragma once

#include "Framework/GameInstance.h"

class Audio;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPaused, bool);

class PtoGameInstance : public GameInstance
{
	friend class App;
public:
	static PtoGameInstance& Get();
	virtual void Initialize(DirectX11& dx) override;

	// -------------------------------------------------------
	// Main
	// -------------------------------------------------------
	void SetPause(bool inPause);

	// -----------------------------
	// Main : Audio
	// -----------------------------
	void SetBGM(bool inPlay);
	void OnAudioVolumeChanged(float inValue);

	// -----------------------------
	// Main : Delegates
	// -----------------------------
	FOnPaused OnPaused;
protected:
	// -----------------------------
	// Main : World
	// -----------------------------
	virtual void OpenWorldDelay() override;

	std::shared_ptr<Audio> pAudio_BGM = nullptr;
};