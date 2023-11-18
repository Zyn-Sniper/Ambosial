#pragma once
#include "../Module.h"
class ChinaHat : public IModule {
public:
	int visualR = 255;
	int visualG = 255;
	int VisualB = 255;
	int Quality = 360;
	float alpha = 0.5;
	SettingEnum mode = this;

	ChinaHat() : IModule(0x0, Category::MISC, "Changes the games Internal timer") {
		addFloat("Opacity", &alpha, alpha, 0.f, 1.f);
	};
	~ChinaHat(){};

	void onLevelRender() override;

	virtual const char* getModuleName() override {
		return "ChinaHat";
	}
};