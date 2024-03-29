#include "NightMode.h"

NightMode::NightMode() : IModule(0, Category::VISUAL, "Darkens the environment.") {
	//addFloat("modifier", &modifier, modifier, 0.1f, 1.f);
}

NightMode::~NightMode() {
}

const char* NightMode::getModuleName() {
	return "NightMode";
}

void NightMode::onEnable() {
	static auto rainbowSkyMod = moduleMgr->getModule<RainbowSky>();
	if (rainbowSkyMod->isEnabled())
		rainbowSkyMod->setEnabled(false);
}