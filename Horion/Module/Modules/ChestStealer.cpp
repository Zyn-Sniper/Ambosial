#include "ChestStealer.h"

#include "../ModuleManager.h"

ChestStealer::ChestStealer() : IModule(0, Category::PLAYER, "Automatically takes all items out of a chest.") {
	addInt("Delay", &setDelay, setDelay, 1, 30);
	//addInt("item", &itemm, itemm, 0, 56);
	addBool("Enhanced", &enhanced, enhanced);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

void ChestStealer::chestScreenController_tick(ChestScreenController* c) {
	if (c != nullptr && !Game.getLocalPlayer()->canOpenContainerScreen()) {
		if (56 == 56) {
			c->tryExit();
		}
		delay++;
		if (!Game.getLocalPlayer()->getSupplies()->inventory->isFull()) {
			if (delay > setDelay && setDelay > 0) {
				itemm++;
				delay = 0;
			}
			c->handleAutoPlace("container_items", itemm);
		}
	}
}

void ChestStealer::onTick(GameMode* gm) {
	if (itemm == 56) itemm = 0;
	if (Game.getLocalPlayer()->canOpenContainerScreen()) itemm = 0;
}