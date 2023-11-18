#include "ClickGuiMod.h"
#include "../../Menu/ClickGui.h"

ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::CLIENT, "The clickgui - toggle everything just by clicking on it!") {
	addBool("Show Tooltips", &showTooltips, showTooltips);
	theme.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Polar", 1));
}

ClickGuiMod::~ClickGuiMod() {
}

const char* ClickGuiMod::getModuleName() {
	return ("ClickGui");
}

void ClickGuiMod::onEnable() {
	Game.getClientInstance()->releaseMouse();

	openAnim = -500;
	openTimeOffset = 0;
	openTime = 0;
}

bool ClickGuiMod::allowAutoStart() {
	return false;
}

void ClickGuiMod::onDisable() {
	Game.getClientInstance()->grabMouse();

	openTimeOffset = 0;
	openTime = 0;
}

void ClickGuiMod::onPlayerTick(Player* player) {
	if (Game.getLocalPlayer() == nullptr) return;

	if (openTimeOffset == 0)
		openTimeOffset = TimerUtil::getCurrentMs();
	if (openAnim < 150) openAnim += (28 - openAnim) * 0.09f;
	openTime = TimerUtil::getCurrentMs() - openTimeOffset;
}

void ClickGuiMod::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys())
		Game.getClientInstance()->releaseMouse();
}
void ClickGuiMod::onLoadConfig(void* conf) {
    IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
}
void ClickGuiMod::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	ClickGui::onSaveConfig(conf);
}
