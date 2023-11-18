#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	addBool("RightClick", &rightclick, rightclick);
	addBool("Only Weapons", &weapons, weapons);
	addBool("Break Blocks", &breakBlocks, breakBlocks);
	addInt("Delay", &delay, delay, 0, 20);
	addBool("Hold", &hold, hold);
}

AutoClicker::~AutoClicker() {}

const char* AutoClicker::getModuleName() {
	return "AutoClicker";
}

void AutoClicker::onTick(GameMode* gm) {
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {
		LocalPlayer* player = Game.getLocalPlayer();
		Level* level = player->level;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = player->getSelectedItem();
			if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			player->swingArm();

			if (level->hasEntity() != 0)
				gm->attack(level->getEntity());
			else if (breakBlocks) {
				bool isDestroyed = false;
				gm->startDestroyBlock(level->block, level->blockSide, isDestroyed);
				gm->stopDestroyBlock(level->block);
				if (isDestroyed && player->getRegion()->getBlock(level->block)->blockLegacy->blockId != 0)
					gm->destroyBlock(&level->block, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((GameData::isRightClickDown() || !hold) && GameData::canUseMoveKeys()) {
			Level* level = Game.getLocalPlayer()->level;
			Odelay++;
			if (Odelay >= delay) {
				bool idk = true;
				gm->buildBlock(new Vec3i(level->block), level->blockSide, idk);
				Odelay = 0;
			}
		}
	}
}