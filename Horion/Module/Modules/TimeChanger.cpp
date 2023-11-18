#include "TimeChanger.h"

TimeChanger::TimeChanger() : IModule(0, Category::VISUAL, "Changes the clientside time.") {
	addFloat("Modifier", &modifier, modifier, 0.1f, 1.f);
}

TimeChanger::~TimeChanger() {
}

const char* TimeChanger::getModuleName() {
	return "TimeChanger";
}
