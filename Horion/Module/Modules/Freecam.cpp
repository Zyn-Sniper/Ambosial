#include "Freecam.h"
#include "../../../SDK/Utils/Utils.h"
#include "../../DrawUtils.h"
Freecam::Freecam() : IModule('V', Category::MISC, "Move your camera without moving the player.") {
	addFloat("Speed", &speed, speed, 0.50f, 1.25f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void* cameraAddr = (void*)FindSignature("FreeCam\\AddressShit::Function::Camera::GetEaguer\\48 89 5c 24 ? 55 56 57 48 83 ec ? 8b 41 ? 48 8b ea 48 8b 49\\\\Minecraft.Windows.exe+398F464\\Addr::f3 0f 10 15 ? ? ? ? 48 8d 0d ? ? ? ? 48 89 4c 24 ? 48 8d 54 24");

void Freecam::onEnable() {
	auto Player = Game.getLocalPlayer();
	pos = Player->getRenderPositionComponent()->renderPos;
	Player->setPlayerGameType(static_cast<GameType>(6));
}

void Freecam::onDisable() {
	auto Player = Game.getLocalPlayer();
	Player->getMovementProxy()->setPos(pos);
	Player->setPlayerGameType(static_cast<GameType>(0));
	Player->location->velocity.x = 0;
	Player->location->velocity.z = 0;
}

void Freecam::onPreRender(MinecraftUIRenderContext* rcx) {
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	auto Player = Game.getLocalPlayer();

	if (input == nullptr)
		return;

	if (Player == nullptr)
		return;
	DrawUtils::drawBox(pos.sub(0.3f, 1.62f, 0.3f), pos.add(0.3f, 0.18f, 0.3f), 0.3f, true);
}
