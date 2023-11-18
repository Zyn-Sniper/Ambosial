#include "TargetInfo.h"
#include "../../../SDK/Utils/Render/RenderUtil.h"

using namespace std;
TargetInfo::TargetInfo() : IModule(0, Category::VISUAL, "Display target on the HUD") {
	style.addEntry(EnumEntry("Box", 0))
		.addEntry(EnumEntry("Circle", 1));
	addEnum("Style", &style);
	addBool("3D", &vector3, vector3);
}

TargetInfo::~TargetInfo() {
}

const char* TargetInfo::getModuleName() {
	return ("TargetInfo");
}

static std::vector<Entity*> targetList;
void findEntityTargetHUD(Entity* currentEntity, bool isRegularEntity) {
	static auto targethudMod = moduleMgr->getModule<TargetInfo>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;
	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < 8) {
		targetList.push_back(currentEntity);
	}
}

void TargetInfo::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto killauraMod = moduleMgr->getModule<Killaura>();
	auto player = Game.getLocalPlayer();
	if (player == nullptr || !Game.canUseMoveKeys()) return;

	lolhurttimetest++;

	if (lolhurttimetest == 40) {
		lolhurttimetest = 0;
	}

	Vec2 windowSize = Game.getGuiData()->windowSize;
	float textsize = 1.f;
	static float animPercent = 0;

	targetList.clear();
	Game.forEachEntity(findEntityTargetHUD);
	if (!targetList.empty()) {
		Vec2 Position = Vec2(windowSize.x / 2, windowSize.y / 2);
		if (vector3) {
			Position = DrawUtils::worldToScreen(targetList[0]->getRenderPositionComponent()->renderPos);
			Position.x -= 20;
			if (Position.x < 0 || Position.y < 0) return;
		}
		//int health = targetList[0]->getHealth();
		string name = targetList[0]->getNameTag()->getText();
		float textWidth = DrawUtils::getTextWidth(&name, textsize);
		float textHeight = DrawUtils::getFontHeight(textsize);
		//float percentage = (health / 20) * 100;
		//animPercent += (percentage - animPercent) / 10;
		switch (style.selected) {
		case 0: {
			// DrawBackGround
			DrawUtils::drawImage("Assets/roundRect.png", Vec2(Position.x, Position.y - 5), Vec2(96 + 40, 27 + 15), Vec2(0.f, 0.f), Vec2(1.f, 1.f), MC_Color(0, 0, 0), 0.35);
			//RenderUtil::RenderImage("Assets/roundRect", Vec4(Position.x, Position.y + 6, Position.x + 96.000000 + 40, Position.y + 27.000000 + 10), Vec2(0.f, 0.f), Vec2(1.f, 1.f), MC_Color(0, 0, 0), 0.35f);
			//DrawUtils::fillRectangle(Vec4(Position.x, Position.y + 6, Position.x + 96.000000 + 40, Position.y + 27.000000 + 10), MC_Color(10, 10, 10, 100), 0.35f);
			// DrawUtils::drawGlowOutline(Vec4(Position.x, Position.y, Position.x + 96.000000 + 40, Position.y + 30.000000 + 10), MC_Color(0, 0, 0, 200 / 30), 30, 3);
			////DrawUtils::drawCircleFilled(Vec2(Position.x + 5, Position.y + 26), Vec2(20, 20), MC_Color(10, 10, 10), 0.35);
			// DrawUtils::drawImage("assets/roundHead.png", Vec2(Position.x + 3, Position.y + 6.5), Vec2(30, 30), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
			float steveOpacity = (lolhurttimetest * 80) / 8;
			float size = 30.f;
			auto damageTime = lolhurttimetest;
			int smoothing = 5;
			int smoothing2 = 15;
			int maxsize = 30;
			int minsize = 27;
			if (damageTime > 20) {
				if (damageTime > 5) {
					if (size > minsize)
						size -= ((size - minsize) / smoothing);
					else if (size < minsize)
						size += ((minsize - size) / smoothing);
				} else {
					if (size > maxsize)
						size -= ((size - maxsize) / smoothing2);
					else if (size < maxsize)
						size += ((maxsize - size) / smoothing2);
				}
			}
			float size2 = size / 2;
			float sizepercent = size / 3;
			
			if (lolhurttimetest > 20) {
				DrawUtils::drawImage("Assets/roundHead.png", Vec2(Position.x + 6, Position.y + 3.5), Vec2(30, 30), Vec2(0.f, 0.f), Vec2(1.f, 1.f), MC_Color(255, 114, 118), steveOpacity);
			} else {
				DrawUtils::drawImage("Assets/roundHead.png", Vec2(Position.x + 6, Position.y + 3.5), Vec2(30, 30), Vec2(0.f, 0.f), Vec2(1.f, 1.f), MC_Color(255, 255, 255), 1);
			}

			static bool a[16] = {false};
			static float randomx[16] = {0};
			static float randomy[16] = {0};
			Vec4 riseRect = Vec4(Position.x, Position.y, Position.x + 140, Position.y + 32);
			float imgsize = 26;

			// rise particle animation
			for (int i = 0; i < 16; i++) {
				if (lolhurttimetest > 20) {
					if (!a[i]) {
						randomx[i] = RandomFloat(riseRect.x, riseRect.x + imgsize + 15.5);
						randomy[i] = RandomFloat(riseRect.y, riseRect.y + imgsize + 12.5);
						a[i] = true;
					}
					randomx[i] *= (i % 8 < 4) ? 1.0005f : 0.9995f;
					randomy[i] *= (i % 4 < 2) ? 1.0005f : 0.9995f;
					float size2 = (i % 2 == 0) ? 1.1f : (i % 4 == 1) ? 0.7f
													: (i % 4 == 2)   ? 0.8f
																	 : 0.9f;
					DrawUtils::drawCircleFilled(Vec2(randomx[i], randomy[i]), Vec2(size2, size2), ColorUtil::ClientColor(1), 1);
				} else {
					a[i] = false;
				}
			}
			targetList.clear();
			Game.forEachEntity(findEntityTargetHUD);
			//DrawUtils::drawText(Vec2(3, 3), &std::to_string(textWidth), MC_Color(255, 255, 255), 2, 1);
			//DrawUtils::drawText(Vec2(3, 10), &std::to_string(textHeight), MC_Color(255, 255, 255), 2, 1);
			// DrawNameTag
			DrawUtils::drawText(Position.add(40, 17), &name, MC_Color(255, 255, 255), 1.30, 1);
			// DrawUtils::drawTextMC(Position.add(40, 7), &name, MC_Color(255, 255, 255), textsize, 1, true);
			////DrawUtils::fillRectangle(Vec4(Position.x + 1, Position.y + textHeight + 3, Position.x + textWidth + 6, Position.y + textHeight + 7), MC_Color(0, 0, 0), 0.35f);
			////DrawUtils::fillRectangle(Vec4(Position.x + 1, Position.y + textHeight + 3, Position.x + 1 + (((Position.x + textWidth + 6) - (Position.x + 1)) / 20) * health, Position.y + textHeight + 7), MC_Color(0, 255, 0), 1.f);
		} break;
		case 1: {
			DrawUtils::drawCircleFilled(Position, Vec2(100, 20), MC_Color(50, 50, 50), 1);
			int pc = animPercent * 2.55;
			MC_Color textColor = MC_Color(1 - pc, pc, 0);
			// DrawUtils::drawText(Position, &std::to_string((int)animPercent), textColor, textsize * 2, 1.f);
		} break;
		}
	}
}
