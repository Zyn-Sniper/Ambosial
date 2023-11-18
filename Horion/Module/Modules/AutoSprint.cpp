#include "AutoSprint.h"

AutoSprint::AutoSprint() : IModule(0, Category::MOVEMENT, "Automatically sprint without holding the key.") {
	addBool("All Directions", &alldirections, alldirections);
}

AutoSprint::~AutoSprint() {
}

const char* AutoSprint::getModuleName() {
	return ("AutoSprint");
}

void AutoSprint::onTick(GameMode* gm) {
	if (gm->player->location->velocity.magnitudexz() > 0.01f) {
		GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
		if (alldirections || GameData::isKeyDown(*input->forwardKey))
		gm->player->getMovementProxy()->setSprinting(true);
	}
}
