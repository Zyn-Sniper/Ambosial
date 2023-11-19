#pragma once

#include <set>

#include "Module.h"

class NameTags : public IModule {
public:
	std::set<std::string> nameTags;
	bool displayArmor = true;
	bool underline = true;
	bool self = false;
	float opacity = 0.2f;
	NameTags();
	~NameTags();

	bool* ingameNametagSetting = nullptr;
	bool lastSetting = true;
	bool gotPrevSetting = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
};
