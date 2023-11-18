#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class ItemView : public IModule {
private:
	//

public:
	bool shouldBlock = false;
	int swingDuration = 6;

	ItemView();
	~ItemView();

	SettingEnum mode = SettingEnum(this);

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
