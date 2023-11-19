#include "Spammer.h"
#include "../../../SDK/Utils/Utils.h"

int shittty = 0; 

Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay.") {
	addInt("Delay", &delay, delay, 1, 10);
	addInt("Length", &length, length, 1, 60);
	addBool("Bypass", &bypass, bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

void Spammer::onTick(GameMode* gm) {
	Odelay++;
	shittty++;
	if (Odelay > delay * 20) {
		TextHolder cmd;
		if (shittty > 0) {
			TextHolder cmd = "/p FlyGamer8908";
		} else if (shittty > 10) {
			TextHolder cmd = "/p disband";
		}
		if (shittty > 20) {
			shittty = 0;
		}
		CommandRequestPacket packet;
		packet.command = cmd;
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		Odelay = 0;
	}
}
