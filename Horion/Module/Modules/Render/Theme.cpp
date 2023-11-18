#include "Theme.h"

void Theme::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys()) return;
}