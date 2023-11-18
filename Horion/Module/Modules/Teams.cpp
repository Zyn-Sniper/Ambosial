#include "Teams.h"

Teams::Teams() : IModule(0, Category::COMBAT, "Don't attack team members!") {
	addBool("Is Allied", &alliedCheck, alliedCheck);
	addBool("Same Color", &colorCheck, colorCheck);
}

Teams::~Teams() {
}

const char* Teams::getModuleName() {
	return ("Teams");
}
