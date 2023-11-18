#pragma once

#include "Module.h"
#include "../../../SDK/Camera.h"

class Freecam : public IModule {
public:
	float speed = 0.325f;
	float yaw = 0;
	Vec2 initialViewAngles;
	Camera* camera = nullptr;
	bool keyPressed = false;
        Vec3 pos;
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* rcx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
