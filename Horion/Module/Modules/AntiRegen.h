#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class AntiRegen : public IModule {
private:
	float range = 4.5f;
	Vec3i blockPos;
	int beforeSlot = 0;
	bool tryScaffold(Vec3 blockBelow);
	bool findBlock();

public:
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	AntiRegen();
};