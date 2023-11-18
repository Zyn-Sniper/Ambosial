#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Enable this module to filter out bots!") {
	addBool("Hitbox Check", &hitboxCheck, hitboxCheck);
	addBool("ASCII Check", &nameCheck, nameCheck);
	addBool("Name Check", &nameCheckA, nameCheckA);
	addBool("Invisible Check", &invisibleCheck, invisibleCheck);
	addBool("EntityID Check", &entityIdCheck, entityIdCheck);
	addBool("Other Check", &otherCheck, otherCheck);
	addBool("Extra Check", &extraCheck, extraCheck);
	addBool("Player Check", &playerCheck, playerCheck);
}

AntiBot::~AntiBot() {
}

const char* AntiBot::getModuleName() {
	return ("AntiBot");
}
