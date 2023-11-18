#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class Theme : public IModule {
public:
	float r = 1.f, g = 77.f, b = 106.f;
	float r2 = 255.f, g2 = 255.f, b2 = 255.f;

	bool nametag = true;

	Theme() : IModule(0x0, Category::MISC, "Changes the games Internal timer") {
		addFloat("Red", &r, r, 0.f, 255.f);
		addFloat("Green", &g, g, 0.f, 255.f);
		addFloat("Blue", &b, b, 0.f, 255.f);
		addFloat("Red2", &r2, r2, 0.f, 255.f);
		addFloat("Green2", &g2, g2, 0.f, 255.f);
		addFloat("Blue2", &b2, b2, 0.f, 255.f);
		addBool("Nametags", &nametag, nametag);
	};
	~Theme(){};

	void onPostRender(MinecraftUIRenderContext* renderCtx) override;

	virtual const char* getModuleName() override {
		return "Theme";
	}
};