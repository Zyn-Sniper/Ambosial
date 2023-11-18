#pragma once

#include "Module.h"

class ChestStealer : public IModule {
private:
	int itemm = 0;
	int delay = 0;
	int setDelay = 7;
	bool enhanced = true;

public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void chestScreenController_tick(ChestScreenController* c);
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
};
