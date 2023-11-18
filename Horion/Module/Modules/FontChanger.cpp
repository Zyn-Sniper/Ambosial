#include "FontChanger.h"

FontChanger::FontChanger() : IModule(0, Category::CLIENT, "Font Changer.") {
	Fonts.addEntry(EnumEntry("Notosans", 0)).addEntry(EnumEntry("Mojangles", 1));
	addEnum("Fonts", &Fonts);
}

FontChanger::~FontChanger() {
}

const char* FontChanger::getModuleName() {
	return ("CustomFont");
}