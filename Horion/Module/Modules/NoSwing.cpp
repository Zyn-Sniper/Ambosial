#include "NoSwing.h"

NoSwing::NoSwing() : IModule(0, Category::PLAYER, "Disable arm swing animation (Server cancels the AnimatePacket being sent)") {
	addBool("Server", &server, server);
}

NoSwing::~NoSwing() {}

const char* NoSwing::getModuleName() {
	return ("NoSwing");
}