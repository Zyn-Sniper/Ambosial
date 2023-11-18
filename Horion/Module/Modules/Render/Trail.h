#pragma once

#include "../Module.h"

class Trails : public IModule {
public:
	Trails() : IModule(0x0, Category::MISC, "Changes the games Internal timer") {

	};
	~Trails(){};
	struct CircleStatus {
		Vec3 pos;
		int duration;
	};
	std::vector<CircleStatus> Circles;

	int trailDuration = 20;
	int trailDelay = 5;

	int countDelay = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(GameMode* gm) override;
};
