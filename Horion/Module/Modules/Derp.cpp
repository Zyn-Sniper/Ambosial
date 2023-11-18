#include "Derp.h"

Derp::Derp() : IModule(0, Category::MISC, "lol you stupid.") {
	addFloat("SpinSpeed", &speed, speed, 2.f, 20.f);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onPlayerTick(Player* player) {
	if (player == nullptr) return;
	yaw += speed;
	if (yaw > 180) yaw = -180;
	player->getMobBodyRotationComponent()->bodyRot = yaw;//clientside only lol
}
