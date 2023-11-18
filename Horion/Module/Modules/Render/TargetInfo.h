#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class TargetInfo : public IModule {
public:
	int quality = 1;
	int lolhurttimetest = 1;
	bool vector3 = false;

	SettingEnum style = SettingEnum(this);

	TargetInfo();
	~TargetInfo();

	// Inherited via IModule
	virtual const char* getModuleName();
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
};
