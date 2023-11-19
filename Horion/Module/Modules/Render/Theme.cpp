#include "Theme.h"

#include "../../../../SDK/Utils/Render/RenderUtil.h"

using namespace std;
void Theme::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys()) return;

	// Watermark
	string name = localPlayer->getNameTag()->getText();
	name = Utils::sanitize(name);
	name = name.substr(0, name.find('\n'));
	string fpsText = to_string(Game.getFPS()) + "fps";

	std::string WatermarkText = +WHITE + std::string("ambosial") + WHITE + " | 5.0 | " + RESET + WHITE + name +
								" "
								"| " +
								fpsText;

	float textHeight = 12.f * 1;
	float textPadding = 1.f * 1;

	auto WatermarkLen = RenderUtil::getTextLen(&WatermarkText, 1, MOJANG) + 6.f;
	auto height = textHeight + (textPadding * 2);

	Vec4 newRect = Vec4(5, 7, 5 + WatermarkLen, 7 + textHeight + (textPadding * 2));

	if (watermark) {
		RenderUtil::RenderRectangle(newRect, MC_Color(0, 0, 0, 70));
		RenderUtil::drawText(Vec2(newRect.x + (textPadding * 3), newRect.y + (textPadding * 3)), &WatermarkText, MC_Color(255, 255, 255), 1, 1, true, MOJANG);
		//RenderUtil::Glow(newRect);
	}

	// LevelInfo
	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	Vec3* currPos = Game.getLocalPlayer()->getPos();

	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	string PingLevelInfo = string("Ping: ") + to_string(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count());

	string FpsLevelInfo = string("FPS: ") + to_string(Game.getFPS());
	string XYZLevelInfo = "XYZ: " + std::to_string((int)floorf(currPos->x)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z));

	float FPSLen = RenderUtil::getTextLen(&FpsLevelInfo, 1, MOJANG) + 6.5;
	float XYZLen = RenderUtil::getTextLen(&XYZLevelInfo, 1, MOJANG) + 6.5;

	if (levelinfo) {
		RenderUtil::drawText(Vec2(4.f, windowSize.y - 22), &FpsLevelInfo, MC_Color(255, 255, 255), 1, 1, true, MOJANG);
		RenderUtil::drawText(Vec2(4.f, windowSize.y - 12), &XYZLevelInfo, MC_Color(255, 255, 255), 1, 1, true, MOJANG);
		RenderUtil::drawText(Vec2(4.f, windowSize.y - 32), &PingLevelInfo, MC_Color(255, 255, 255), 1, 1, true, MOJANG);
	}
}