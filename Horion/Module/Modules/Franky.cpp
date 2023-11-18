#include "Franky.h"

using namespace std;
Franky::Franky() : IModule(0, Category::MISC, "does exactly what you think it does") {

}

const char* Franky::getModuleName() {
	return ("Franky");
}

void Franky::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}

void Franky::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}

void Franky::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}
