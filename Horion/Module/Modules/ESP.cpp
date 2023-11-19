#include "ESP.h"
#include "../../FriendList/FriendsManager.h"
#include "../../../SDK/Utils/Target.h"
#include "../../../SDK/Utils/ColorUtil.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you.") {
	addBool("Rainbow", &doRainbow, doRainbow);
	addBool("MobEsp", &isMobEsp, isMobEsp);
	addBool("2D", &is2d, is2d);
	addBool("Self", &self, self);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(Entity* ent, bool isRegularEntitie) {
	static auto espMod = moduleMgr->getModule<ESP>();
	
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (ent == localPlayer)
		return;
	static auto noFriendsModule = moduleMgr->getModule<NoFriends>();
	if (!noFriendsModule->isEnabled() && FriendsManager::findFriend(ent->getNameTag()->getText())) {
		DrawUtils::setColor(0.1f, 0.9f, 0.1f, 1.f);
	} else if (Target::isValidTarget(ent)) {
		if (espMod->doRainbow)
			DrawUtils::setColor(ColorUtil::ClientColor(1).r, ColorUtil::ClientColor(1).b, ColorUtil::ClientColor(1).g, 255);
		else
			DrawUtils::setColor(0.9f, 0.9f, 0.9f, 1.f);
	} else if (espMod->isMobEsp) {
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;

		if (ent->getEntityTypeId() == 64)  //Item
			return;
		
		if (ent->isInvisible())
			return;

		if (!localPlayer->canAttack(ent, false))
			return;
		if (espMod->doRainbow)
			DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], 1.f);
		else
			DrawUtils::setColor(0.9f, 0.9f, 0.9f, 1.f);
	} else
		return;
	if (espMod->is2d) {
		Vec3 position = ent->getRenderPositionComponent()->renderPos;
		// DrawUtils::drawBox(position.add(-0.5, 0.2, -0.5), position.add(0.5, 0.1, 0.5), 0.3f, true);
		DrawUtils::draw2D(ent, 0.5);
		//DrawUtils::draw2DBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.3f, false, true);
	}
	else 
		DrawUtils::drawEntityBox(ent, (float)fmax(0.5f, 1 / (float)fmax(1, localPlayer->getRenderPositionComponent()->renderPos.dist(ent->getRenderPositionComponent()->renderPos))));
}

void ESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys() && self) {
		Vec3 position = localPlayer->getRenderPositionComponent()->renderPos;
		DrawUtils::setColor(ColorUtil::ClientColor(1).r, ColorUtil::ClientColor(1).b, ColorUtil::ClientColor(1).g, 255);
		DrawUtils::draw2D(localPlayer, 0.6f);
		//DrawUtils::draw2DBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.6f, false, true);
	}

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		// Rainbow colors
		{
			if (rcolors[3] < 1) {
				rcolors[0] = 0.2f;
				rcolors[1] = 0.2f;
				rcolors[2] = 1.f;
				rcolors[3] = 1;
			}

			Utils::ApplyRainbow(rcolors, 0.0015f);
		}

		Game.forEachEntity(doRenderStuff);
	}
}
