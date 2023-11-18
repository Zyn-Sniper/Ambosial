#include "AntiBan.h"

using namespace std;
AntiBan::AntiBan() : IModule(0, Category::MISC, "It doesn't work") {
}

const char* AntiBan::getModuleName() {
	return ("AntiBan");
}

void AntiBan::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}

void AntiBan::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}

void AntiBan::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}
