#pragma once

#include "Framework/GameInstance.h"

class PtoGameInstance : public GameInstance
{
	friend class App;
public:
	static PtoGameInstance& Get();
protected:
	virtual void OpenWorldDelay() override;
};