#pragma once

#include "Framework/GameInstance.h"

class PtoGameInstance : public GameInstance
{
public:
	static PtoGameInstance& Get();
protected:
	virtual void OpenWorldDelay() override;
};