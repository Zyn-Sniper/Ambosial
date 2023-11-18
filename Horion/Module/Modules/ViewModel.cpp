#include "ViewModel.h"


ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Custom item view model.") {
	addBool("Reset", &Reset, Reset);
	addBool("Translate", &doTranslate, doTranslate);
	addBool("Scale", &doScale, doScale);

	addFloat("TranslateX", &xTrans, 0.f, -2.f, 2.f);
	addFloat("TranslateY", &yTrans, 0.f, -2.f, 2.f);
	addFloat("TranslateZ", &zTrans, 0.f, -2.f, 2.f);

	addFloat("ScaleX", &xMod, 1.f, 0.f, 2.f);
	addFloat("ScaleY", &yMod, 1.f, 0.f, 2.f);
	addFloat("ScaleZ", &zMod, 1.f, 0.f, 2.f);
}

ViewModel::~ViewModel() {
}

const char* ViewModel::getModuleName() {
	return "ViewModel";
}

void ViewModel::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr)
		return;

	if (Reset) {
		xTrans = 0.f;
		yTrans = 0.f;
		zTrans = 0.f;

		xMod = 1.f;
		yMod = 1.f;
		zMod = 1.f;
		Reset = false;
	}
}

