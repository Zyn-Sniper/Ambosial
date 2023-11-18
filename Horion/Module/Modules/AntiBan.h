#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class AntiBan : public IModule {
private:
	float a;

public:
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	AntiBan();
};