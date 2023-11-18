#pragma once

#include "Module.h"

class ClickGuiMod : public IModule {
public:
	ClickGuiMod();
	~ClickGuiMod();

	bool showTooltips = true;

	float openAnim = 1.f;
	int openTime = 0;
	int openTimeOffset = 0;

	SettingEnum theme = this;

	// Inherited via IModule
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual const char* getModuleName() override;
	virtual void onPlayerTick(Player* player);
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual bool allowAutoStart() override;
	virtual void onLoadConfig(void* conf) override;
	virtual void onSaveConfig(void* conf) override;
};
